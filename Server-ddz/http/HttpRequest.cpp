#include "HttpRequest.h"
#include "Log.h"
#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


char* HttpRequest::splitRequestLine(const char* start, const char* end,
	const char* sub, std::function<void(std::string)> callback)
{
	char* space = const_cast<char*>(end);
	if (sub != nullptr) {
		space = (char*)memmem(start, end - start, sub, strlen(sub));
		assert(space != NULL);
	}
	int length = space - start;
	callback(std::string(start, length));
	return space + 1;
}

// 将字符转换为整形数
int HttpRequest::hexToDec(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;

	return 0;
}

HttpRequest::HttpRequest()
{
	reset();
}

void HttpRequest::reset()
{
	reqHeaders_.clear();
	methor_ = url_ = version_ = "";
	curState_ = HttpRequestState::ParseReqLine;
}

void HttpRequest::addHeader(const std::string key, const std::string value)
{
	if (key.empty() || value.empty())
		return;
	reqHeaders_.insert(std::make_pair(key, value));
}

std::string HttpRequest::getHeader(const std::string key)
{
	auto iter = reqHeaders_.find(key);
	if (iter == reqHeaders_.end())
		return std::string();
	return iter->second;
}

bool HttpRequest::parseRequestLine(Buffer* readBuf)
{
	// 读出请求行，保存字符串结束地址
	char* end = readBuf->findCRLF();
	// 保存字符串起始地址
	char* start = readBuf->getData() + readBuf->getReadPos();
	// 请求行长度
	int lineSize = end - start;

	if (lineSize > 0) {
		// get /xxx/1.jpg http/1.1\r\n
		auto methorFunc = std::bind(&HttpRequest::setMethor, this, std::placeholders::_1);
		auto urlFunc = std::bind(&HttpRequest::setUrl, this, std::placeholders::_1);
		auto versionFunc = std::bind(&HttpRequest::setVersion, this, std::placeholders::_1);
		start = splitRequestLine(start, end, " ", methorFunc);
		start = splitRequestLine(start, end, " ", urlFunc);
		splitRequestLine(start, end, NULL, versionFunc);

		// 准备解析请求头
		readBuf->setReadPos(readBuf->getReadPos() + lineSize + 2);	// 跳到下一行开始
		curState_ = HttpRequestState::ParseReqHeader;				// 更改状态
		return true;
	}
	return false;
}

bool HttpRequest::parseRequestHeader(Buffer* readBuf)
{
	char* end = readBuf->findCRLF();
	if (end != nullptr) {
		char* start = readBuf->getData() + readBuf->getReadPos();
		int lineSize = end - start;
		char* middle = (char*)memmem(start, lineSize, ": ", 2);
		if (middle != nullptr) {
			auto key_tmp = std::string(start, middle - start);
			auto value_tmp = std::string(middle + 2, end - middle - 2);

			addHeader(key_tmp, value_tmp);							// 添加键值对
			readBuf->setReadPos(readBuf->getReadPos() + lineSize + 2);
		}
		else {
			// 遇到空行，请求头已经解析完了
			readBuf->setReadPos(readBuf->getReadPos() + 2);			//跳过空行
			curState_ = HttpRequestState::ParseReqDone;	//只考虑get请求不考虑post
		}
		return true;
	}
	return false;
}

bool HttpRequest::parseHttpRequest(Buffer* readBuf, HttpResponse* response, Buffer* sendBuf, int socket)
{
	bool flag = true;
	while (curState_ != HttpRequestState::ParseReqDone) {
		switch (curState_)
		{
		case HttpRequestState::ParseReqLine:
			flag = parseRequestLine(readBuf);
			break;
		case HttpRequestState::ParseReqHeader:
			flag = parseRequestHeader(readBuf);
			break;
		case HttpRequestState::ParseReqBody:
			break;
		default:
			break;
		}
		if (!flag)	return flag;

		// 如果解析完成则准备回复数据
		if (curState_ == HttpRequestState::ParseReqDone) {
			// 解析请求，把东西塞到response里
			processHttpRequest(response);
			// 把response里的东西写到sendBuf
			response->prepareMsg(sendBuf, socket);
		}
	}
	curState_ = HttpRequestState::ParseReqLine;
	return flag;
}

bool HttpRequest::processHttpRequest(HttpResponse* response)
{
	if (strcasecmp(methor_.data(), "get") != 0)
	{
		return -1;
	}
	url_ = decodeMsg(url_);
	// 处理客户端请求的静态资源(目录或者文件)
	const char* file = NULL;
	if (strcmp(url_.data(), "/") == 0)
	{
		file = "./";
	}
	else
	{
		file = url_.data() + 1;
	}
	// 获取文件属性
	struct stat st;
	int ret = stat(file, &st);
	if (ret == -1)
	{
		// 文件不存在 -- 回复404
		response->setFileName("404.html");
		response->setStatusCode(StatusCode::NotFound);
		// 响应头
		response->addHeader("Content-Type", getFileType("404.html"));
		response->sendDataFunc_ = std::bind(&HttpRequest::sendFile, this,
			std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		return 0;
	}

	response->setFileName(file);
	response->setStatusCode(StatusCode::OK);
	// 判断文件类型
	if (S_ISDIR(st.st_mode))
	{
		// 把这个目录中的内容发送给客户端
		// 响应头
		response->addHeader("Content-Type", getFileType(".html"));
		response->sendDataFunc_ = std::bind(&HttpRequest::sendDir, this,
			std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}
	else
	{
		// 把文件的内容发送给客户端
		// 响应头
		response->addHeader("Content-type", getFileType(file));
		response->addHeader("Content-length", std::to_string(st.st_size));
		response->sendDataFunc_ = std::bind(&HttpRequest::sendFile, this,
			std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}

	return false;
}

const std::string HttpRequest::getFileType(const std::string name)
{
	// a.jpg a.mp4 a.html
	// 自右向左查找‘.’字符, 如不存在返回NULL
	const char* dot = strrchr(name.data(), '.');
	if (dot == NULL)
		return "text/plain; charset=utf-8";	// 纯文本
	if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
		return "text/html; charset=utf-8";
	if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
		return "image/jpeg";
	if (strcmp(dot, ".gif") == 0)
		return "image/gif";
	if (strcmp(dot, ".png") == 0)
		return "image/png";
	if (strcmp(dot, ".css") == 0)
		return "text/css";
	if (strcmp(dot, ".au") == 0)
		return "audio/basic";
	if (strcmp(dot, ".wav") == 0)
		return "audio/wav";
	if (strcmp(dot, ".avi") == 0)
		return "video/x-msvideo";
	if (strcmp(dot, ".mov") == 0 || strcmp(dot, ".qt") == 0)
		return "video/quicktime";
	if (strcmp(dot, ".mpeg") == 0 || strcmp(dot, ".mpe") == 0)
		return "video/mpeg";
	if (strcmp(dot, ".vrml") == 0 || strcmp(dot, ".wrl") == 0)
		return "model/vrml";
	if (strcmp(dot, ".midi") == 0 || strcmp(dot, ".mid") == 0)
		return "audio/midi";
	if (strcmp(dot, ".mp3") == 0)
		return "audio/mpeg";
	if (strcmp(dot, ".ogg") == 0)
		return "application/ogg";
	if (strcmp(dot, ".pac") == 0)
		return "application/x-ns-proxy-autoconfig";

	return "text/plain; charset=utf-8";
}

std::string HttpRequest::decodeMsg(std::string msg)
{
	std::string str = std::string();
	const char* from = msg.data();
	for (; *from != '\0'; ++from)
	{
		// isxdigit -> 判断字符是不是16进制格式, 取值在 0-f
		// Linux%E5%86%85%E6%A0%B8.jpg
		if (from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2]))
		{
			// 将16进制的数 -> 十进制 将这个数值赋值给了字符 int -> char
			// B2 == 178
			// 将3个字符, 变成了一个字符, 这个字符就是原始数据
			str.append(1, hexToDec(from[1]) * 16 + hexToDec(from[2]));

			// 跳过 from[1] 和 from[2] 因此在当前循环中已经处理过了
			from += 2;
		}
		else
		{
			// 字符拷贝, 赋值
			str.append(1, *from);
		}
	}
	str.append(1, '\0');
	return str;
}

void HttpRequest::sendFile(const std::string fileName, Buffer* sendBuf, int cfd)
{// 把文件内容输出到sendBuf
	// 1. 打开文件
	int fd = open(fileName.data(), O_RDONLY);
	assert(fd > 0);
#if 1 
	while (1)
	{
		char buf[1024];
		int len = read(fd, buf, sizeof buf);
		if (len > 0)
		{
			//send(cfd, buf, len, 0);
			sendBuf->appendString(buf, len);
#ifndef MSG_SEND_DATA
			sendBuf->sendData(cfd);
#endif
		}
		else if (len == 0)
		{
			break;
		}
		else
		{
			close(fd);
			perror("read");
		}
	}
#else
	off_t offset = 0;
	int size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	while (offset < size)
	{
		int ret = sendfile(cfd, fd, &offset, size - offset);
		printf("ret value: %d\n", ret);
		if (ret == -1 && errno == EAGAIN)
		{
			printf("没数据...\n");
		}
	}
#endif
	close(fd);
	return;
}

void HttpRequest::sendDir(const std::string dirName, Buffer* sendBuf, int cfd)
{// 把目录内容输出到sendBuf
	char buf[4096] = { 0 };
	sprintf(buf, "<html><head><title>%s</title></head><body><table>", dirName.data());
	struct dirent** namelist;
	int num = scandir(dirName.data(), &namelist, NULL, alphasort);
	for (int i = 0; i < num; ++i)
	{
		// 取出文件名 namelist 指向的是一个指针数组 struct dirent* tmp[]
		char* name = namelist[i]->d_name;
		struct stat st;
		char subPath[1024] = { 0 };
		sprintf(subPath, "%s/%s", dirName.data(), name);
		stat(subPath, &st);
		if (S_ISDIR(st.st_mode))
		{
			// a标签 <a href="">name</a>
			sprintf(buf + strlen(buf),
				"<tr><td><a href=\"%s/\">%s</a></td><td>%ld</td></tr>",
				name, name, st.st_size);
		}
		else
		{
			sprintf(buf + strlen(buf),
				"<tr><td><a href=\"%s\">%s</a></td><td>%ld</td></tr>",
				name, name, st.st_size);
		}
		//send(cfd, buf, strlen(buf), 0);
		sendBuf->appendString(buf);
#ifndef MSG_SEND_DATA
		sendBuf->sendData(cfd);
#endif // !MSG_SEND_DATA
		memset(buf, 0, sizeof(buf));
		free(namelist[i]);
	}
	sprintf(buf, "</table></body></html>");
	//send(cfd, buf, strlen(buf), 0);
	sendBuf->appendString(buf);
#ifndef MSG_SEND_DATA
	sendBuf->sendData(cfd);
#endif // !MSG_SEND_DATA
	free(namelist);
}


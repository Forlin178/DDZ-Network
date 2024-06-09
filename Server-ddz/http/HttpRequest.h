#pragma once
#include "Buffer.h"
#include "HttpResponse.h"
#include <string>
#include <map>
#include <functional>

enum class HttpRequestState :char	// 占用内存少
{
	ParseReqLine,		// 解析请求行
	ParseReqHeader,		// 解析请求头
	ParseReqBody,		// 解析请求数据块(跳过了空行)
	ParseReqDone		// 解析完成
};

class HttpRequest
{
public:
	HttpRequest();

	// 重置
	void reset();

	// 获取解析状态
	inline HttpRequestState getState() { return curState_; }

	// 添加请求头键值对
	void addHeader(const std::string key, const std::string value);
	// 获取请求头键值对
	std::string getHeader(const std::string key);

	// 解析请求行
	bool parseRequestLine(Buffer* readBuf);
	// 解析请求头中的一行
	bool parseRequestHeader(Buffer* readBuf);
	// 解析http请求协议
	bool parseHttpRequest(Buffer* readBuf, HttpResponse* response,
		Buffer* sendBuf, int socket);
	// 处理http请求协议
	bool processHttpRequest(HttpResponse* response);

	// 获取对应content-type的值
	const std::string getFileType(const std::string name);
	std::string decodeMsg(std::string from);
	void sendFile(const std::string fileName, Buffer* sendBuf, int cfd);
	void sendDir(const std::string dirName, Buffer* sendBuf, int cfd);

	inline void setMethor(std::string methor) { methor_ = methor; }
	inline void setUrl(std::string url) { url_ = url; }
	inline void setVersion(std::string version) { version_ = version; }

private:
	char* splitRequestLine(const char* start, const char* end,
		const char* sub, std::function<void(std::string)> callback);
	int hexToDec(char c);

private:
	HttpRequestState curState_;

	// 请求行的三部分
	std::string methor_;
	std::string url_;
	std::string version_;

	// 请求头键值对数组
	std::map<std::string, std::string> reqHeaders_;
};

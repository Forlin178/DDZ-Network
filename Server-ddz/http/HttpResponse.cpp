#include "HttpResponse.h"
#include "TcpConnection.h"
#include "Log.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

HttpResponse::HttpResponse()
{
	headers_.clear();
	fileName_ = std::string();
	statusCode_ = StatusCode::Unknown;
	sendDataFunc_ = nullptr;
}

void HttpResponse::addHeader(const std::string key, const std::string value)
{
	if (key.empty() || value.empty())	return;
	headers_.insert(std::make_pair(key, value));
}

void HttpResponse::prepareMsg(Buffer* sendBuf, int socket)
{
	// 状态行
	char temp[1024] = { 0 };
	int code = static_cast<int>(statusCode_);
	sprintf(temp, "HTTP/1.1 %d %s\r\n", code, info_.at(code).data());
	sendBuf->appendString(temp);
	// 响应头
	for (const auto& iter:headers_) {
		sprintf(temp, "%s: %s\r\n", iter.first.data(), iter.second.data());
		sendBuf->appendString(temp);
	}
	// 空行
	sendBuf->appendString("\r\n");

#ifndef MSG_SEND_DATA
	sendBuf->sendData(socket);
#endif // !MSG_SEND_DATA

	// 回复的数据
	sendDataFunc_(fileName_, sendBuf, socket);
}

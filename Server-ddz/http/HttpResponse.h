#pragma once
#include "Buffer.h"
#include <string>
#include <map>
#include <functional>

enum class StatusCode
{
	Unknown,
	OK = 200,
	MovedPermanently = 301,
	MovedTemporarily = 302,
	BadRequest = 400,
	NotFound = 404
};

class HttpResponse
{
public:
	using responseBody = std::function<void(const std::string, Buffer*, int)>;

	HttpResponse();

	void addHeader(const std::string key, const std::string value);// 添加响应头	
	void prepareMsg(Buffer* sendBuf, int socket);	// 组织http响应数据
	responseBody sendDataFunc_;						// 发送包体的函数

	inline void setFileName(std::string name) { fileName_ = name; }
	inline void setStatusCode(StatusCode code) { statusCode_ = code; }

private:
	std::string fileName_;
	StatusCode statusCode_;							// 状态码
	std::map<std::string, std::string> headers_;	// 响应头: 键值对
	const std::map<int, std::string> info_ = {		// 状态码和说明的对应关系
		{200, "OK"},
		{301, "MovedPermanently"},
		{302, "MovedTemporarily"},
		{400, "BadRequest"},
		{404, "NotFound"}
	};
};


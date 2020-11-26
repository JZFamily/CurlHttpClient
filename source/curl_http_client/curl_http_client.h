//////////////////////////////////////////////////////////////////////////////////
///
///
///
///
///
/////////////////////////////////////////////////////////////////////////////////

#include "i_curl_http_client.h"
#include "curl/curl.h"
#include <string>
#ifndef __CURL_HTTP_CLIENT_H_
#define __CURL_HTTP_CLIENT_H_

class libCurlInit
{
public:
	bool static islibCurlInit() { static libCurlInit libcurl; return libcurl.m_error == 0; };
	~libCurlInit();

private:
	libCurlInit();
	libCurlInit(const libCurlInit&) = delete;
	libCurlInit(libCurlInit&&) = delete;
	int m_error;
};


class CurlHttpClient : public ICurlHttpClient
{
public:
	CurlHttpClient();
	~CurlHttpClient();
public:
	virtual bool exec(const std::string& url);
	
public:
	virtual bool setVerbose(bool onoff = false);
	virtual bool appendHeader(const std::string& headerStr);
	virtual bool resetHeader();
	virtual bool setCert(const std::string& certPath);
	virtual bool setTCPKeepAlive(bool enable = false, long delay = 60, long interwal = 60);
public:
	virtual bool get(const std::string& url);
	virtual bool post(const std::string& url, const std::string& postData);
	virtual bool put(const std::string& url, const std::string& putData);
	virtual bool delete_(const std::string& url);
public:
	virtual std::string getResponse() { return m_resData.first;};
	virtual void setResponseCallBack(std::function<void(const std::string&, std::string&)>callback = nullptr) { m_resData.second = callback; };
private:
	static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
	static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userdata);
	bool setResData();
private:
	CURL *m_pHandle;
	typedef std::pair<std::string, std::function<void(const std::string&, std::string&)>> ResponsePack;
	ResponsePack m_resData;
	struct curl_slist *m_headers;
};

#endif // !__CURL_HTTP_CLIENT_H_
//////////////////////////////////////////////////////////////////////////////////
///
///
///
///
///
/////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <functional>
#ifndef __I_CURL_HTTP_CLIENT_
#define __I_CURL_HTTP_CLIENT_

#if WIN32

#ifdef DLL_EXPORT_CURL_HTTP_CLIENT
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif // DLL_EXPORT

#endif // 



 class DLL_API ICurlHttpClient
{
public:
	virtual ~ICurlHttpClient() {};

public:
	virtual bool setVerbose(bool onoff = false) = 0;
	virtual bool appendHeader(const std::string& headerStr) = 0;
	virtual bool resetHeader() = 0;
	virtual bool setCert(const std::string& certPath) = 0;
	virtual bool get(const std::string& url) = 0;
	virtual bool post(const std::string& url,const std::string& postData) = 0;
	virtual bool put(const std::string& url, const std::string& putData) = 0;
	virtual bool delete_(const std::string& url) = 0;
	virtual bool setTCPKeepAlive(bool enable = false, long delay =60, long interwal = 60) = 0;
public:
	virtual std::string getResponse() = 0;
	virtual void setResponseCallBack(std::function<void(const std::string&, std::string&)>callback = nullptr) = 0;
};

 extern "C" DLL_API ICurlHttpClient * createCurlHttpClient();
 extern "C" DLL_API void releaseCurlHttpClient(ICurlHttpClient *pICurlHttpClient);

#endif // !__I_CURL_HTTP_CLIENT_

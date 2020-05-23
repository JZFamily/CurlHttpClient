#include "curl_http_client.h"

extern "C"
{
	ICurlHttpClient * createCurlHttpClient()
	{
		return new CurlHttpClient();
	}
	void releaseCurlHttpClient(ICurlHttpClient *pICurlHttpClient)
	{
		delete pICurlHttpClient;
	}
}

libCurlInit::libCurlInit()
	:m_error(0)
{
	m_error = (int) curl_global_init(CURL_GLOBAL_ALL);
}

libCurlInit::~libCurlInit()
{
	curl_global_cleanup();
}

CurlHttpClient::CurlHttpClient()
	:m_pHandle(nullptr)
	, m_headers(nullptr)
{
	if (!libCurlInit::islibCurlInit())
	{
		throw std::runtime_error("libCurlInit failed!");
	}
	m_pHandle = curl_easy_init();
}

CurlHttpClient::~CurlHttpClient()
{
	if (m_pHandle)
	{
		curl_easy_cleanup(m_pHandle);
	}
	resetHeader();
}

size_t CurlHttpClient::write_callback(char * ptr, size_t size, size_t nmemb, void * userdata)
{
	std::string *puserdata = (std::string *)userdata;
	if (puserdata == nullptr)
	{
		return 0;
	}
	size_t realsize = size * nmemb;
	puserdata->append(ptr, realsize);
	return realsize;
}

bool CurlHttpClient::setResData()
{
	CURLcode ret = CURLE_OK;
	m_resData.clear();
	ret = curl_easy_setopt(m_pHandle, CURLOPT_WRITEDATA, &m_resData);
	if (ret != CURLE_OK)
	{
		return false;
	}
	ret = curl_easy_setopt(m_pHandle, CURLOPT_WRITEFUNCTION, write_callback);
	return ret == CURLE_OK ? true : false;
}

bool CurlHttpClient::exec(const std::string & url)
{
	return false;
}
bool CurlHttpClient::setVerbose(bool onoff)
{
	long lonoff = onoff == true ? 1 : 0;
	CURLcode ret = curl_easy_setopt(m_pHandle, CURLOPT_VERBOSE, lonoff);
	return ret == CURLE_OK ? true : false;
}

bool CurlHttpClient::appendHeader(const std::string & headerStr)
{
	m_headers = curl_slist_append(m_headers, headerStr.c_str());
	return m_headers != nullptr ? true : false;
}

bool CurlHttpClient::resetHeader()
{
	if (m_headers)
	{
		curl_slist_free_all(m_headers);
		m_headers = nullptr;
	}
	return true;
}

bool CurlHttpClient::setCert(const std::string & certPath)
{
	CURLcode ret = CURLE_OK;
	if (certPath.empty())
	{
		//����֤��У��
		ret = curl_easy_setopt(m_pHandle, CURLOPT_SSL_VERIFYPEER, 0);
		ret = curl_easy_setopt(m_pHandle, CURLOPT_SSL_VERIFYHOST, 0);
	}
	else
	{
		// ��֤������֤����Ч��
		ret = curl_easy_setopt(m_pHandle, CURLOPT_SSL_VERIFYPEER, 1);
		// ����֤���е�������������ʵ�������һ��
		ret = curl_easy_setopt(m_pHandle, CURLOPT_SSL_VERIFYHOST, 2);
		// ָ�� CA ֤��·��
		ret = curl_easy_setopt(m_pHandle, CURLOPT_CAINFO, certPath.c_str());
	}

	return ret == CURLE_OK ? true : false;
}

bool CurlHttpClient::get(const std::string & url)
{
	CURLcode ret = CURLE_OK;
	//���ñ�ͷ
	ret = curl_easy_setopt(m_pHandle, CURLOPT_HTTPHEADER, m_headers);
	//���ûص�
	setResData();
	//����url
	ret = curl_easy_setopt(m_pHandle, CURLOPT_URL, url.c_str());
	//���÷���
	ret = curl_easy_setopt(m_pHandle, CURLOPT_HTTPGET, 1);
	//ִ��
	ret = curl_easy_perform(m_pHandle);
	return ret == CURLE_OK ? true : false;
}

bool CurlHttpClient::post(const std::string & url, const std::string & postData)
{
	CURLcode ret = CURLE_OK;
	//���ñ�ͷ
	ret = curl_easy_setopt(m_pHandle, CURLOPT_HTTPHEADER, m_headers);
	//���ûص�
	setResData();
	//����url
	ret = curl_easy_setopt(m_pHandle, CURLOPT_URL, url.c_str());
	//���÷���
	ret = curl_easy_setopt(m_pHandle, CURLOPT_POST, 1);
	//
	ret = curl_easy_setopt(m_pHandle, CURLOPT_POSTFIELDS, postData.c_str());
	ret = curl_easy_setopt(m_pHandle, CURLOPT_POSTFIELDSIZE_LARGE,(curl_off_t)postData.length());
	//ִ��
	ret = curl_easy_perform(m_pHandle);
	return ret == CURLE_OK ? true : false;
}



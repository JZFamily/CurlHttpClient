//////////////////////////////////////////////////////////////////////////////////
///
///
///
///
///
/////////////////////////////////////////////////////////////////////////////////
#ifndef __UTILITY_H_
#define __UTILITY_H_

//-------------------------------------------------------------------------------
#include <string>
#include <iostream>
#include "boost/filesystem.hpp"
#include "json.hpp"
#include <fstream>
#include <sstream>
#include "boost/format.hpp"
#if WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif // WIN32
//-------------------------------------------------------------------------------

class UrlObject
{
public:
	UrlObject()
	{

	}
	UrlObject(const UrlObject&) = default;
	UrlObject(const std::string& protcol, const std::string& m_domain)
		:m_protcol(protcol)
		, m_domain(m_domain)
	{
		m_urlPath += m_protcol;
		m_urlPath += "://";
		m_urlPath += m_domain;
	}
	UrlObject(const std::string& protcol, const std::string& m_domain, const std::string& url)
		:m_protcol(protcol)
		, m_domain(m_domain)
		, m_url(url)
	{
		m_urlPath += m_protcol;
		m_urlPath += "://";
		m_urlPath += m_domain;
		m_urlPath += m_url;
	}
	~UrlObject() {}
	operator const std::string()
	{
		return m_urlPath;
	}
	void setUrl(const std::string& url)
	{
		m_urlPath.clear();
		m_urlPath += m_protcol;
		m_urlPath += "://";
		m_urlPath += m_domain;
		m_url = url;
		m_urlPath += url;
	}
	bool appendUrlParam(const std::pair<std::string, std::string>& paramPair, const char* splitStr = "&")
	{
		m_urlPath += splitStr;
		m_urlPath += paramPair.first;
		m_urlPath += "=";
		m_urlPath += paramPair.second;
		return true;
	}
private:
	std::string m_protcol;
	std::string m_domain;
	std::string m_url;
	std::string m_urlPath;
};
bool execCmd(const std::string& cmd, std::string retStr);
bool getExePath(std::string& exePath);
std::string getCurrentTimeStamp();
std::string getExecPath();
#endif // __UTILITY_H_

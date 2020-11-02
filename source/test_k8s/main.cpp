#include "i_curl_http_client.h"
#include <iostream>
#include <string>
#include "boost/filesystem.hpp"
#include "json.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include "boost/format.hpp"
#if WIN32
#include <Windows.h>
#else // linux
#include <unistd.h>
#endif // WIN32

bool getExePath(std::string& exePath)
{
	const size_t  MAX_PATH_LENGTH = 260;
	char bufferFileName[MAX_PATH_LENGTH] = {0};
#if WIN32
	
	DWORD ret = GetModuleFileNameA(nullptr, bufferFileName, MAX_PATH_LENGTH);
	if (ret <= 0 || ret >= MAX_PATH_LENGTH)
	{
		return false;
	}
#else
	ssize_t ret = readlink("/proc/self/exe", bufferFileName, MAX_PATH_LENGTH);
	if (ret <= 0 || ret >= MAX_PATH_LENGTH)
	{
		return false;
	}
#endif // WIN32
	exePath = bufferFileName;
	std::cerr << exePath << std::endl;
	return true;
}
//std::string&& getCurrentTimeStamp()
//{
//	auto tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
//	time_t tt = std::chrono::system_clock::to_time_t(tp);
//	struct tm * tm = localtime(&tt);
//	std::stringstream ss;
//	ss << std::setiosflags(std::ios::right) << std::setw(4) << tm->tm_year + 1900 << "-" 
//		<< std::setfill('0') << std::setw(2)  << tm->tm_mon+1 << "-" << tm->tm_mday << " "
//		<< std::setw(2) << tm->tm_hour <<":" << std::setw(2) << tm->tm_min << ":"<< std::setw(2) << tm->tm_sec
//		<< "." << tp.time_since_epoch().count()%1000;
//	std::cerr << ss.str()  << std::endl;
//	return ss.str();
//}

//返回的时间戳无法当作文件名无论是2020-11-01 23:36:12.111 还是2020-11-01-23:36:12.111
std::string getCurrentTimeStamp()
{
	using format = boost::format;
	auto tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	time_t tt = std::chrono::system_clock::to_time_t(tp);
	struct tm * tm = localtime(&tt);
	format ft("%1$04d-%2$02d-%3$02d %4$02d:%5$02d:%6$02d.%7$03d");
	std::cerr << boost::str(ft 
		% (tm->tm_year + 1900) 
		% (tm->tm_mon + 1) 
		% tm->tm_mday 
		% tm->tm_hour 
		% tm->tm_min 
		% tm->tm_sec 
		% (tp.time_since_epoch().count() % 1000)) << std::endl;
	return std::move(boost::str(ft
		% (tm->tm_year + 1900)
		% (tm->tm_mon + 1)
		% tm->tm_mday
		% tm->tm_hour
		% tm->tm_min
		% tm->tm_sec
		% (tp.time_since_epoch().count() % 1000)));
}
int main()
{
	getCurrentTimeStamp();
	std::string exePath;
	if (!getExePath(exePath))
	{
		return 0;
	}
	using Path = boost::filesystem::path;
	Path  path(exePath);
	std::cerr << path.parent_path().string<std::string>() << std::endl;
	std::string execDirPath = path.parent_path().string<std::string>();
	using  json = nlohmann::json;

	const char *configPath = "/config/run.json";
	std::string configfullPath = path.parent_path().string<std::string>() + configPath;
	std::cerr << configfullPath << std::endl;
	json configData;
	try
	{
		std::fstream f;
		f.exceptions(std::ios_base::failbit | std::ios_base::badbit);
		f.open(configfullPath);
		configData << f;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 0;
	}

	ICurlHttpClient *pICurlHttpClient = createCurlHttpClient();
	//���õ���ģʽ
	pICurlHttpClient->setVerbose(true);


	//�����Զ���ͷ
	std::string url = "https://";
	std::string doMain = configData["domain"];
	url += doMain;

	std::string token = "Authorization: Bearer ";
	token += std::string(configData["token"]);
	pICurlHttpClient->appendHeader(token);


	//����cert·�����������֤����֤
	pICurlHttpClient->setCert("");
	
	//get node list
	//{
	//	std::string tmpUrl = url;
	//	std::string path = "/api/v1/nodes";
	//	tmpUrl += path;

	//	//get
	//	if (pICurlHttpClient->get(tmpUrl))
	//	{
	//		std::cout << pICurlHttpClient->getResponse() << std::endl;
	//	}
	//}

	//delete node 
	//{
	//	std::string tmpUrl = url;
	//	std::string path = "/api/v1/nodes/work1.node";
	//	tmpUrl += path;

	//	//delete
	//	if (pICurlHttpClient->delete_(tmpUrl))
	//	{
	//		std::cout << pICurlHttpClient->getResponse() << std::endl;
	//	}
	//}
	
	//GET  /api/v1/events
	{
		std::string tmpUrl = url;
		std::string path = "/api/v1/events";
		tmpUrl += path;

		//delete
		if (pICurlHttpClient->get(tmpUrl))
		{
			std::cout << pICurlHttpClient->getResponse() << std::endl;
			std::string filename = getCurrentTimeStamp();
			try
			{
				std::stringstream ss;
				ss.str(pICurlHttpClient->getResponse());
				json data;
				data << ss;
				std::fstream f;
				std::string fullpath = execDirPath;
				fullpath += "/";
				fullpath += "config/";
				fullpath += "hhehe";
				fullpath += ".json";
				f.exceptions(std::ios_base::failbit | std::ios_base::badbit);
				f.open(fullpath, std::ios_base::out);
				f << data;
				
			}
			catch (const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
				return 0;
			}

		}
	}
		//GET /api/v1/watch/events
	{
		std::string tmpUrl = url;
		std::string path = "/api/v1/watch/events";
		tmpUrl += path;

		//delete
		if (pICurlHttpClient->get(tmpUrl))
		{
			std::cout << pICurlHttpClient->getResponse() << std::endl;
			std::string filename = getCurrentTimeStamp();
			try
			{
				std::stringstream ss;
				ss.str(pICurlHttpClient->getResponse());
				json data;
				data << ss;
				std::fstream f;
				std::string fullpath = execDirPath;
				fullpath += "/";
				fullpath += "config/";
				fullpath += "hhehe_watch";
				fullpath += ".json";
				f.exceptions(std::ios_base::failbit | std::ios_base::badbit);
				f.open(fullpath, std::ios_base::out);
				f << data;
				
			}
			catch (const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
				return 0;
			}

		}
	}
	releaseCurlHttpClient(pICurlHttpClient);
}
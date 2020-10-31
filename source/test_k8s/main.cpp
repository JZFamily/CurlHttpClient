#include "i_curl_http_client.h"
#include <iostream>
#include <string>
#include "boost/filesystem.hpp"
#include "json.hpp"
#include <fstream>
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
int main()
{
	std::string exePath;
	if (!getExePath(exePath))
	{
		return 0;
	}
	using Path = boost::filesystem::path;
	Path  path(exePath);
	std::cerr << path.parent_path().string<std::string>() << std::endl;
	using  json = nlohmann::json;

	const char *configPath = "/config/run.json";
	std::string configfullPath = path.parent_path().string<std::string>() + configPath;
	std::cerr << configfullPath << std::endl;
	json configData;
	try
	{
		std::fstream f(configfullPath);
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
	pICurlHttpClient->appendHeader("");
	//����cert·�����������֤����֤
	pICurlHttpClient->setCert("");
	
	//get
	if (pICurlHttpClient->get("https://github.com/TheDenominationForShow/WeatherSpider"))
	{
		std::cout << pICurlHttpClient->getResponse() << std::endl;
	}
	releaseCurlHttpClient(pICurlHttpClient);
}
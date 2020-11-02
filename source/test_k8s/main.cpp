#include "i_curl_http_client.h"
#include <iostream>
#if WIN32
#include <direct.h>
#include <windows.h>

#else
#include <unistd.h>
#endif // WIN32
bool getExePath(std::string& workDir)
{
	char buffer[260];
#if WIN32
	DWORD ret = GetModuleFileNameA(NULL, buffer, 260);
	if (ret == 0 || ret == 260)
	{
		return false;
	}
#else

	int result = readlink("/proc/self/exe", buffer, 260);

	if (result < 0 || result >= 260)
	{
		return false;
	}
#endif
	std::cerr << buffer << std::endl;
	workDir = buffer;
	return true;
}

bool getCurrentWorkDir(std::string& workDir)
{
	char buffer[260];
	if (getcwd(buffer, 260) == nullptr)
	{
		return false;
	}
	workDir = buffer;
	return true;
}
int main()
{
	std::string workDir;
	if (!getExePath(workDir))
	{
		return 0;
	}
	std::cerr << workDir << std::endl;
	ICurlHttpClient *pICurlHttpClient = createCurlHttpClient();
	//设置调试模式
	pICurlHttpClient->setVerbose(true);
	//设置自定义头
	pICurlHttpClient->appendHeader("");
	//设置cert路径，空则忽略证据验证
	pICurlHttpClient->setCert("");
	
	//get
	if (pICurlHttpClient->get("https://github.com/TheDenominationForShow/WeatherSpider"))
	{
		std::cout << pICurlHttpClient->getResponse() << std::endl;
	}
	releaseCurlHttpClient(pICurlHttpClient);
}
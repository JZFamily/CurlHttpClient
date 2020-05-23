#include "i_curl_http_client.h"
#include <iostream>

int main()
{
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
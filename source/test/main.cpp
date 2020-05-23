#include "i_curl_http_client.h"
#include <iostream>

int main()
{
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
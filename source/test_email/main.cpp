#include "i_curl_http_client.h"
#include <iostream>
#include "curl/curl.h"


//POP3服务器: pop.126.com
//	SMTP服务器 : smtp.126.com
//	IMAP服务器 : imap.126.com
#include <stdio.h>
#include <curl/curl.h>

/* This is a simple example showing how to obtain information about a mailbox
 * folder using libcurl's IMAP capabilities via the EXAMINE command.
 *
 * Note that this example requires libcurl 7.30.0 or above.
 */

int main(void)
{
	CURL *curl;
	CURLcode res = CURLE_OK;

	curl = curl_easy_init();

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		/* Set username and password */
		curl_easy_setopt(curl, CURLOPT_USERNAME, "");
		curl_easy_setopt(curl, CURLOPT_PASSWORD, "");

		/* This is just the server URL */
		//curl_easy_setopt(curl, CURLOPT_URL, "imap://imap.126.com");
		//5446
		curl_easy_setopt(curl, CURLOPT_URL,"imap://imap.126.com/INBOX");
		//忽略证书校验
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "COPY 3 hello");
		//curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "SEARCH RECENT");
		//curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "NOOP");
		/* Perform the list */
		res = curl_easy_perform(curl);

		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));

		/* Always cleanup */
		curl_easy_cleanup(curl);
	}

	return (int)res;
}
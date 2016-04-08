#include "HttpRequest.h"
#include <curl/curl.h>
static int writer(char *data, size_t size, size_t nmemb, std::string *writerData)
{
    unsigned long sizes = size * nmemb;
    if (writerData == NULL) return 0;
    writerData->append(data, sizes);
    return sizes;
}


HttpRequest::HttpRequest():curl(NULL){}

HttpRequest::~HttpRequest()
{
    if(curl)
    {

        curl_easy_cleanup(curl);
        curl = NULL;
    }

}

int HttpRequest::init()
{
    int ret = 0;
    curl = curl_easy_init();
    if(NULL == curl)
    {
        fprintf(stderr,"%s\n","curl init error");
        ret = -1;
    }
    return ret;
}

int HttpRequest::getConnect(const std::string& url,std::string* buffer)
{
    CURLcode code;
    int ret = 0;
    if(NULL == curl)
    {
        ret = -1;
        return ret;
    }

    code = curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
    if(code!=CURLE_OK)
    {
        fprintf(stderr,"%s\n","fail to set url");
        ret = 1;
        return ret;
    }
    code = curl_easy_setopt(curl,CURLOPT_WRITEDATA,buffer);
      if(code!=CURLE_OK)
    {
        fprintf(stderr,"%s\n","fail to set write buffer");
        ret = 2;
        return ret;
    }

    code = curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,writer);
       if(code!=CURLE_OK)
    {
        fprintf(stderr,"%s\n","fail to set write function");
        ret = 3;
        return ret;
    }

    curl_easy_setopt(curl,CURLOPT_COOKIEJAR,"/tmp/1.cookie");
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE,"/tmp/1.cookie");

    code = curl_easy_perform(curl);
    if(code!=CURLE_OK)
    {
        fprintf(stderr,"%s\n","not connect");
    }
        return ret;

}

int HttpRequest::close()
{
    int ret = 0;
    if(NULL == curl)
    {
        ret = -1;
        fprintf(stderr,"%s\n","not init");
        return ret;
    }

    curl_easy_cleanup(curl);
    curl = NULL;
    return -1;
}

int HttpRequest::postConnect(const std::string& url,const std::string& postData,std::string* buffer)
{

    int ret = 0;
    CURLcode code;
    if(NULL == curl)
    {
        ret = -1;
        fprintf(stderr,"not init\n");
        return ret;
    }
   // printf("test\n");
    curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
    curl_easy_setopt(curl,CURLOPT_POSTFIELDS,postData.c_str());
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,buffer);
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,writer);
    curl_easy_setopt(curl,CURLOPT_POST,1);
    curl_easy_setopt(curl,CURLOPT_HEADER,1);
    curl_easy_setopt(curl,CURLOPT_COOKIEJAR,"/tmp/1.cookie");
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE,"/tmp/1.cookie");
    curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1);
    code = curl_easy_perform(curl);


      if (code != CURLE_OK)
    {
        switch(code)
        {
            case CURLE_UNSUPPORTED_PROTOCOL:
                fprintf(stderr,"不支持的协议,由URL的头部指定\n");
            case CURLE_COULDNT_CONNECT:
                fprintf(stderr,"不能连接到remote主机或者代理\n");
            case CURLE_HTTP_RETURNED_ERROR:
                fprintf(stderr,"http返回错误\n");
            case CURLE_READ_ERROR:
                fprintf(stderr,"读本地文件错误\n");
            default:
                fprintf(stderr,"返回值:%d\n",code);
        }
        return -1;
    }

    return ret;
}


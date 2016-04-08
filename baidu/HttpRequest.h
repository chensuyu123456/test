#ifndef HTTPREQUEST_H_INCLUDED
#define HTTPREQUEST_H_INCLUDED
#include <curl/curl.h>
#include <string>

class HttpRequest{
public:
    HttpRequest();
    ~HttpRequest();
    int init();
    int getConnect(const std::string&,std::string*);
    int close();
    int postConnect(const std::string&,const std::string&,std::string*);
private:
    CURL* curl;
};

extern bool isLogin;

#endif // HTTPREQUEST_H_INCLUDED

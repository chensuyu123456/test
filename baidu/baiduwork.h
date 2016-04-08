#ifndef BAIDUWORK_H_INCLUDED
#define BAIDUWORK_H_INCLUDED
#include <string>
#include <iostream>
#include <assert.h>
#define TOKEN_URL "https://passport.baidu.com/v2/api/?getapi&class=login&tpl=mn&tangram=true"
#define LOGIN_URL "https://passport.baidu.com/v2/api/?login"
#define TIEBA_URL "http://tieba.baidu.com/p/"
#define TIEBA_RE_URL "http://tieba.baidu.com/f/commit/post/add"




int login(const std::string&,const std::string&);
int reback(const std::string&,const std::string&,const std::string&);
unsigned char ToHex(unsigned char x);
unsigned char FromHex(unsigned char x);
std::string UrlEncode(const std::string& str);
std::string UrlDecode(const std::string& str);
int rebackfoor(const std::string&,const std::string&,const std::string&,int);
extern bool isLogin;
#endif // BAIDUWORK_H_INCLUDED

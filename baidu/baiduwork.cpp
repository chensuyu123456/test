#include "baiduwork.h"
#include "HttpRequest.h"

unsigned char ToHex(unsigned char x)
{
    return  x > 9 ? x + 55 : x + 48;
}

unsigned char FromHex(unsigned char x)
{
    unsigned char y;
    if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
    else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
    else if (x >= '0' && x <= '9') y = x - '0';
    else assert(0);
    return y;
}

std::string UrlEncode(const std::string& str)
{
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (isalnum((unsigned char)str[i]) ||
            (str[i] == '-') ||
            (str[i] == '_') ||
            (str[i] == '.') ||
            (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else
        {
            strTemp += '%';
            strTemp += ToHex((unsigned char)str[i] >> 4);
            strTemp += ToHex((unsigned char)str[i] % 16);
        }
    }
    return strTemp;
}

std::string UrlDecode(const std::string& str)
{
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (str[i] == '+') strTemp += ' ';
        else if (str[i] == '%')
        {
            assert(i + 2 < length);
            unsigned char high = FromHex((unsigned char)str[++i]);
            unsigned char low = FromHex((unsigned char)str[++i]);
            strTemp += high*16 + low;
        }
        else strTemp += str[i];
    }
    return strTemp;
}






bool isLogin = false;
int getIndex(const std::string,const std::string);

int login(const std::string& name,const std::string& password)
{
    int ret = 0;
    HttpRequest request;
    ret = request.init();
    if(ret)
        return ret;
    std::string buf;
    request.getConnect("http://www.baidu.com/cache/user/html/login-1.2.html",&buf);
    std::string buf1;
    request.getConnect(TOKEN_URL,&buf1);
    std::cout<<buf1<<std::endl;
    int startIndex = getIndex(buf1,"token");
    std::cout<<startIndex<<std::endl;
    // int endIndex = getIndex()
    std::string token = buf1.substr(startIndex+7,32);
    //std::cout<<token<<std::endl;
    //ppui_logintime=36261&charset=utf-8&codestring=&token=f94a6fe7cc10f143a0a5c8105b1db45d&isPhone=true&index=1&u=&safeflg=0&staticpage=http%3A%2F%2Fwww.baidu.com%2Fcache%2Fuser%2Fhtml%2Fjump.html&loginType=1&tpl=mn&callback=parent.bdPass.api.login._postCallback&username=18360471417&password=a68622417&verifycode=&mem_pass=on

    //
    std::string postData = "ppui_logintime=36261&charset=utf-8&codestring=&token="+token+"&isPhone=true&index=1&u=&safeflg=0&staticpage=http%3A%2F%2Fwww.baidu.com%2Fcache%2Fuser%2Fhtml%2Fjump.html&loginType=1&tpl=mn&callback=parent.bdPass.api.login._postCallback&username="+UrlEncode(name)+"&password="+password+"&verifycode=&mem_pass=on";
    std::cout<<postData<<std::endl;
    std::string buf3;
    //ret = request.getConnect()
    request.postConnect(LOGIN_URL,postData,&buf3);
    std::cout<<buf3<<std::endl;
    isLogin = true ;

    return ret;
}

int getIndex(std::string content,std::string sub)
{
    return content.find(sub);
}


int reback(const std::string& content,const std::string& name,const std::string& id)
{
    int ret = 0;
    if(isLogin)
    {
        std::string url = TIEBA_URL+id;
        std::cout<<url<<std::endl;
        std::cout<<"----------------------------------------------------------------------------------------------------"<<std::endl;
        HttpRequest request;
        std::string buffer;
        request.init();
        request.getConnect(url,&buffer);
        int index = buffer.find("fid");

        std::string fid = buffer.substr(index+4,6);
        index = buffer.find("tbs\":");
        std::string tbs = buffer.substr(index+7,26);
       // std::cout<<tbs<<std::endl;
        //ie=utf-8&kw=toolext&fid=289621&tid=3769209083&vcode_md5=&floor_num=1&rich_text=1&tbs=21ac148eeaddeae61459996986&content=123456&files=%5B%5D&mouse_pwd=103%2C96%2C107%2C126%2C99%2C100%2C98%2C96%2C124%2C97%2C102%2C107%2C107%2C101%2C103%2C103%2C106%2C103%2C107%2C97%2C101%2C103%2C91%2C99%2C126%2C98%2C126%2C99%2C126%2C98%2C126%2C99%2C126%2C98%2C126%2C99%2C126%2C98%2C126%2C99%2C126%2C98%2C91%2C102%2C99%2C99%2C101%2C103%2C91%2C99%2C107%2C96%2C98%2C126%2C99%2C98%2C106%2C98%2C14599968837820&mouse_pwd_t=1459996883782&mouse_pwd_isclick=0&__type__=reply
        //std::cout<<buffer<<std::endl;
        //std::cout<<fid<<std::endl;
        std::string postData = "ie=utf-8&kw="+UrlEncode(name)+"&fid="+fid+"&tid="+id+"&vcode_md5=&floor_num=1&rich_text=1&tbs="+tbs+"&content="+UrlEncode(content)+"&files=%5B%5D&mouse_pwd=103%2C96%2C107%2C126%2C99%2C100%2C98%2C96%2C124%2C97%2C102%2C107%2C107%2C101%2C103%2C103%2C106%2C103%2C107%2C97%2C101%2C103%2C91%2C99%2C126%2C98%2C126%2C99%2C126%2C98%2C126%2C99%2C126%2C98%2C126%2C99%2C126%2C98%2C126%2C99%2C126%2C98%2C91%2C102%2C99%2C99%2C101%2C103%2C91%2C99%2C107%2C96%2C98%2C126%2C99%2C98%2C106%2C98%2C14599968837820&mouse_pwd_t=1459996883782&mouse_pwd_isclick=0&__type__=reply";
        //std::cout<<postData<<std::endl;
        std::string addBuffer;
        request.postConnect(TIEBA_RE_URL,postData,&addBuffer);
        std::cout<<addBuffer<<std::endl;
    }
    else
        return -1;

    return ret;
}


int rebackfoor(const std::string& content,const std::string& name,const std::string& id,int floor)
{
    int ret = 0;
    if(isLogin)
    {
        std::string url = TIEBA_URL+id;
        std::cout<<url<<std::endl;
        std::cout<<"----------------------------------------------------------------------------------------------------"<<std::endl;
        HttpRequest request;
        std::string buffer;
        request.init();
        request.getConnect(url,&buffer);
        int index = buffer.find("fid");

        std::string fid = buffer.substr(index+4,6);
        index = buffer.find("tbs\":");
        std::string tbs = buffer.substr(index+7,26);
        std::string quote_id;
        if(floor==1)
        {
            index = buffer.find("\"post_content");
            quote_id = buffer.substr(index+14,11);

        }else{
            char buf[2]={0};
            buf[0] ='0' + (floor-1);
            std::string findFloor(buf);
            findFloor.append("楼");
            std::cout<<findFloor<<std::endl;
            index = buffer.find(findFloor);
            index = buffer.find("\"post_content",index);
            quote_id = buffer.substr(index+14,11);
        }
        std::string reBuffer;
        std::string postData = "ie=utf-8&kw=toolext&fid="+fid+"&tid=3769209083&floor_num=2&quote_id="+quote_id+"&rich_text=1&tbs="+tbs+"&content="+content+"&lp_type=0&lp_sub_type=0&new_vcode=1&tag=11&repostid="+quote_id+"&anonymous=0";
        request.postConnect(TIEBA_RE_URL,postData,&reBuffer);

       // index =   buffer.rfind("post_content",index);
       // std::string quote_id = buffer.substr(index+12,11);
        //std::cout<<quote_id<<std::endl;
       // std::cout<<tbs<<std::endl;
        //ie=utf-8&kw=toolext&fid=289621&tid=3769209083&vcode_md5=&floor_num=1&rich_text=1&tbs=21ac148eeaddeae61459996986&content=123456&files=%5B%5D&mouse_pwd=103%2C96%2C107%2C126%2C99%2C100%2C98%2C96%2C124%2C97%2C102%2C107%2C107%2C101%2C103%2C103%2C106%2C103%2C107%2C97%2C101%2C103%2C91%2C99%2C126%2C98%2C126%2C99%2C126%2C98%2C126%2C99%2C126%2C98%2C126%2C99%2C126%2C98%2C126%2C99%2C126%2C98%2C91%2C102%2C99%2C99%2C101%2C103%2C91%2C99%2C107%2C96%2C98%2C126%2C99%2C98%2C106%2C98%2C14599968837820&mouse_pwd_t=1459996883782&mouse_pwd_isclick=0&__type__=reply
        //std::cout<<buffer<<std::endl;
        //std::cout<<fid<<std::endl;
        //std::string postData = "ie=utf-8&kw="+UrlEncode(name)+"&fid="+fid+"&tid="+id+"&vcode_md5=&floor_num=1&rich_text=1&tbs="+tbs+"&content="+UrlEncode(content)+"&files=%5B%5D&mouse_pwd=103%2C96%2C107%2C126%2C99%2C100%2C98%2C96%2C124%2C97%2C102%2C107%2C107%2C101%2C103%2C103%2C106%2C103%2C107%2C97%2C101%2C103%2C91%2C99%2C126%2C98%2C126%2C99%2C126%2C98%2C126%2C99%2C126%2C98%2C126%2C99%2C126%2C98%2C126%2C99%2C126%2C98%2C91%2C102%2C99%2C99%2C101%2C103%2C91%2C99%2C107%2C96%2C98%2C126%2C99%2C98%2C106%2C98%2C14599968837820&mouse_pwd_t=1459996883782&mouse_pwd_isclick=0&__type__=reply";
        //std::cout<<postData<<std::endl;
        //std::string addBuffer;
//        request.postConnect(TIEBA_RE_URL,postData,&addBuffer);
        //std::cout<<addBuffer<<std::endl;
    }
    else
        return -1;

    return ret;


}

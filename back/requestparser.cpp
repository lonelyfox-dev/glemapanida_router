#include "requestparser.h"


std::string requestparser::getHandler(std::string request)
{
    std::istringstream iss(request);
    std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
    std::string method=parsed[0];
    if(method=="GET")
    {
        std::string str=parsed[1];
        size_t start = str.find('/');
        size_t end = str.find('?');
        std::string handler = str.substr(start + 1, end - start - 1);
        return handler;
    }
    if(method=="POST")
    {
        return "";
    }
    return "";
}

std::map<std::string, std::string> requestparser::getParams(std::string request)
{
    std::istringstream iss(request);
    std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
    std::string method=parsed[0];
    std::map<std::string,std::string> params;
    if(method=="GET")
    {
        std::string str=parsed[1];
        int start_ind=str.find('?');

        if(start_ind== -1) return params;
        if(str.find('=')== -1)return params;

        str.erase(0,start_ind+1);
        for(;str!="";)
        {
            int eql=str.find('=');
            std::string key = str.substr(0, eql);
            str.erase(0,eql+1);
            std::string val;
            int amp=str.find('&');
            if(amp!=-1)
            {
                val=str.substr(0, amp);
                str.erase(0,amp+1);
            }
            else
            {
                val=str;
                str="";
            }
            QByteArray byteArray = QByteArray::fromPercentEncoding(QString::fromStdString(val).toUtf8());
            QString decodedString = QString::fromUtf8(byteArray);

            params[key]=decodedString.toStdString();

        }
    }
    return params;
}

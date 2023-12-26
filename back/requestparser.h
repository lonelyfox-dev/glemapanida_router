#ifndef REQUESTPARSER_H
#define REQUESTPARSER_H
#include <iostream>
#include <map>
#include <vector>
#include <iterator>
#include <string>
#include <istream>
#include <sstream>
#include <QString>

class requestparser
{
public:
    static std::string getHandler(std::string request);
    static std::map<std::string,std::string> getParams(std::string request);
private:
    requestparser();
};

#endif // REQUESTPARSER_H

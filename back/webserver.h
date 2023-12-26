#ifndef WEBSERVER_H
#define WEBSERVER_H
#include "tcplistener.h"
#include <string>
#include <istream>
#include <sstream>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <vector>
#include <iterator>
#include <QDebug>
#include <QSqlDatabase>
#include<QSqlQuery>
#include<QJsonArray>
#include<QJsonDocument>
#include <QJsonObject>
#include "dbcontroller.h"
#include "config.h"
#include "api.h"
class WebServer : public TcpListener
{
public:

    WebServer(const char* ipAddress, int port) :
        TcpListener(ipAddress, port) { }
    void connectToDB();
private:
    dbcontroller *dbCtrl;
    std::string formResponse(api::Header header);
protected:

    // Handler for client connections
    virtual void onClientConnected(int clientSocket);

    // Handler for client disconnections
    virtual void onClientDisconnected(int clientSocket);

    // Handler for when a message is received from the client
    virtual void onMessageReceived(int clientSocket, const char* msg, int length);
};
#endif // WEBSERVER_H

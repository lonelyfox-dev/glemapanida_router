#include "webserver.h"

void WebServer::onMessageReceived(int clientSocket, const char* msg, int length)
{
    auto response=api::requestProcessing(msg,dbCtrl);
    std::string output=formResponse(response);
    sendToClient(clientSocket, output.c_str(), output.size()+1);
}


void WebServer::connectToDB()
{
    config::params par=config::instance().getParams();
    dbCtrl = new dbcontroller(par.ip,par.port,par.dbname,par.user,par.password,par.schema);
}

std::string WebServer::formResponse(api::Header header)
{
    std::ostringstream oss;
    oss << "HTTP/1.1 " << header.errorCode << " "+header.status<<"\r\n";
    oss<<"Access-Control-Allow-Origin: *\r\n";
    oss << "Cache-Control: no-cache, private\r\n";
    oss << "Content-Type: application/json\r\n";
    oss << "Content-Length: " << header.contentLength << "\r\n";
    oss << "\r\n";
    oss << header.response;
    std::string output = oss.str();
    return output;
}

// Handler for client connections
void WebServer::onClientConnected(int clientSocket)
{

}

// Handler for client disconnections
void WebServer::onClientDisconnected(int clientSocket)
{

}

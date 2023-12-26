#ifndef TCPLISTENER_H
#define TCPLISTENER_H
#include<iostream>
#include<ws2tcpip.h>
#include<string>
#include<sstream>

class TcpListener
{
public:
    TcpListener(const char* ipAddress,int port) :
        m_ipAddress(ipAddress), m_port(port){};
    int init();
    int run();
protected:
    virtual void onClientConnected(int clientSocket);
    virtual void onClientDisconnected(int clientSocket);
    // Handler for when a message is received from the client
    virtual void onMessageReceived(int clientSocket, const char* msg, int length);
    void sendToClient(int clientSocket,const char* msg, int length);
private:
    const char*     m_ipAddress;//адрес сервера
    int             m_port;
    int             m_socket;
    //master FD set
    fd_set          m_master; /*служит для отслеживания состояния нескольких клиентских соединений одновременно*/
};

#endif // TCPLISTENER_H

#include "tcpserver.h"

TCPServer::TCPServer(QObject *parent) : QTcpServer(parent)
{
    listen(QHostAddress::Any);
}

TCPServer::~TCPServer()
{

}

void TCPServer::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    ClientSession *client = new ClientSession(socket, this);
    emit newConnection(client);
}

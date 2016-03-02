#include "tcpserver.h"
#include "src/utils/logger.h"

TCPServer::TCPServer(QObject *parent) : QTcpServer(parent)
{
    LOG_INFO("Démarrage du serveur TCP...");
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
    emit sig_newConnection(client);
}

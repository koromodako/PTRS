#include "udpserver.h"
#include "../const.h"

#include <QNetworkInterface>

static const unsigned broadcastPort = 45000;

UDPServer::UDPServer(quint16 tcpServerPort, QObject *parent) : QObject(parent)
{
    _tcpServerPort = tcpServerPort;
    _broadcastSocket.bind(QHostAddress::Any, broadcastPort, QUdpSocket::ShareAddress
                         | QUdpSocket::ReuseAddressHint);

    connect(&_broadcastSocket, &QUdpSocket::readyRead, this, &UDPServer::readBroadcastDatagram);
}

UDPServer::~UDPServer()
{
}

void UDPServer::readBroadcastDatagram()
{
    while (_broadcastSocket.hasPendingDatagrams())
    {
        QHostAddress senderIp;
        quint16 senderPort;
        QByteArray d;
        d.resize(_broadcastSocket.pendingDatagramSize());
        if (_broadcastSocket.readDatagram(d.data(), d.size(), &senderIp, &senderPort) == -1)
            continue;

        QList<QString> datagrams = QString(d).split("#@@#", QString::SkipEmptyParts);
        foreach (QString datagram, datagrams)
        {
            if (datagram.split("##").first().toInt() != HELLO)
                continue;

            //On envoie l'ip et le port du server au client qui l'a demandé
            QByteArray rep = QByteArray::number(HELLO_FROM_SERVER) + "##";
            rep += QByteArray::number(_tcpServerPort) + "#@@#";

            foreach (QNetworkInterface interface, QNetworkInterface::allInterfaces())
            {
                foreach (QNetworkAddressEntry entry, interface.addressEntries())
                {
                    if (entry.broadcast() != QHostAddress::Null && entry.ip() != QHostAddress::LocalHost)
                        _broadcastSocket.writeDatagram(rep, entry.broadcast(), broadcastPort);
                }
            }
        }
    }
}

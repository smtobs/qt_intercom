#include "image_reciver.hpp"
#include <QDebug>

ImageReciver::ImageReciver(QObject *parent)
    : QObject(parent)
{
    socket = new QUdpSocket(this);
    if (!socket->bind(QHostAddress::Any, 18080))
    {
        qDebug() << "Failed to bind the socket to port 18080";
        return;
    }
    connect(socket, &QUdpSocket::readyRead, this, &ImageReciver::ReadPendingDatagrams);

    qDebug() << "Server started.";
}

void ImageReciver::ReadPendingDatagrams()
{
    while (socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        if ((this->SaveImage(datagram, "/tmp/image.jpg")) == true)
        {
            emit this->GenerateImage(3);
            qDebug() << "File Save Success : " << this->image_path;
        }
    }
}

bool ImageReciver::SaveImage(QByteArray& datagram, const QString& image_path)
{
    QFile file(image_path);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(datagram);
        file.close();
        return true;
    }
    else
    {
       return false;
    }
}

ImageReciver::~ImageReciver()
{
    if (socket)
    {
        delete socket;
    }
}

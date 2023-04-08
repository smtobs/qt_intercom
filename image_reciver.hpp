#ifndef IMAGERECIVER_H
#define IMAGERECIVER_H

#include <QtNetwork>
#include <QUdpSocket>
#include <QObject>


class ImageReciver : public QObject
{
    Q_OBJECT
public:
    explicit ImageReciver( QObject* parent = nullptr);
    ~ImageReciver();

signals:
    void GenerateWebCamImage(const QString& image_path);

private slots:
    void ReadPendingDatagrams();

private:
    QUdpSocket *socket;
    const QString image_path = "/tmp/image.jpg";
    bool SaveImage(QByteArray& datagram, const QString& image_path);
};

#endif // IMAGERECIVER_H

#ifndef WEATHER_H
#define WEATHER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QPixmap>


class Weather : public QObject
{
    Q_OBJECT

public:
    explicit Weather(QObject *parent = nullptr);
    void UpdateWeather(const double latitude, const double longitude);

private:
    QNetworkAccessManager *m_manager;
    QString m_apiKey;

    void IconDownloadFinished(const QPixmap& icon, const QString& icon_path);

signals:
    void WeatherUpdated(const QString& description, const QString& icon_path, const double temperature, const double humidity);

private slots:
    void ParseJson(const QByteArray& json);
    void DownloadIcon(const QString& iconCode, const QString &icon_path);

};

#endif // WEATHER_H

#include "weather.hpp"

Weather::Weather(QObject *parent) : QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);
	m_apiKey = "ffffffffffffffffffffffffffffffff"; //Pleas enter API Key 
}

void Weather::UpdateWeather(double latitude, double longitude)
{
    QString url = QString("https://api.openweathermap.org/data/2.5/weather?lat=%1&lon=%2&appid=%3&units=metric").arg(QString::number(latitude), QString::number(longitude), m_apiKey);

    QNetworkRequest request;
    request.setUrl(QUrl(url));

    QNetworkReply *reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished, [=]()
    {
        if (reply->error() == QNetworkReply::NoError)
        {
            QByteArray json = reply->readAll();
            ParseJson(json);
        }
        else
        {
            qWarning() << "Error retrieving weather data:" << reply->errorString();
        }

        reply->deleteLater();
    });
}


void Weather::ParseJson(const QByteArray& json)
{
    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonObject obj = doc.object();

    QString description = obj.value("weather").toArray().at(0).toObject().value("description").toString();

    QJsonArray weather_array = obj.value("weather").toArray();
    QString icon = weather_array.at(0).toObject().value("icon").toString();
    double temperature = obj.value("main").toObject().value("temp").toDouble();
    double humidity = obj.value("main").toObject().value("humidity").toDouble();

    QString icon_path = "/tmp/weather_icon.png";
    DownloadIcon(icon, icon_path);


    emit WeatherUpdated(description, icon_path, temperature, humidity);
}

void Weather::DownloadIcon(const QString& iconCode, const QString& icon_path)
{
    QString url = QString("https://openweathermap.org/img/w/%1.png").arg(iconCode);
    QNetworkRequest request(url);

    QNetworkReply* reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished, [=]()
    {
        if (reply->error() == QNetworkReply::NoError)
        {
            QPixmap icon;
            icon.loadFromData(reply->readAll());

            emit IconDownloadFinished(icon, icon_path);
        }
        else
        {
            qWarning() << "Error downloading weather icon:" << reply->errorString();
        }

        reply->deleteLater();
    });
}

void Weather::IconDownloadFinished(const QPixmap& icon, const QString& icon_path)
{
    // Save icon to a file
    QString filename = icon_path;
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    icon.save(&file, "PNG");
    file.close();
}

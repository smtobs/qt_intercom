#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QPixmap"
#include <QMessageBox>
#include <QtCore>
#include <QtNetwork>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    /* Update Display Time */
    this->TimerDisplay();

    /* Set UI */
    ui->setupUi(this);

    /* Create ImageRecive */
    image_reciver = new ImageReciver(this);
    connect(image_reciver, &ImageReciver::GenerateWebCamImage, this, &MainWindow::UpdateWebCamImage);

    /* Get weather info */
    weather = new Weather();
    connect(weather, &Weather::WeatherUpdated, this, &MainWindow::UpdateWeatherInfo);
    weather->UpdateWeather(37.4653, 126.7052);

    /* Mqtt */
    mqtt_iface = new MqttIface("192.168.219.103", "door", 1, 20, 20);
    this->mqtt_timer = new QTimer(this);
    this->mqtt_timer->setInterval(3000);

    connect(this->mqtt_timer, SIGNAL(timeout()), this, SLOT(TimerMqttEventHandler()));
    mqtt_timer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mqtt_iface;
    delete image_reciver;
    delete weather;
    delete display_timer;
}

void MainWindow::UpdateWebCamImage(const QString& img_path)
{
    try
    {
        QPixmap pix(img_path);
        if (pix.isNull())
        {
            throw std::runtime_error("Failed to load image.");
        }
        ui->labelWebCamImg->setPixmap(pix);
    }
    catch (const std::exception& e)
    {
        qDebug() << "Exception caught: " << e.what();
    }
}

void MainWindow::UpdateWeatherInfo(const QString& description, const QString& icon_path, const double temperature, const double humidity)
{
    /* Update weather icon */
    QPixmap pixmap(icon_path);
    ui->labelWeth->setPixmap(pixmap);

    /* Update temperature text */
    QString text = QString::number(temperature, 'f', 1) + " °C";
    ui->labelTemp->setText(text);

    /* Update humidity text */
    text = QString::number(humidity, 'f', 1) + " %";
    ui->labelHum->setText(text);
}

void MainWindow::TimerDisplay()
{
    display_timer = new QTimer(this);

    connect(display_timer, &QTimer::timeout, this, &MainWindow::UpdateTime);
    display_timer->start(1000);
}

void MainWindow::UpdateTime()
{ 
    QDateTime current_data_time = QDateTime::currentDateTime();
    QTime current_time = current_data_time.time();

    QString time_period;
    if (current_time.hour() >= 12)
    {
        time_period = "오후";
        if (current_time.hour() > 12)
        {
            current_time = current_time.addSecs(-12 * 60 * 60);
        }
    }
    else
    {
        time_period = "오전";
        if (current_time.hour() == 0)
        {
            current_time = QTime(12, current_time.minute(), current_time.second());
        }
    }
    QString current_date_time_string = current_time.toString("hh:mm:ss");

    ui->labelWebCamImg->setText(time_period + " " + current_date_time_string);
}

void MainWindow::TimerMqttEventHandler()
{
    bool is_msg = mqtt_iface->Yield();
    /* Not Used Subscriber */
}

void MainWindow::on_allowButton_clicked()
{
    if (mqtt_iface->IsConnected() == true)
    {
         mqtt_iface->Publish("/home/access", "allow");
         QMessageBox::information(nullptr, "Title", "door open!");
    }
}

void MainWindow::on_denyButton_clicked()
{
    if (mqtt_iface->IsConnected() == true)
    {
        mqtt_iface->Publish("/home/access", "deny");
        QMessageBox::information(nullptr, "Title", "door close!");
    }
}

void MainWindow::on_poweroffButton_clicked()
{
    qDebug() << "power Off";
    qApp->quit();
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QDateTime>
#include <QTimer>

#include "mqtt_iface.hpp"
#include "image_reciver.hpp"
#include "weather.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void UpdateWebCamImage(const QString& img_path);
    void UpdateWeatherInfo(const QString& description, const QString& icon_path, const double temperature, const double humidity);
    void UpdateTime();
    void TimerMqttEventHandler();

    void on_allowButton_clicked();
    void on_denyButton_clicked();
    void on_poweroffButton_clicked();

private:
    Ui::MainWindow *ui;
    MqttIface* mqtt_iface;
    ImageReciver* image_reciver;
    Weather* weather;
    QTimer* display_timer;
    QTimer* mqtt_timer;

    void TimerDisplay();
};
#endif // MAINWINDOW_H

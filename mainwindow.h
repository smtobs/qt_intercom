#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include "mqtt_iface.hpp"
#include "image_reciver.hpp"

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
    void on_allowButton_clicked();
    void on_denyButton_clicked();
    void UpdateImage(int val);

private:
    Ui::MainWindow *ui;
    MqttIface *mqtt_iface;
    ImageReciver *image_reciver;
};
#endif // MAINWINDOW_H

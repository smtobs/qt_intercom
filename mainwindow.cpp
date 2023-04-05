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
    mqtt_iface = new MqttIface("192.168.219.103", "/home/access", "/home/user", "bsoh22", 1, 20, 20);
    mqtt_iface->ConnectBroker();
    ui->setupUi(this);
    image_reciver = new ImageReciver(this);

    connect(image_reciver, &ImageReciver::GenerateImage, this, &MainWindow::UpdateImage);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mqtt_iface;
    delete image_reciver;
}

void MainWindow::on_allowButton_clicked()
{
    if (mqtt_iface->IsConnected() == true)
    {
         mqtt_iface->Publish(mqtt_iface->GetPubTopicName(), "allow");
          QMessageBox::information(nullptr, "Title", "door open!");
    }
    else
    {
        mqtt_iface->ConnectBroker();
    }

}

void MainWindow::on_denyButton_clicked()
{
    if (mqtt_iface->IsConnected() == true)
    {
        mqtt_iface->Publish(mqtt_iface->GetPubTopicName(), "deny");
        QMessageBox::information(nullptr, "Title", "door close!");
    }
}

void MainWindow::UpdateImage(int val)
{
    QPixmap pix("/tmp/image.jpg");
    this->ui->label_pic->setPixmap(pix);
}

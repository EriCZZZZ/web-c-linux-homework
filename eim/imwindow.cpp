#include "imwindow.h"
#include "ui_imwindow.h"

#include <QMessageBox>
#include <QtNetwork/QHostAddress>

IMWindow::IMWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IMWindow)
{
    ui->setupUi(this);
}

IMWindow::~IMWindow()
{
    delete ui;
    delete client;
}

void IMWindow::on_connectBtn_clicked()
{
    ui->connectionStatus->setText(tr("Connecting..."));
    client = new QTcpSocket(this);
    client->connectToHost(QHostAddress(ui->serverIp->text()), (quint16) ui->port->text().toInt());
    connect(client, SIGNAL(connected()), this, SLOT(connected()));
    connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectError(QAbstractSocket::SocketError)));
}

void IMWindow::connected()
{
    ui->connectionStatus->setText(tr("Connect Successfully."));
    client->write(ui->roomNumber->text().toLocal8Bit().data());
    connect(client, &QTcpSocket::readyRead, [this](){
        this->ui->infos->append(QString::fromLocal8Bit(client->readAll()));
    });
}

void IMWindow::connectError(QAbstractSocket::SocketError error)
{
    ui->connectionStatus->setText(client->errorString());
}

void IMWindow::on_send_clicked()
{
    if (ui->msg->text().count() > 0) {
        client->write(ui->msg->text().toLocal8Bit().data());
        ui->msg->setText(tr(""));
    }
}

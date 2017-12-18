#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startServer_clicked()
{
    this->server = new QTcpServer(this);
    server->listen(QHostAddress(QHostAddress::Any), (quint16) ui->port->text().toInt());
    connect(server, SIGNAL(newConnection()), this, SLOT(newClient()));
    this->ui->cmd->append(tr("listen on ") + ui->port->text());
}

void MainWindow::newClient()
{
    QTcpSocket* tmp = server->nextPendingConnection();
    connect(tmp, &QTcpSocket::readyRead, [this, tmp](){
        emit this->newInfo(tmp);
    });
    connect(this, &MainWindow::newInfo, [this](QTcpSocket* tmp) {
        if(!socket2room.contains(tmp)) {
            int roomNum = tmp->readAll().toInt();
            rooms[roomNum].append(tmp);
            socket2room[tmp] = roomNum;
            ui->cmd->append(QString("[NEW CLIENT][%1:%2][room %3]").arg(tmp->peerAddress().toString()).arg(tmp->peerPort()).arg(roomNum));
            tmp->write(QString("[System]Connect Room %1 Successfully.").arg(roomNum).toLocal8Bit().data());
        } else {
            QString address = tmp->peerAddress().toString();
            quint16 port = tmp->peerPort();
            QString info = QString::fromLocal8Bit(tmp->readAll());
            if (info.length() == 0) {
                return;
            }
            ui->cmd->append(QString("[%1:%2][room %3]%4").arg(address).arg(port).arg(socket2room[tmp]).arg(info));
            for (int i = 0; i < rooms[socket2room[tmp]].length(); i++) {
                rooms[socket2room[tmp]][i]->write(QString("[%1:%2][room %3]%4").arg(address).arg(port).arg(socket2room[tmp]).arg(info).toLocal8Bit().data());
            }
        }
    });
}

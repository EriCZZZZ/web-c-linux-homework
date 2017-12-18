#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void newInfo(QTcpSocket *src);

private slots:
    void on_startServer_clicked();
    void newClient();

private:
    Ui::MainWindow *ui;
    QTcpServer *server;
    QMap<int, QList<QTcpSocket*>> rooms;
    QMap<QTcpSocket*, int> socket2room;
};

#endif // MAINWINDOW_H

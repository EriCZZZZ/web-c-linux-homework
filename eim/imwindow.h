#ifndef IMWINDOW_H
#define IMWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>

namespace Ui {
class IMWindow;
}

class IMWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit IMWindow(QWidget *parent = 0);
    ~IMWindow();

private slots:
    void on_connectBtn_clicked();
    void connected();
    void connectError(QAbstractSocket::SocketError error);

    void on_send_clicked();

private:
    Ui::IMWindow *ui;
    QTcpSocket *client;
};

#endif // IMWINDOW_H

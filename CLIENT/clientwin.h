#ifndef CLIENTWIN_H
#define CLIENTWIN_H

#include <QMainWindow>
#include <QTcpSocket>
#include"request_json.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ClientWin;
}
QT_END_NAMESPACE

class ClientWin : public QMainWindow
{
    Q_OBJECT

public:
    ClientWin(QWidget *parent = nullptr);
    ~ClientWin();

private slots:
    void onReadyRead()
    {
        QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
        if (!clientSocket)
            return;

        QByteArray data = clientSocket->readAll();
        QMessageBox::critical(nullptr, "CLIENT", data);
        data.clear();
    }

    void on_commandLinkButton_pressed();
    void on_commandLinkButton_clicked();

private:
    Ui::ClientWin *ui;
    QTcpSocket sock;
};
#endif // CLIENTWIN_H

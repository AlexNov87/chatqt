#ifndef CLIENTWIN_H
#define CLIENTWIN_H

#include <QMainWindow>
#include <QTcpSocket>
#include"request_json.h"
#include"../structs.h"

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
    void onReadyRead();
    void on_commandLinkButton_clicked();
    void on_commandLinkButton_2_clicked();

private:
    Ui::ClientWin *ui;
    SocketComplect sock;
};
#endif // CLIENTWIN_H

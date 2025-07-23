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

private:
    Ui::ClientWin *ui;
};
#endif // CLIENTWIN_H

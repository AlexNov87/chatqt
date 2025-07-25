#include"mainwindow.h"
void ServerSession::MakeErrorAnsweToSocket(const QJsonObject &answer_obj){
    QByteArray arr;
    auto wg = _srv->_maiwin->ui->lw_events;
    arr = json::WritetoQByteArrayJson(answer_obj);

    wg->addItem(QString(arr));
    WriteToSocketWithFlush(_sock, arr);
}

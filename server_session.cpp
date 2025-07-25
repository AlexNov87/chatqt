#include"mainwindow.h"
void ServerSession::MakeErrorAnsweToSocket(const QJsonObject &answer_obj){
    QByteArray arr;
    auto wg = _srv->_maiwin->ui->lw_events;
    arr = json::WritetoQByteArrayJson(answer_obj);

    wg->addItem(QString(arr));
    WriteToSocketWithFlushAddingSplitSym(_sock->socket, arr);
}

ServerSession::ServerSession
(const std::shared_ptr<GraphicsServer> srv, SocketComplect* sock)
    : _srv(srv), _sock(sock){}

void ServerSession::Execute(){

    //Проверяем валидность указателей на сокет и сервер.
    if(!IsPointersValid()){return;}

    while (auto qbyte = _sock->GetExecuteObject()){

        FatalErrorMessageBox(*qbyte);
        json_obj json_stuff = json::ReadJsonObjectFromQbyteArray(*qbyte);

        //Проверяем текущий объект на валидность. Если объект будет не вален,
        //то вернется объект, содержащий ошибку.
        std::optional<json_obj> err_obj = FirstStepCheckErrors(json_stuff);
        if(err_obj){
            MakeErrorAnsweToSocket(*err_obj);
            continue;
        }
        QByteArray arr = QString("RESPONCE").toUtf8();
        WriteToSocketWithFlushAddingSplitSym(_sock->socket, arr);
    }

}

std::optional<json_obj> ServerSession::FirstStepCheckErrors
    (const json_obj& js_obj){
    //Проверка, если данный объект является объектом ошибки.
    bool is_err = json::IsErrorJsonObject(js_obj);
    if(is_err){
        return js_obj;
    }

    //Есть ли в объекте поле действия.
    auto reason = json::IsContainsFieldAndNotEmpty(js_obj, CONSTANTS::LF_ACTION);
    if(reason){
        return ans_obj::MakeErrorObject(*reason, ACTIONS::SYSTEM);
    }

    //Если это действие есть в списке
    str_type act_value = (js_obj).value(CONSTANTS::LF_ACTION).toString();
    if(!_ACT_SERVER.contains(act_value)){
        return ans_obj::MakeErrorObject("Can not find action:" + act_value, ACTIONS::SYSTEM);
    };
    return std::nullopt;
}

bool ServerSession::IsPointersValid(){
    if(!_srv || !_sock){
        if(!_srv){FatalErrorMessageBox("The server is null");}
        if(!_sock){FatalErrorMessageBox("The socket is null");}
        return false;
    }
    return true;
}

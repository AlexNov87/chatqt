#include "mainwindow.h"
///////////////////////////////////////////////////////////////////////

GraphicsServer::GraphicsServer()  :  ServerBase(), GraphicWidgets() {
    connect(this, &QTcpServer::newConnection,
            this, &GraphicsServer::OnNewConnection);
}

void GraphicsServer::InitGraphicForms(){
    _maiwin = std::make_shared<MainWindow>(shared_from_this());
    _maiwindes = std::make_shared<MainWindowDesigner>(_maiwin);
}

void GraphicsServer::SetDefaultValues(){

    QSpinBox *sb_maxconn = _maiwin->ui->sb_maxconn;
    sb_maxconn->setValue(_max_conn);

    QSpinBox *sb_port = _maiwin->ui->sb_port;
    sb_port->setValue(_port);

    QLineEdit *le_ip = _maiwin->ui->le_setip;
    le_ip->setText(_ip.toString());
}

void GraphicsServer::InitAndRun(){
    InitGraphicForms();
    SetDefaultValues();
    _maiwin->show();
}

QJsonObject GraphicsServer::GetRoomsJs() {

    ACTIONS this_act = ACTIONS::GET_ROOMS_LIST;
    auto lam = [&]{
        str_type roomlist = GetSerializatedRoomList();
        return ans_obj::SuccessServerRooms(roomlist);
    };
    return ans_obj::GuardExceptSetter(lam, this_act);
}

QJsonObject GraphicsServer::GetRoomUsersJs(str_type roomname) {

    ACTIONS this_act = ACTIONS::GET_ROOM_USERS;
    auto lam = [&]{
        LGR(_mtx_room);
        if(!_rooms.contains(roomname)){
            return ans_obj::MakeErrorObject
                ("Not found room "+ roomname , ACTIONS::GET_ROOM_USERS);
        }
        auto room = _rooms.at(roomname);
        auto users = room->SerializatedJsonUsers();
        return ans_obj::SuccessRoomUsers(std::move(roomname), std::move(users));
    };
    return ans_obj::GuardExceptSetter(lam, this_act);
}

json_obj GraphicsServer::LoginUserJs
    (str_type name,  str_type password) {

    ACTIONS this_act = ACTIONS::LOGIN;
    auto lam = [&]{

        if(auto res =_sql_work->AuthorizatedError
                       (name, password, this_act)){
            return  *res;
        }

        return ans_obj::SuccessLogin();
    };
    return ans_obj::GuardExceptSetter(lam, this_act);

}

///////////////////////////////////////////
std::optional<json_obj> FirstStepCheckServerObjectErrors
    (const json_obj& js_obj){
    //Проверка, если данный объект является объектом ошибки.
    bool is_err = json::IsErrorJsonObject(js_obj);
    if(is_err){
        return js_obj;
    }

    static std::set<str_type> obj_complect{CONSTANTS::LF_ACTION};
    //Есть ли в объекте поле действия.
    auto reason = json::IsContainsFieldAndStringAndNotEmpty(js_obj, obj_complect);
    if(reason){
        return ans_obj::MakeErrorObject(*reason, ACTIONS::SYSTEM);
    }

    //Если это действие есть в списке
    str_type act_value = (js_obj).value(CONSTANTS::LF_ACTION).toString();
    if(!_ACT_SERVER.contains(act_value) && !_ACT_ADMIN.contains(act_value)){
        return ans_obj::MakeErrorObject("Can not find action: " + act_value, ACTIONS::SYSTEM);
    };
    return std::nullopt;
}

void ExecuteIncoming(std::shared_ptr<GraphicsServer>srv,
                     SocketComplect* complect){

    QByteArray socket_stuff = complect->socket->readAll();
    complect->AddToBuffer(socket_stuff);

    while (auto qbyte = complect->GetExecuteObject()){

        json_obj json_stuff = json::ReadJsonObjectFromQbyteArray(*qbyte);
        //Проверяем текущий объект на валидность. Если объект будет не вален,
        //то вернется объект, содержащий ошибку.
        std::optional<json_obj> err_obj = FirstStepCheckServerObjectErrors(json_stuff);
        if(err_obj){
            QByteArray arr;
            arr = json::WritetoQByteAnyJson(*err_obj);
            WriteToSocketWithFlushAddingSplitSym(complect->socket, arr);
            continue;
        }
        json_obj answer;
        str_type act_value = (json_stuff).value(CONSTANTS::LF_ACTION).toString();

        if(_ACT_SERVER.contains(act_value)){
           ServerSession session(srv, std::move(json_stuff), complect);
           answer = session.SessionResult();
        }
        else if(_ACT_ADMIN.contains(act_value)){
            ServerAdminSession session(srv, std::move(json_stuff), complect);
            answer = session.SessionResult();
        }
        else{
            answer = ans_obj::MakeErrorObject("UNKNOWN ACTION EXECUTE INCOMING", ACTIONS::SYSTEM);
        }
        QByteArray arr = json::WritetoQByteAnyJson(answer);
        complect->GuardSendMessageOtherSide(arr);
    }
}



#include "structs_ch.h"

ChatUser::ChatUser(str_type name, str_type token,  SocketComplect* socket) :
    _name(std::move(name)),
    _token(std::move(token)),
    _socket(std::move(socket)) {}

QString ChatUser::GetName() const{
    return _name;
}

QString ChatUser::GetToken() const{
    return _token;
}

void ChatUser::RecievePublicMessage(const str_type& author, str_type message){
    auto js = ans_obj::IncomePublicMessage(author, std::move(message));
    QByteArray arr = json::WritetoQByteAnyJson(js);
    _socket->GuardSendMessageOtherSide(arr);
}

void ChatUser::RecievePrivateMessage(const str_type& author, str_type message){
    auto js = ans_obj::IncomePrivateMessage(author, std::move(message));
    QByteArray arr = json::WritetoQByteAnyJson(js);
    _socket->GuardSendMessageOtherSide(arr);
}

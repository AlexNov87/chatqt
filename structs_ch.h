#ifndef STRUCTS_CH_H
#define STRUCTS_CH_H

#include <QTcpServer>
#include<unordered_map>
#include "sql.h"
#include "structs.h"
#include "tokenizer.h"

class ChatRoom;
class ChatUser;
class ServerBase;


class ChatUser
{
public:
    ChatUser(str_type name, str_type token,  SocketComplect* socket) :
        _name(std::move(name)),
        _token(std::move(token)),
        _socket(std::move(socket)) {}

    QString GetName() const{
        return _name;
    }

    QString GetToken() const{
        return _token;
    }

    void RecievePublicMessage(const str_type& author, str_type message){
        auto js = ans_obj::IncomePublicMessage(author, std::move(message));
        QByteArray arr = json::WritetoQByteAnyJson(js);
        _socket->GuardSendMessageOtherSide(arr);
    }

    void RecievePrivateMessage(const str_type& author, str_type message){
         auto js = ans_obj::IncomePrivateMessage(author, std::move(message));
         QByteArray arr = json::WritetoQByteAnyJson(js);
        _socket->GuardSendMessageOtherSide(arr);
    }

protected:

    friend class ChatRoom;
    str_type _name;
    str_type _token;
    SocketComplect* _socket;
};


class MessageManager{
public:
  void  IncomePublicMessage(const str_type& author,const str_type& message){
        LG(mtx);
        json_obj obj;
        obj.insert(author, message);
        _archive_public.push_back(std::move(obj));
        while(_archive_public.size() > 30){
            _archive_public.pop_front();
        }
    }

   QByteArray SerializedLastMessages(){
         LG(mtx);
         return json::WritetoQByteAnyJson(_archive_public);
     }
protected:
    std::mutex mtx;
    json_arr _archive_public;
};

class ChatRoom
{
public:
    ChatRoom(ServerBase* srv, str_type creator, str_type name);
    json_obj AddUser(std::shared_ptr<ChatUser> user);
    json_obj DeleteUserByName(const str_type& name);
    json_obj DeleteUserByToken(const str_type& name);
    json_obj PublicMessage(const str_type&token, str_type message);
    json_obj PrivateMessage(const str_type&token,
                            const str_type& user_to,str_type message);
    const str_type& GetCreator() const;
    str_type SerializatedJsonUsers() const;
    const MessageManager& MessageMan() const {
        return _manager;
    }
protected:

    std::optional<json_obj> MessageCheckErrorTemplate
        (const str_type&token,const str_type& message, ACTIONS action);

    void UpdateRoomMembersForAll(){
        auto roommems = SerializatedJsonUsers();
        auto js = ans_obj::SuccessRoomUsers(_name, std::move(roommems));
        QByteArray buf = json::WritetoQByteAnyJson (js);
        for (auto&& us : _users){
           ChatUser& ch = *us.second;
            ch._socket->GuardSendMessageOtherSide(buf);
        };
    }

    ServerBase* _srv;
    str_type _creator;
    str_type _name;
    mutable std::recursive_mutex _mtx;
    std::map<str_type, std::shared_ptr<ChatUser>> _tokens;
    std::map<str_type, std::shared_ptr<ChatUser>> _users;
    MessageManager _manager;
};

class ServerBase :
/*Для свойств QTcpServer*/
public QTcpServer {
public:

    ServerBase() : QTcpServer(){
        ConfigInit init;

         _sql_work = std::make_shared<sql::SQLWorker>(init.Object());
        _ip.setAddress(init.Object().value(LOAD_CONSTANTS::SERVER_IP).toString());
        _port = init.Object().value(LOAD_CONSTANTS::SERVER_PORT).toInt();
        _max_conn = init.Object().value(LOAD_CONSTANTS::MAX_CONNECTIONS).toInt();
        _max_message_len = init.Object().value(LOAD_CONSTANTS::MAX_MESSAGE_LEN).toInt();

        for(auto&& el : init.Object().value(LOAD_CONSTANTS::DEFAULT_CHATROOMS).toArray()){
            _rooms[el.toString()] = std::make_shared<ChatRoom>(this, "DEFAULT", el.toString());
        }

    }

  virtual json_obj SetIP(str_type ip) = 0;
  virtual json_obj SetPort(int port) = 0;
  virtual json_obj SetMaxUsers(int max) = 0;

  virtual   json_obj AddRoomJs(str_type name, str_type password, str_type roomname) = 0;
  virtual   json_obj DeleteRoomJs(str_type name, str_type password, str_type roomname) = 0;
  virtual   json_obj JoinRoomUserJs(str_type name, str_type password,
                               str_type roomname, SocketComplect* complect) = 0;
  virtual json_obj LoginUserJs(str_type name, str_type password) = 0;
  virtual   json_obj RegisterUserJs(str_type name, str_type password) = 0;
  virtual   json_obj DeleteUserJs(str_type name, str_type password,
                                  str_type to_delete) = 0;
  virtual   json_obj GetRoomsJs() = 0;
  virtual   json_obj GetRoomUsersJs(str_type roomname) = 0;

  virtual json_obj DisconnectJs(str_type token, str_type room_name) = 0;

  virtual json_obj PublicMessageJs
      (str_type token, str_type message, str_type room_name) =0;

  virtual json_obj PrivateMessageJs
      (str_type token, str_type message, str_type user_to, str_type room_name) =0;

  QHostAddress GetIP() const;
  QString GetIPStr() const;
  int GetPort() const;
  int GetMaxUsers() const;
  const std::unordered_map<QString, std::shared_ptr<ChatRoom>>& GetRooms() const;

  protected:

    std::unordered_map<QString, std::shared_ptr<ChatRoom>> _rooms;
    std::unordered_map<QTcpSocket*, SocketComplect> _socket_db;

    bool HasPermission(QString name, QString password, ACTIONS act){
        return true;
    }

    bool HasPermissionAdmin(QString name, QString password, ADMIN_ACTIONS act){
        return true;
    }
    QString GetSerializatedRoomList();

    mutable std::recursive_mutex _mtx_room;
    mutable std::mutex _mtx_net;
    Service::TokenGen _token_generator;

    QHostAddress _ip;
    int _max_conn = 100;
    int _port = 80;
    int _max_message_len = 512;
    std::shared_ptr<sql::SQLWorker> _sql_work;

};

#endif // STRUCTS_CH_H

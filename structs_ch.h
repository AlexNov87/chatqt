#ifndef STRUCTS_CH_H
#define STRUCTS_CH_H

#include <QTcpServer>
#include<unordered_map>

#include "structs.h"
#include "tokenizer.h"

class ChatRoom;
class ChatUser;
class ServerBase;


class ChatUser
{
public:
    ChatUser(QString name, QTcpSocket* socket, ChatRoom* chatroom) :
        _name(std::move(name)), _socket(std::move(socket)), _chatroom(chatroom){}

    QString GetName() const{
        return _name;
    }

    QString GetToken() const{
        return _token;
    }
private:
    friend class ChatRoom;
    QString _name;
    QString _token;
    QTcpSocket* _socket;
    ChatRoom* _chatroom;
};

class ChatRoom
{
public:
    ChatRoom(ServerBase* srv, QString creator) : _srv(srv),
    _creator(std::move(creator)) {
        assert(srv != nullptr);

    }

    std::optional<QString> AddUser(std::shared_ptr<ChatUser> user){

        try{
            LG(_mtx);
            if(_users.contains(user->GetName())){
                return "User " + user->GetName() + " is already in room" ;
            };

            _users[user->GetName()] = user;
            _tokens.insert(user->GetToken());
            return std::nullopt;
        }
        catch(const std::exception&ex){
            return  "AddUser exception: " + QString(ex.what());
        }
    }

    std::optional<QString> DeleteUser(const QString& name){

        try{
            LG(_mtx);
            if(!_users.contains(name)){
                return "There is no user with name: "+ name;
            }
           auto user = _users.at(name);
            _users.erase(name);
            _tokens.erase(user->GetToken());
            return std::nullopt;
        }
        catch(const std::exception&ex){
            return  "DeleteUser exception: " + QString(ex.what());
        }
    }

    const QString& GetCreator(){
        return _creator;
    }

    QString SerializatedJsonUsers(){
        LG(_mtx);
        return json::GetMapMembersJsonArrayView(_users);
    }

protected:
    ServerBase* _srv;
    std::unordered_map<QString, std::shared_ptr<ChatUser>> _users;
    QString _creator;
    std::mutex _mtx;
    std::unordered_set<QString> _tokens;
};

class ServerBase :
/*Для свойств QTcpServer*/
public QTcpServer,
/*Для установки настроек из файла или по умолчанию*/
public ConfigInit {
public:

    ServerBase() : QTcpServer(), ConfigInit() {}

  virtual json_obj SetIP(str_type ip) = 0;
  virtual json_obj SetPort(int port) = 0;
  virtual json_obj SetMaxUsers(int max) = 0;

  virtual   json_obj AddRoomJs(str_type name, str_type password, str_type roomname) = 0;
  virtual   json_obj DeleteRoomJs(str_type name, str_type password, str_type roomname) = 0;
  virtual   json_obj LoginUserJs(str_type name, str_type password, str_type roomname) = 0;
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
  const std::unordered_set<QString>& GetRooms() const;

protected:

    struct UserRole{
        QString password;
        Role role;
    };

    std::unordered_map<QString, std::shared_ptr<ChatRoom>> _rooms;
    std::unordered_map<QString, UserRole> _pass_hash;
    std::unordered_map<QTcpSocket*, SocketComplect> _socket_db;

    bool HasPermission(QString name, QString password, ACTIONS act){
        return true;
    }

    bool IsUserInBase(QString name, QString password){
        return (_pass_hash.contains(name));
    }

    QString GetSerializatedRoomList();

    mutable std::mutex _mtx_mod_users;
    mutable std::mutex _mtx_room;
    mutable std::mutex _mtx_net;
    Service::TokenGen _token_generator;

};

#endif // STRUCTS_CH_H

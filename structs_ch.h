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
    ChatUser(str_type name, str_type token,  SocketComplect* socket);
    QString GetName() const;
    QString GetToken() const;
    void RecievePublicMessage(const str_type& author, str_type message);
    void RecievePrivateMessage(const str_type& author, str_type message);

protected:

    friend class ChatRoom;
    str_type _name;
    str_type _token;
    SocketComplect* _socket;
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
    void UpdateRoomMembersForAll();

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

  ServerBase();

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

  virtual json_obj UpdateUserRoleJs(str_type name, str_type password,
   str_type to_update, Role role) = 0;

  virtual json_obj BanUserJs(str_type name, str_type password,str_type to_ban) = 0;
  virtual json_obj UnBanUserJs(str_type name, str_type password,str_type to_ban) = 0;



  QHostAddress GetIP() const;
  QString GetIPStr() const;
  int GetPort() const;
  int GetMaxUsers() const;
  const std::unordered_map<QString, std::shared_ptr<ChatRoom>>& GetRooms() const;

  protected:

    std::unordered_map<QString, std::shared_ptr<ChatRoom>> _rooms;
    std::unordered_map<QTcpSocket*, SocketComplect> _socket_db;

    bool HasPermissionAdmin(QString name, QString password, ADMIN_ACTIONS act){
        return true;
    }
    str_type GetSerializatedRoomList();

    std::optional<json_obj> AuthorizatedAndHasPermissionAdmin
        (QString name, QString password, ADMIN_ACTIONS act);

    mutable std::recursive_mutex _mtx_room;
    mutable std::mutex _mtx_net;
    Service::TokenGen _token_generator;

    QHostAddress _ip;
    int _max_conn = 100;
    int _port = 80;
    int _max_message_len = 512;
    std::shared_ptr<sql::SQLWorker> _sql_work;

    bool _is_cached_roomlist = false;
    str_type _cached_roomlist_json_with_owners;

    const str_type& GetRoomlistWithOwners();


};

#endif // STRUCTS_CH_H

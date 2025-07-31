#ifndef STRUCTS_H
#define STRUCTS_H
#include"helpfoo.h"
#include"constants.h"
#include"constants_load.h"
#include"initializators_help.h"
#include<QHostAddress>
#include<mutex>
#include <memory>
#include <deque>
#include <chrono>
#include <variant>
#include<QListWidget>
#include<QTcpSocket>


struct ConfigInit
{
public:
    ConfigInit();
    const json_obj& Object(){return _object;}
private:
 json_obj _object;
 QHostAddress test_adress;
  void  CheckServerAllRight();
  void  CheckSQLAllRight();
  void  CheckRoomField();
};

struct SocketComplect{
    SocketComplect();
    QTcpSocket* socket;
    QByteArray buffer;
    QChar terminator = CONSTANTS::SERIAL_SYM;

    std::optional<QByteArray> GetExecuteObject();
    int AddToBuffer(const QByteArray& arr);
    void GuardSendMessageOtherSide(QByteArray arr);

private:
     std::shared_ptr<std::mutex> _mtx;
};

class NamePassword {
public:
    const str_type& Name(){
        return _name;
    }

    const str_type& Password(){
        return _pass;
    }

protected:
    str_type _name;
    str_type _pass;
};

#endif // STRUCTS_H

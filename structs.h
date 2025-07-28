#ifndef STRUCTS_H
#define STRUCTS_H
#include"helpfoo.h"
#include"sql.h"
#include"constants.h"
#include"constants_load.h"
#include"initializators_help.h"
#include<QHostAddress>
#include<mutex>
#include <memory>
#include <deque>
#include <chrono>
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

  private :

};

struct SocketComplect{

    SocketComplect(){
        _mtx = std::make_shared<std::mutex>();
    }

    QTcpSocket* socket;
    QByteArray buffer;
    QChar terminator = CONSTANTS::SERIAL_SYM;

    std::optional<QByteArray> GetExecuteObject(){
        LG(*_mtx);
        int cnt = 0;
        QString tempString;
        for (QChar ch : buffer){
            if(ch != terminator){
                tempString.append(ch);
                ++cnt;
            }
            else{
                buffer.remove(0, cnt+1);
                QByteArray arr = tempString.toUtf8();
                return arr;
            }
        }
        return std::nullopt;
    }

    int AddToBuffer(const QByteArray& arr){
        LG(*_mtx);
        buffer.append(arr);
        return arr.size();
    }

   void GuardSendMessageOtherSide(QByteArray arr){
        LG(*_mtx);
        WriteToSocketWithFlushAddingSplitSym(socket, arr);
    }

private:
     std::shared_ptr<std::mutex> _mtx;
};

#endif // STRUCTS_H

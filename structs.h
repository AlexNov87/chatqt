#ifndef STRUCTS_H
#define STRUCTS_H
#include"helpfoo.h"
#include"constants.h"
#include"initializators_help.h"
#include<QHostAddress>
#include<mutex>
#include <memory>
#include <deque>
#include <chrono>
#include<QListWidget>
#include<QTcpSocket>

enum class Role {
    USER , SUPERUSER , ADMIN , MASTER
};

class File_Json {
public:
    File_Json();
    const QJsonObject& JsonValues() const;
private:
    void  SetConfigFile();
    QString _file_config;
    QJsonObject _values;
};

class ConfigInit
{
public:
    ConfigInit();
protected:
    QHostAddress _ip;
    int _port = 80;
    int _max_conn = 100;
    std::unordered_set<QString> _roomlist;
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

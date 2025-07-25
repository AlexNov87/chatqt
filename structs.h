#ifndef STRUCTS_H
#define STRUCTS_H
#include"helpfoo.h"
#include"constants.h"
#include"initializators_help.h"
#include<QHostAddress>
#include<mutex>
#include <memory>
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


    QTcpSocket* socket;
    QByteArray buffer;
    QChar terminator = CONSTANTS::SERIAL_SYM;

    std::optional<QByteArray> GetExecuteObject(){

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
        buffer.append(arr);
        return arr.size();
    }


};

#endif // STRUCTS_H

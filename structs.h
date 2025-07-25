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

    std::optional<QByteArray> ToExecute(){
        int cnt = 0;
        QTextStream stream;
        for (QChar ch : buffer){
            if(ch != terminator){
                stream << ch;
                ++cnt;
            }
            else{
                buffer.remove(0, cnt+1);
                return stream.string()->toUtf8();
            }
        }
        return std::nullopt;
    }

    int ReadToBuffer(){
        QByteArray arr = socket->readAll();
        buffer.append(std::move(arr));
        return arr.size();
    }




};

#endif // STRUCTS_H

#ifndef STRUCTS_H
#define STRUCTS_H
#include"helpfoo.h"
#include"constants.h"
#include"initializators_help.h"
#include<QHostAddress>
#include<mutex>
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

#endif // STRUCTS_H

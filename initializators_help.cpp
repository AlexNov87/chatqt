#include "initializators_help.h"

bool IsCorrectPort(int port){
    if((port <0) || (port > 65535)){ return false;}
    return true;
}

bool IsCorrectIP(const QString &ip){
    QHostAddress adress;
    bool result = adress.setAddress(ip);
    return result;
}

bool IsCorrectConnections(int conn){
    return bool(conn > 0);
}

bool IsCorrertInitiator(const str_type & initiator){
    return( (initiator == CONSTANTS::RF_DIRECTION_CHATROOM )||
            (initiator == CONSTANTS::RF_DIRECTION_SERVER));
}

namespace json {
std::optional<std::unordered_set<QString> > FromJSONUsetByName(const QString &name, const QJsonObject &obj){

    if(!obj.contains(name)){
        return std::nullopt;
    }

    if(!obj.value(name).isArray()){
        FatalErrorMessageBox("The value is not array");
        return std::nullopt;
    }

    const auto& arr = obj.value(name).toArray();
    int cnt = 0;
    std::unordered_set<QString> result;
    for(auto && room : arr){
        if(!room.isString()){
            FatalErrorMessageBox("The value num: " +
                                 QString::number(cnt)  + " is not string");
            return std::nullopt;
        }
        else {
            result.insert(room.toString());
        }
        ++cnt;
    }
    return result;
}

std::optional<int> FromJSONintByName(const QString &name, const QJsonObject &obj){
    if(!obj.contains(name)){
        return std::nullopt;
    }

    if(!obj.value(name).isDouble()){
        FatalErrorMessageBox("The value is not numeric");
        return std::nullopt;
    }
    return  obj.value(name).toInt();
}

std::optional<QString> FromJSONIP(const QString &name, const QJsonObject &obj){

    auto ip = FromJSONQStringByName(name, obj);
    if(!ip){
        return ip;
    }
    if(!IsCorrectIP(ip.value())){
        FatalErrorMessageBox("The IP: " + ip.value() + " is not correct");
        return std::nullopt;
    }
    return ip.value();
}

std::optional<QString> FromJSONQStringByName(const QString &name, const QJsonObject &obj){
    if(!obj.contains(name)) {
        return std::nullopt;
    }

    if(!obj.value(name).isString()){
        FatalErrorMessageBox("The field:" + name + " is not string");
        return std::nullopt;
    }
    return obj.value(name).toString();

}

std::optional<str_type> CheckInitObject(const json_obj&obj){
    static std::set<str_type> string_fields {
        LOAD_CONSTANTS::DB_HOST,LOAD_CONSTANTS::DB_PASSWORD,
        LOAD_CONSTANTS::DB_TABLE_NAME, LOAD_CONSTANTS::DB_USER,
        LOAD_CONSTANTS::SERVER_IP
    };
    static std::set<str_type> numeric_fields {
        LOAD_CONSTANTS::DB_PORT, LOAD_CONSTANTS::MAX_CONNECTIONS,
        LOAD_CONSTANTS::MAX_MESSAGE_LEN, LOAD_CONSTANTS::SERVER_PORT
    };

    auto reason = IsContainsFieldAndInt(obj, numeric_fields);
    if(reason){return reason;}
    reason = IsContainsFieldAndStringAndNotEmpty(obj, string_fields);
    if(reason){return reason;}

    return std::nullopt;
}

}


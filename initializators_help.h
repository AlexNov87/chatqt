#ifndef INITIALIZATORS_HELP_H
#define INITIALIZATORS_HELP_H

#include"constants.h"
#include"constants_load.h"
#include"helpfoo.h"

#include<optional>
#include <QString>
#include<QHostAddress>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>

bool IsCorrertInitiator(const str_type& initiator);
bool IsCorrectPort(int port);
bool IsCorrectIP(const QString& ip);
bool IsCorrectConnections(int conn);

namespace json {

std::optional<QString> FromJSONQStringByName(const QString& name,
                                             const QJsonObject& obj);;
std::optional<QString> FromJSONIP(const QString& name,const QJsonObject& obj);

std::optional<int> FromJSONintByName(const QString& name,
                                     const QJsonObject& obj);

std::optional<std::unordered_set<QString>>
FromJSONUsetByName(const QString& name, const QJsonObject& obj);

///////////////////////////////////////////////////////////////

template<typename Container>
std::optional<str_type> IsContainsFieldAndStringAndNotEmpty
    (const json_obj&obj, const Container& fields){

    for(auto && field: fields){
        if(!obj.contains(field)){
            return "There is no field: " + field;
        }

        if(!obj.value(field).isString()){
            "The field: " + field + " is not string";
        }

        if(obj.value(field).toString().isEmpty()){
           "The field: " + field + " is empty";
        }
    }
    return std::nullopt;
}


template<typename Container>
std::optional<str_type> IsContainsFieldAndInt
    (const json_obj&obj, const Container& fields){
    for(auto && field: fields){
        if(!obj.contains(field)){
            return "There is no field: " + field;
        }
        if(!obj.value(field).isDouble()){
            "The field: " + field + " is not numeric";
        }
    }
    return std::nullopt;
}

std::optional<str_type> CheckInitObject(const json_obj&obj);

}//NAMESPACE
#endif // INITIALIZATORS_HELP_H



#ifndef INITIALIZATORS_HELP_H
#define INITIALIZATORS_HELP_H

#include"constants.h"
#include"helpfoo.h"

#include<optional>
#include <QString>
#include<QHostAddress>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>

bool IsCorrectPort(int port);
bool IsCorrectIP(const QString& ip);
bool IsCorrectConnections(int conn);

std::optional<QString> FromJSONQStringByName(const QString& name,
                                             const QJsonObject& obj);;
std::optional<QString> FromJSONIP(const QString& name,const QJsonObject& obj);

std::optional<int> FromJSONintByName(const QString& name,
                                     const QJsonObject& obj);


std::optional<std::unordered_set<QString>>
FromJSONUsetByName(const QString& name, const QJsonObject& obj);


///////////////////////////////////////////////////////////////

bool IsCorrertInitiator(const str_type& initiator);

template<typename... Args>
std::optional<str_type> IsContainsFieldAndNotEmpty
    (const json_obj&obj, Args... args){

    std::unordered_set<str_type> fields;
    (fields.insert(std::move(args)), ...);

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
#endif // INITIALIZATORS_HELP_H



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

#endif // INITIALIZATORS_HELP_H



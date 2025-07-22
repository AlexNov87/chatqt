#ifndef HELPFOO_H
#define HELPFOO_H
#include<thread>
#include<mutex>
#include<QString>
#include<QMessageBox>
#include<QFileDialog>
#include<unordered_set>
#include <QFile>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>

#define LG(mutex_name) std::lock_guard<std::mutex> lg(mutex_name)

QString SelectConfigFile(QString comment);
bool SetEnvVariable(QString name, QString param);
void FatalErrorMessageBox(const QString& what , QString framename = "Error");

QJsonDocument ReadJsonDocument(const QString &path);
QJsonObject ReadFromFileConfig(const QString& path);

template<typename Foo>
void AnotherThreadRunFoo(Foo foo){
    std::jthread jth ([=]{
        foo();
    });
}

template<typename JSON>
void WritetoFileJson(const JSON& json){
    QString write_pth = QFileDialog::getSaveFileName
        (nullptr,"Save JSON To file", QDir::currentPath(), "*.txt;*.conf;*.json");
    if(write_pth.isEmpty()){
        return;
    }
    QFile file(write_pth);
    if(!file.open(QIODevice::WriteOnly)){
        FatalErrorMessageBox("Can not open file to write");
        return;
    }
    QJsonDocument doc(json);
    file.write(doc.toJson());
    file.close();
    QMessageBox::information(nullptr, "SaveSucess", "File successfully was writen to: " + write_pth);
}

#endif // HELPFOO_H




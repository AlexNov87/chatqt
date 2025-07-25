#ifndef HELPFOO_H
#define HELPFOO_H
#include"answer_obj.h"


#include<thread>
#include<mutex>
#include<QString>
#include<QMessageBox>
#include<QFileDialog>
#include<unordered_set>
#include <QFile>
#include <QTcpSocket>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>

#define LG(mutex_name) std::lock_guard<std::mutex> lg(mutex_name)

//DIALOGS
QString SelectConfigFile(QString comment);
void FatalErrorMessageBox(const QString& what , QString framename = "Error");
bool ChooseBox(const QString& question);

//JSON

namespace  json {

QJsonDocument ReadJsonDocument(const QString &path);
std::optional<QJsonDocument> ReadJsonFromQByte(QByteArray array);
QJsonObject ReadJsonFromFileConfig(const QString& path);

template<typename K, typename U, template<typename, typename> class MapType>
QString GetMapMembersJsonArrayView (const MapType<K, U>& map){

        QTextStream stream;
        int cnt = 0;
        int max = map.size() - 1;
        stream << "[\n";
        {
            for(auto&& elem : map){
                stream <<'"'<< elem.first << '"';
                ++cnt;
                if(cnt == max){break;}
                stream << ',';
            }
        }
        stream << "\n]\n";
        return *stream.string();
}

template<typename JSON>
void WritetoFileJson(JSON& json){
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

template<typename JSON>
QByteArray WritetoQByteArrayJson(const JSON& json){
    QJsonDocument doc(json);
    return doc.toJson();
}

bool IsErrorJsonObject(const json_obj& obj);
json_obj ReadJsonObjectFromQbyteArray(QByteArray& arr);

}//NANESPACE JSON

//OTHER
template<typename Foo>
void AnotherThreadRunFoo(Foo foo){
    std::jthread jth ([=]{
        foo();
    });
}
void WriteToSocketWithFlush(QTcpSocket* socket, const QByteArray& arr);
void WriteToSocketWithFlushAddingSplitSym(QTcpSocket* socket,QByteArray& arr);


#endif // HELPFOO_H




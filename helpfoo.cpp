#include "helpfoo.h"
//DIALOGS
QString SelectConfigFile(QString comment){
    return QFileDialog::getOpenFileName(nullptr, std::move(comment),
                                        QDir::currentPath(), "*.txt;*.json;*.conf");
}

void FatalErrorMessageBox(const QString &what, QString framename ){
    QMessageBox::critical(nullptr, std::move(framename), what);
}

bool ChooseBox(const QString& question){
    auto result = QMessageBox::warning(nullptr, "", std::move(question),
                                       QMessageBox::StandardButton::Yes,QMessageBox::StandardButton::No);
    if (result == QMessageBox::StandardButton::Yes){
        return true;
    }
    return false;
}

//JSON
namespace json {

QJsonObject ReadJsonFromFileConfig(const QString &path){

    try{
        QJsonDocument doc = ReadJsonDocument(path);
        if(!doc.isObject()){
            FatalErrorMessageBox("Parsed JSON value is not an JSON-object");
            return {};
        }
        return doc.object();
    }
    catch(const std::exception&ex){
        FatalErrorMessageBox("ReadFromFileConfig: " + QString(ex.what()));
        return {};
    }
    return {};
}

QJsonDocument ReadJsonDocument(const QString &path){
    try{
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            FatalErrorMessageBox("Could not open file to read");
            return {};
        }
        QByteArray jsondata = file.readAll();
        file.close();

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(jsondata,&err);
        if(err.error != QJsonParseError::NoError){
            FatalErrorMessageBox("JSON parse error:" + err.errorString());
            return {};
        }
        return doc;
    }
    catch(const std::exception&ex){
        FatalErrorMessageBox("ReadJsonDocument JSON Parse EX: " + QString(ex.what()));
        return {};
    }
    return {};
}

std::optional<QJsonDocument> ReadJsonFromQByte(QByteArray array){
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(array,&err);
    if(err.error != QJsonParseError::NoError){
        return std::nullopt;
    }
    return doc;
}

json_obj ReadJsonObjectFromQbyteArray(QByteArray& data){

    try
    {
      std::optional<QJsonDocument> readjson = ReadJsonFromQByte(data);

    //Произошла ошибка во время чтения JSON.
    if(!readjson.has_value()){
        FatalErrorMessageBox("READJSONERR: "+ QString(data));
        return  ans_obj::MakeErrorObject("Errors occurs while readind json", ACTIONS::SYSTEM);
    }

    //документ JSON не объект.
    if(!(readjson->isObject())){
        return  ans_obj::MakeErrorObject("The json-document is not an json-object", ACTIONS::SYSTEM);
    }
    const auto& obj = readjson->object();

    //Проверка, что все значения по ключу имеют string представления.
    for (auto && key : obj.keys()){
        if(!obj.value(key).isString()){
            return  ans_obj::MakeErrorObject("The member: "+key +" of the object is not string" , ACTIONS::SYSTEM);
        }
    }
       return obj;
    }
    catch(const std::exception&ex){
        return  ans_obj::MakeErrorObject("ReadJsonObjectFromSocket EX:" + QString(ex.what()) , ACTIONS::SYSTEM);
    }
}

bool IsErrorJsonObject(const json_obj& obj){
    if(!obj.contains(CONSTANTS::LF_RESULT)) {return false;}
    if(obj.value(CONSTANTS::LF_RESULT).toString() == CONSTANTS::RF_ERROR){
        return true;
    }
    return false;
}

}

//OTHER
void WriteToSocketWithFlush(QTcpSocket* socket, const QByteArray& arr){
    socket->write(arr);
    socket->flush();
}

void WriteToSocketWithFlushAddingSplitSym(QTcpSocket* socket,QByteArray& arr){
    arr.push_back(CONSTANTS::SERIAL_SYM);
    WriteToSocketWithFlush(socket, arr);
}

void Fatal(const str_type& str){
    FatalErrorMessageBox(str, "Config Init");
    std::abort();
}

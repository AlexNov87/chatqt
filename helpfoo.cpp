#include "helpfoo.h"

QString SelectConfigFile(QString comment){
    return QFileDialog::getOpenFileName(nullptr, std::move(comment),
                                        QDir::currentPath(), "*.txt;*.json;*.conf");
}

bool SetEnvVariable(QString name, QString param){
    try
    {
        return qputenv(name.toUtf8().constData() , param.toUtf8());
    }
    catch(const std::exception& ex){
        QMessageBox::critical(nullptr, "Mistake", "SetEnvVariable EX:" + QString(ex.what()));
    }
    return false;
}


QJsonObject ReadFromFileConfig(const QString &path){

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

void FatalErrorMessageBox(const QString &what, QString framename ){
    QMessageBox::critical(nullptr, std::move(framename), what);
}

#include "structs.h"
NetSettings::NetSettings(){
    _ip.setAddress("127.0.0.1");
}

bool NetSettings::SetListenIP(const QString& ip){
    LM(_net_mtx);
    bool res = _ip.setAddress(ip);
    if(!res){
        FatalErrorMessageBox(ip + " is not correct IP-adress");
        return res;
    }
    return res;
}

void NetSettings::SetPort(int port){
    LM(_net_mtx);
    if( (port < 0) || (port > 65535)){
        FatalErrorMessageBox(QString::number(port) + " can not be as port");
        return;
    };
    _port = port;
}

void NetSettings::SetMaxConnections(int conn){
    LM(_net_mtx);
    if( (conn < 1) || (conn > 999999)){
        FatalErrorMessageBox(QString::number(conn) +
                             " can not be used as connection (1 - 999.999)");
        return;
    };
    _max_conn = conn;
}

int NetSettings::GetAdressProtocol() {
    LM(_net_mtx);
    int protocol = _ip.protocol();
    switch(protocol){
    case 0:
        return 4;
    case 1:
        return 6;
    default:
        return -1;
    }
    return -1;
}

QHostAddress NetSettings::GetListenIP() {
    LM(_net_mtx);
    return _ip;
}

int NetSettings::GetPort() {
    LM(_net_mtx);
    return _port;
}

int NetSettings::GetMaxConnections(){
    LM(_net_mtx);
    return _max_conn;
}

void NetSettings::SetNetworkParamstoJSONObject(QJsonObject& obj){
    LM(_net_mtx);
    obj[__IP] = _ip.toString();
    obj[__PORT] = _port;
    obj[__MAX_CONNECTIONS] = _max_conn;
}

void NetSettings::SetNetworkDefaults(const QJsonObject& obj){
    if(obj.contains(__IP)){
        if(!obj.value(__IP).isString()){
            FatalErrorMessageBox("IP perfomance is not string");
        }
        else
        {
            SetListenIP(obj.value(__IP).toString());
        }
    }

    if(obj.contains(__MAX_CONNECTIONS)){
        if(!obj.value(__MAX_CONNECTIONS).isDouble()){
            FatalErrorMessageBox("MAX_CONNECTIONS perfomance is not numeric");
        }
        else
        {
            SetMaxConnections(obj.value(__MAX_CONNECTIONS).toInt());
        }
    }

    if(obj.contains(__PORT)){
        if(!obj.value(__PORT).isDouble()){
            FatalErrorMessageBox("MAX_CONNECTIONS perfomance is not numeric");
        }
        else
        {
            SetPort(obj.value(__PORT).toInt());
        }
    }
}

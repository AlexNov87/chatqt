#include"structs.h"


SocketComplect::SocketComplect(){
    _mtx = std::make_shared<std::mutex>();
}

std::optional<QByteArray> SocketComplect::GetExecuteObject(){
    LG(*_mtx);
    int cnt = 0;
    QString tempString;
    for (QChar ch : buffer){
        if(ch != terminator){
            tempString.append(ch);
            ++cnt;
        }
        else{
            buffer.remove(0, cnt+1);
            QByteArray arr = tempString.toUtf8();
            return arr;
        }
    }
    return std::nullopt;
}

int SocketComplect::AddToBuffer(const QByteArray& arr){
    LG(*_mtx);
    buffer.append(arr);
    return arr.size();
}

void SocketComplect::GuardSendMessageOtherSide(QByteArray arr){
    LG(*_mtx);
    WriteToSocketWithFlushAddingSplitSym(socket, arr);
}

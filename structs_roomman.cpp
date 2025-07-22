#include "structs.h"

RoomsManager::RoomsManager(QListWidget* rooms_wid) :
    _rooms_wid(rooms_wid) {}

void RoomsManager::AddRoom(QString room){
    LM(_mut_rooms);
    if(_roomlist.contains(room)){
        AnotherThreadRunFoo([=]{
            FatalErrorMessageBox("Room "+ room + " is already in list");
        });
        return;
    }
    _rooms_wid->addItem(room);
    _roomlist.insert(std::move(room));
    _arr_tmp = {};
}
void RoomsManager::RemoveRoom(const QString& room){
    LM(_mut_rooms);
    if(!_roomlist.contains(room)){
        AnotherThreadRunFoo([=]{
            FatalErrorMessageBox("Room "+ room + " is not in list");
        });
        return;
    }
    auto items = _rooms_wid->findItems(room, Qt::MatchExactly);
    for (auto item : items) {
        delete _rooms_wid->takeItem(_rooms_wid->row(item));
    }
    _roomlist.erase(_roomlist.find(room));
    _arr_tmp = {};
}

const QSet<QString>& RoomsManager::GetRooms(){
    LM(_mut_rooms);
    return _roomlist;
}

void RoomsManager::SetRoomstoJSONObject(QJsonObject& obj){
    LM(_mut_rooms);
    if(!_arr_tmp.isEmpty()){
        for (auto && room : _roomlist){
            _arr_tmp.push_back(room);
        }};
    obj[__ROOMS] = _arr_tmp;
}

void RoomsManager::SetRoomsDefaults(const QJsonObject& obj){
    if(!obj.contains(__ROOMS)){

    }

    if(!obj.value(__ROOMS).isArray()){
        FatalErrorMessageBox("Rooms field is not JSON array");
        return;
    }

    const auto& arr = obj.value(__ROOMS).toArray();
    for(auto&& el : arr){
        if(!el.isString()){
            FatalErrorMessageBox("Wrong type of room-element, must be string");
            return;
        }
    }

    for(auto&& el : arr){
        AddRoom(el.toString());
    };
}

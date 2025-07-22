#include "structs.h"

//FILE JSON
File_Json::File_Json() {
    SetConfigFile();
    if(!_file_config.isEmpty()){
        _values = ReadFromFileConfig(_file_config);
    }
}

const QJsonObject& File_Json::JsonValues() const {
    return _values;
}

void  File_Json::SetConfigFile(){
    _file_config = SelectConfigFile("Select your config file");
    if(_file_config.isEmpty()){
        QMessageBox::information(nullptr, "Info", "No config presets, values will be default");
    }
}

//CONFIG INIT
    ConfigInit::ConfigInit() {
        _ip.setAddress("127.0.0.1");
        File_Json json_defaults;
        const auto& obj = json_defaults.JsonValues();

        auto ip = FromJSONIP(CONSTANTS::IP, obj);
        if(ip){
            _ip.setAddress(ip.value());
        }

        auto port = FromJSONintByName(CONSTANTS::PORT, obj);
        if(port){
            _port = port.value();
        }

        auto max_conn = FromJSONintByName(CONSTANTS::MAX_CONNECTIONS, obj);
        if(port){
            _max_conn = max_conn.value();
        }
        auto rooms = FromJSONUsetByName(CONSTANTS::CHATROOMS,obj);
        if(rooms) {
            _roomlist = std::move(rooms.value());
        }
    }



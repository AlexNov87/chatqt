#include "structs.h"

//CONFIG INIT
    ConfigInit::ConfigInit() {

        str_type file_config = SelectConfigFile("Select your config file");
        //Если нет конфигурационного файла, останавливаем работу приложения.
        if(file_config.isEmpty()){
            Fatal("Can not load config file");
        }

        //Если не удалось прочитать конфигурационный файл,
        //То останавливаем работу приложения.
        json_obj configs = json::ReadJsonFromFileConfig(file_config);
        if(configs.empty()){
            Fatal("Check your JSON-FILE, may be something incorrect");
        }

        //Проверяем наличие валидность полей, которые необходимы для работы сервера.
        auto reason = json::CheckInitObject(configs);
        if(reason){
            Fatal(*reason);
        }
        this->_object = std::move(configs);
        CheckRoomField();
        CheckServerAllRight();
        CheckSQLAllRight();
    }


   void ConfigInit::CheckServerAllRight() {
       int port = _object.value(LOAD_CONSTANTS::SERVER_PORT).toInt();
       if(!IsCorrectPort(port)){
           Fatal("SERVER port is not valid");
       }

       int max_conn = _object.value(LOAD_CONSTANTS::MAX_CONNECTIONS).toInt();
       if(!IsCorrectConnections(max_conn)){
           Fatal("MaxConeections is not valid");
        }

       int mess_len = _object.value(LOAD_CONSTANTS::MAX_MESSAGE_LEN).toInt();
       if(mess_len < 0){
            Fatal("MaxLen is less than zero");
        }

        auto ip = _object.value(LOAD_CONSTANTS::SERVER_IP).toString();
        bool result = this->test_adress.setAddress(ip);
        if(!result) {
            Fatal("Server IP is not valid");
        }
    }


   void  ConfigInit::CheckSQLAllRight() {

       int port = _object.value(LOAD_CONSTANTS::DB_PORT).toInt();
       if(!IsCorrectPort(port)){
           FatalErrorMessageBox("SQL port is not valid");
       }
   }


   void  ConfigInit::CheckRoomField(){
       //Если нет поля , содержащее список комнат, то выходим.
       if(!_object.contains(str_type(LOAD_CONSTANTS::DEFAULT_CHATROOMS))) {return;}

       //Если поле, которое должно содержать список комнат, не массив,
       //то останавливаем работу приложения.
       if(!_object.value(LOAD_CONSTANTS::DEFAULT_CHATROOMS).isArray()){
             FatalErrorMessageBox("Message Field is not array");
             std::abort();
         }

         const auto roomlist = _object.value(LOAD_CONSTANTS::DEFAULT_CHATROOMS).toArray();
         //Если какой-нибудь элемент не строка, также останавливаем работу приложения.
         for (auto && room : roomlist){
             if(!room.isString()){
                 FatalErrorMessageBox("Member in room is not string");
                 std::abort();
             }
         }

     }

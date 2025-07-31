#include "answer_checker.h"


std::variant<json_obj, std::set<str_type>>
ServerAnswerChecker::CheckRoomListAnswer(const json_obj& obj){

    ACTIONS act = ACTIONS::GET_ROOMS_LIST;
    if(!obj.contains(CONSTANTS::LF_ROOMLIST)){
        return ans_obj::MakeErrorObject
            ("There is no roomlist field", act );
    }

    //пытаемся распарсить содержание комнат
    auto js_doc = json::ReadJsonFromQByte
        (obj.value(CONSTANTS::LF_ROOMLIST).toString().toUtf8());
    if(!js_doc){
        return ans_obj::MakeErrorObject
            ("There roomlist document is not json", act );
    }

    if(!js_doc->isArray()){
        return ans_obj::MakeErrorObject
            ("There field ROOMLIST is not array", act );
    }

    json_arr arr = js_doc->array();
    for(auto&& rm : arr){
        if(!rm.isString()){
            return ans_obj::MakeErrorObject
                ("The array element is not string", act );
        }
    }

    std::set<str_type> rooms;
    for(auto && rm : arr){
        rooms.insert(rm.toString());
    }
    std::variant<json_obj, std::set<str_type>> var(rooms);
    return var;
}



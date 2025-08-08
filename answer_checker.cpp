#include "answer_checker.h"

std::variant<json_obj, QStringList>
CheckTemplateArrayField(const json_obj& obj, const str_type& field_name,
ACTIONS act){

    if(!obj.contains(field_name)){
        return ans_obj::MakeErrorObject
            ("There is no field "+ field_name, act );
    }

    //пытаемся распарсить содержание поля
    auto js_doc = json::ReadJsonFromQByte
        (obj.value(field_name).toString().toUtf8());
    //Если не Json
    if(!js_doc){
        return ans_obj::MakeErrorObject
            ("There is " + field_name + " document is not json", act );
    }

    //Если не массив
    if(!js_doc->isArray()){
        return ans_obj::MakeErrorObject
            ("There field "+ field_name +" is not array", act );
    }

    //Если хоть один элемент не строка
    json_arr arr = js_doc->array();
    for(auto&& rm : arr){
        if(!rm.isString()){
            return ans_obj::MakeErrorObject
                ("The array element from " + field_name + " is not string", act );
        }
    }

    //Заполняеи значениями
    QStringList members;
    for(auto && am : arr){
        members.push_back(am.toString());
    }
    std::variant<json_obj, QStringList> var(members);
    return var;
}

std::variant<json_obj, QStringList>
ServerAnswerChecker::CheckRoomListAnswer(const json_obj& obj){
        return CheckTemplateArrayField
        (obj, CONSTANTS::LF_ROOMLIST, ACTIONS::GET_ROOMS_LIST);
}


 std::variant<json_obj, QStringList>
ServerAnswerChecker::CheckMembersRoomListAnswer(const json_obj& obj){

     static std::set<str_type> ch_fields{CONSTANTS::LF_ROOMNAME};
     auto reason = json::IsContainsFieldAndStringAndNotEmpty
         (obj, ch_fields);

     if(reason){
         return ans_obj::MakeErrorObject
             (*reason , ACTIONS::GET_ROOM_USERS);
     }
     return CheckTemplateArrayField
        (obj, CONSTANTS::LF_USERS, ACTIONS::GET_ROOM_USERS);
}


std::optional<json_obj>
ServerAnswerChecker::CheckJoinRoomAnswer(const json_obj& obj){
    static std::set<str_type>
    ch_fields{CONSTANTS::LF_LAST_MSG , CONSTANTS::LF_TOKEN};
    auto reason = json::IsContainsFieldAndStringAndNotEmpty(obj,ch_fields);
    if(reason){
        return ans_obj::MakeErrorObject(*reason, ACTIONS::LOGIN);
    }
    return std::nullopt;
}


std::optional<json_obj>
ServerAnswerChecker::CheckPrivateMessageAnswer(const json_obj& obj){
    static std::set<str_type>
        ch_fields{CONSTANTS::LF_NAME , CONSTANTS::LF_PRIVATE_MESSAGE};
    auto reason = json::IsContainsFieldAndStringAndNotEmpty(obj,ch_fields);
    if(reason){
        return ans_obj::MakeErrorObject(*reason, ACTIONS::PRIVATE_MESSAGE);
    }
    return std::nullopt;
}

std::optional<json_obj>
ServerAnswerChecker::CheckPublicMessageAnswer(const json_obj& obj){
    static std::set<str_type>
        ch_fields{CONSTANTS::LF_NAME , CONSTANTS::LF_PUBLIC_MESSAGE};
    auto reason = json::IsContainsFieldAndStringAndNotEmpty(obj,ch_fields);
    if(reason){
        return ans_obj::MakeErrorObject(*reason, ACTIONS::PUBLIC_MESSAGE);
    }
    return std::nullopt;
}




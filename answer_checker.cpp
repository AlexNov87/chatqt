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

std::variant<json_arr, json_obj> CheckHasFieldAndFieldIsArray
    (const json_obj& obj, const str_type&needed_fields,
    ADMIN_ACTIONS this_act){

    std::set<str_type> fields{
        needed_fields
    };
    //Если нет поля
    if(auto reason =
        json::IsContainsFieldAndStringAndNotEmpty(obj,fields)){
        return ans_obj::MakeAdminErrorObject(*reason,this_act);
    }

    const str_type& roomlist_str =
        obj.value(needed_fields).toString();

    //пытаемся преобразовать в json
    auto json_doc = json::ReadJsonFromQByte(roomlist_str.toUtf8());
    if(!json_doc) {
        return ans_obj::MakeAdminErrorObject
            ("Can not read json in " + needed_fields
                 + " field", this_act);
    }

    if(!(*json_doc).isArray()){
        return ans_obj::MakeAdminErrorObject("The json object " +
                                                 needed_fields + " is not array", this_act);
    }

    const json_arr& arr = (*json_doc).array();

    for(auto&& element : arr){
         if(!element.isObject()){

             return ans_obj::MakeAdminErrorObject("The element in json object " +
                                                      needed_fields +
                        " is not json_object", this_act);
         }
    }

    return arr;
}



std::variant<json_obj, std::map<str_type,str_type>>
ServerAnswerChecker::CheckAdminRooms(const json_obj& obj){

    ADMIN_ACTIONS this_act = ADMIN_ACTIONS::ROOM_LIST;
    const str_type needed_field = CONSTANTS::LF_ROOMLIST;

    auto object = CheckHasFieldAndFieldIsArray
        (obj, needed_field, this_act);

    if(std::holds_alternative<json_obj>(object)){
        return std::get<json_obj>(object);
    }
    json_arr array = std::get<json_arr>(object);

    static std::set<str_type> values {
       CONSTANTS::LF_ROOMNAME, CONSTANTS::LF_NAME};

    std::map<str_type, str_type> rooms_owners;

    for(auto && obj : array){
    const json_obj& objx =  obj.toObject();
    if(auto reason = json::IsContainsFieldAndStringAndNotEmpty
       (objx, values))
       {
        return ans_obj::MakeAdminErrorObject(*reason, this_act);
       }
        rooms_owners[objx.value(CONSTANTS::LF_ROOMNAME).toString()] =

           objx.value(CONSTANTS::LF_NAME).toString();
    };//for
    return rooms_owners;
}

std::variant<json_obj, std::map<str_type,str_type>>
ServerAnswerChecker::CheckAdminUsers(const json_obj& obj){

    ADMIN_ACTIONS this_act = ADMIN_ACTIONS::USER_LIST;
    const str_type needed_field = CONSTANTS::LF_USERS;

    auto object = CheckHasFieldAndFieldIsArray
        (obj, needed_field, this_act);

    if(std::holds_alternative<json_obj>(object)){
        return std::get<json_obj>(object);
    }
    json_arr array = std::get<json_arr>(object);

    static std::set<str_type> values {
            CONSTANTS::LF_ROLE, CONSTANTS::LF_NAME};

    std::map<str_type, str_type> user_role;

    for(auto && obj : array){
        const json_obj& objx =  obj.toObject();
        if(auto reason = json::IsContainsFieldAndStringAndNotEmpty
            (objx, values))
        {
            return ans_obj::MakeAdminErrorObject
                (*reason, this_act);
        }
        user_role[objx.value(CONSTANTS::LF_NAME).toString()] =

            objx.value(CONSTANTS::LF_ROLE).toString();
    };//for
    return user_role;

}




#include"structs_ch.h"

ChatRoom::ChatRoom(ServerBase* srv, str_type creator, str_type name) : _srv(srv),
    _creator(std::move(creator)), _name(std::move(name)) {
    assert(srv != nullptr);
}

json_obj ChatRoom::AddUser(std::shared_ptr<ChatUser> user){
    LGR(_mtx);
    if(_users.contains(user->GetName())){
        return ans_obj::MakeErrorObject
            ("User " + user->GetName() + " is already in room", ACTIONS::JOIN_ROOM);
    };
    _users[user->GetName()] = user;
    _tokens[user->GetToken()] = user;
    UpdateRoomMembersForAll();
    return ans_obj::SuccessJoinRoom(user->GetToken(),
    _manager.SerializedLastMessages());

}

json_obj ChatRoom::DeleteUserByName(const str_type& name){
    LGR(_mtx);
    if(!_users.contains(name)){
        return ans_obj::MakeErrorObject
            ("There is no user with name: "+ name, ACTIONS::DISCONNECT);
    }
     auto user = _users.at(name);
    _users.erase(name);
    _tokens.erase(user->GetToken());

    UpdateRoomMembersForAll();
    return ans_obj::SuccessDisconnect();
}

json_obj ChatRoom::DeleteUserByToken(const str_type& token){

    LGR(_mtx);
    if(!_tokens.contains(token)){
        return ans_obj::MakeErrorObject
            ("This room has not this token", ACTIONS::DISCONNECT);
    }
    auto user = _tokens.at(token);
    _users.erase(user->GetName());
    _tokens.erase(token);

    UpdateRoomMembersForAll();
    return ans_obj::SuccessDisconnect();
}


json_obj ChatRoom::PublicMessage(const str_type&token, str_type message){
    LGR(_mtx);
    if(auto err = MessageCheckErrorTemplate
        (token, message, ACTIONS::PUBLIC_MESSAGE)){
        return *err;
    }

    auto mess_author = _tokens.at(token);
    _manager.IncomePublicMessage(mess_author->GetName(), message);

    for (auto && user : _users){
        user.second->RecievePublicMessage
            (mess_author->GetName(),message);
    }
    return ans_obj::SuccessPublicMessage();
}

json_obj ChatRoom::PrivateMessage(const str_type&token,
                        const str_type& user_to,str_type message) {
    LGR(_mtx);
    if(auto err = MessageCheckErrorTemplate
        (token, message, ACTIONS::PRIVATE_MESSAGE)){
        return *err;
    }
    if(!_users.contains(user_to)){
        return ans_obj::MakeErrorObject
            ("There is no user with name: "+ user_to, ACTIONS::PRIVATE_MESSAGE);
    }

    auto mess_author = _tokens.at(token);
    auto mess_reciever = _users.at(user_to);
    mess_reciever->RecievePrivateMessage
        (mess_author->GetName(), std::move(message));

    return ans_obj::SuccessPrivateMessage();
}


const str_type& ChatRoom::GetCreator() const {
    return _creator;
}

str_type ChatRoom::SerializatedJsonUsers() const {
    LGR(_mtx);
    return json::GetMapMembersJsonArrayView(_users);
}

std::optional<json_obj> ChatRoom::MessageCheckErrorTemplate
    (const str_type&token,const str_type& message, ACTIONS action){

    if(!_tokens.contains(token)){
        return ans_obj::MakeErrorObject
            ( "Your token is not in room." , action);
    }

    if(message.size() > 512){
        return ans_obj::MakeErrorObject
            ( "Your message is too long" , action);
    }
    return std::nullopt;
}

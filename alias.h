#ifndef ALIAS_H
#define ALIAS_H
using chart = const char*;
#define json_obj QJsonObject
#define json_arr QJsonArray
#define str_type QString

#define LG(mutex_name) std::lock_guard<std::mutex> lg(mutex_name)
#define shptr(name , type) std::shared_ptr<type> name = std::make_shared<type>


#endif // ALIAS_H


/*
{
"IP_SERVER" : "127.1.0.1" ,
"SERVER_PORT" : 80,
"MAX_CONNECTIONS" : 200000,
"DEFAULT_CHATROOMS" : [ "R1", "R2", "R3"],
"MAX_MESSAGE_LEN" : 512,

"DB_HOST" : "127.0.0.1",
"DB_PASSWORD" : "12345",
"DB_PORT" : 5432,
"DB_USER" : "postgres",
"DB_TABLE_NAME" : "chattable"

}
*/

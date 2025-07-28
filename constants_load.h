#ifndef CONSTANTS_LOAD_H
#define CONSTANTS_LOAD_H
#include "alias.h"

struct LOAD_CONSTANTS
{
    //C
    static chart DEFAULT_CHATROOMS;
    //D
    static chart DB_HOST;
    static chart DB_PASSWORD;
    static chart DB_PORT;
    static chart DB_TABLE_NAME;
    static chart DB_USER;
    //M
    static chart MAX_CONNECTIONS ;
    static chart MAX_MESSAGE_LEN ;
    //S
    static chart SERVER_IP;
    static chart SERVER_PORT;

private:
    LOAD_CONSTANTS(){}
};
#endif // CONSTANTS_LOAD_H



/**
 * @file answer_obj.h
 * @brief Заголовочный файл с функциями-фабриками для создания JSON-ответов.
 *
 * Этот файл содержит набор вспомогательных функций, которые упрощают
 * создание стандартизированных JSON-объектов для ответов сервера.
 * Функции сгруппированы по типу действий: ошибки, успешные ответы и
 * входящие сообщения.
 */

#ifndef ANSWER_OBJ_H
#define ANSWER_OBJ_H

#include "constants.h"
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <stdexcept> // Для std::exception

namespace ans_obj {

//-------------------------------------------------------------------------
// Функции для административных ответов
//-------------------------------------------------------------------------

/**
 * @brief Создает JSON-объект ошибки для административного действия.
 * @param reason Причина ошибки.
 * @param act Тип административного действия, в котором произошла ошибка.
 * @return QJsonObject с полями для ошибки.
 */
json_obj MakeAdminErrorObject(str_type reason, ADMIN_ACTIONS act);

/**
 * @brief Создает JSON-объект успешного создания комнаты.
 * @param name Имя созданной комнаты.
 * @return QJsonObject, подтверждающий успешное создание.
 */
json_obj AdminSuccessCreateRoom(str_type name);

/**
 * @brief Создает JSON-объект успешного удаления комнаты.
 * @param name Имя удаленной комнаты.
 * @return QJsonObject, подтверждающий успешное удаление.
 */
json_obj AdminSuccessDeleteRoom(str_type name);

/**
 * @brief Создает JSON-объект успешного удаления пользователя.
 * @param name Имя удаленного пользователя.
 * @return QJsonObject, подтверждающий успешное удаление.
 */
json_obj AdminSuccessDeleteUser(str_type name);

/**
 * @brief Создает JSON-объект успешной блокировки пользователя.
 * @return QJsonObject, подтверждающий успешную блокировку.
 */
json_obj AdminSuccessBanUser();

/**
 * @brief Создает JSON-объект успешной разблокировки пользователя.
 * @return QJsonObject, подтверждающий успешную разблокировку.
 */
json_obj AdminSuccessUnbanUser();

/**
 * @brief Создает JSON-объект успешного обновления роли пользователя.
 * @return QJsonObject, подтверждающий успешное обновление роли.
 */
json_obj AdminSuccessUpdateRoleUser();

/**
 * @brief Создает JSON-объект со списком пользователей для администратора.
 * @param usetlist Сериализованный список пользователей.
 * @return QJsonObject со списком.
 */
json_obj AdminUserList(str_type usetlist);

/**
 * @brief Создает JSON-объект со списком комнат для администратора.
 * @param roomlist Сериализованный список комнат.
 * @return QJsonObject со списком.
 */
json_obj AdminRoomList(str_type roomlist);

/**
 * @brief Шаблонная функция-обёртка для обработки исключений.
 *
 * Эта функция вызывает переданный функтор и ловит все исключения `std::exception`,
 * возвращая JSON-объект ошибки в случае исключения.
 *
 * @tparam Foo Тип функтора.
 * @param foo Функтор, который нужно выполнить.
 * @param this_act Административное действие.
 * @return QJsonObject, созданный функтором, или объект ошибки в случае исключения.
 */
template<typename Foo>
json_obj GuardExceptSetter(Foo foo, ADMIN_ACTIONS this_act){
    try{
        return foo();
    }
    catch(const std::exception& ex){
        return ans_obj::MakeAdminErrorObject
            ("Exception:" + QString(ex.what()), this_act);
    }
    return {};
}

} // namespace ans_obj

namespace ans_obj {

//-------------------------------------------------------------------------
// Функции для стандартных ответов и сообщений
//-------------------------------------------------------------------------

/**
 * @brief Создает JSON-объект ошибки для стандартного действия.
 * @param reason Причина ошибки.
 * @param act Тип стандартного действия, в котором произошла ошибка.
 * @return QJsonObject с полями для ошибки.
 */
json_obj MakeErrorObject(str_type reason, ACTIONS act);

/**
 * @brief Создает шаблонный JSON-объект успешного ответа.
 * @param act Тип действия.
 * @return QJsonObject с полем "result": "success".
 */
json_obj MakeSuccessTemplate(ACTIONS act);

/**
 * @brief Создает JSON-объект успешного создания пользователя.
 * @param name Имя созданного пользователя.
 * @return QJsonObject, подтверждающий успешное создание.
 */
json_obj SuccessCreateUser(str_type name);

/**
 * @brief Создает JSON-объект успешного отключения.
 * @return QJsonObject, подтверждающий успешное отключение.
 */
json_obj SuccessDisconnect();

/**
 * @brief Создает JSON-объект со списком пользователей в комнате.
 * @param roomname Имя комнаты.
 * @param users_serialized Сериализованный список пользователей.
 * @return QJsonObject со списком пользователей.
 */
json_obj SuccessRoomUsers (str_type roomname, str_type users_serialized);

/**
 * @brief Создает JSON-объект успешного входа в систему.
 * @return QJsonObject, подтверждающий успешный вход.
 */
json_obj SuccessLogin();

/**
 * @brief Создает JSON-объект со списком комнат.
 * @param rooms_serialized Сериализованный список комнат.
 * @return QJsonObject со списком.
 */
json_obj SuccessServerRooms (str_type rooms_serialized);

/**
 * @brief Создает JSON-объект успешного входа в комнату.
 * @param token Токен доступа для комнаты.
 * @param messages_serialized Сериализованные старые сообщения.
 * @return QJsonObject с токеном и сообщениями.
 */
json_obj SuccessJoinRoom(str_type token, str_type messages_serialized);

/**
 * @brief Создает JSON-объект успешной отправки личного сообщения.
 * @return QJsonObject, подтверждающий успешную отправку.
 */
json_obj SuccessPrivateMessage();

/**
 * @brief Создает JSON-объект успешной отправки публичного сообщения.
 * @return QJsonObject, подтверждающий успешную отправку.
 */
json_obj SuccessPublicMessage();

/**
 * @brief Создает JSON-объект входящего публичного сообщения.
 * @param author Автор сообщения.
 * @param msg Текст сообщения.
 * @return QJsonObject, представляющий входящее публичное сообщение.
 */
json_obj IncomePublicMessage(const str_type author,str_type msg );

/**
 * @brief Создает JSON-объект входящего личного сообщения.
 * @param author Автор сообщения.
 * @param msg Текст сообщения.
 * @return QJsonObject, представляющий входящее личное сообщение.
 */
json_obj IncomePrivateMessage(const str_type author,str_type msg);

/**
 * @brief Тестовая функция, возвращающая пустой JSON-объект.
 * @return Пустой QJsonObject.
 */
json_obj TestObject();

/**
 * @brief Шаблонная функция-обёртка для обработки исключений в стандартных действиях.
 *
 * Эта функция вызывает переданный функтор и ловит все исключения `std::exception`,
 * возвращая JSON-объект ошибки в случае исключения.
 *
 * @tparam Foo Тип функтора.
 * @param foo Функтор, который нужно выполнить.
 * @param this_act Стандартное действие.
 * @return QJsonObject, созданный функтором, или объект ошибки в случае исключения.
 */
template<typename Foo>
json_obj GuardExceptSetter(Foo foo, ACTIONS this_act){
    try{
        return foo();
    }
    catch(const std::exception& ex){
        return ans_obj::MakeErrorObject
            ("Exception:" + QString(ex.what()), this_act);
    }
    return {};
}
} // namespace ans_obj


#endif // ANSWER_OBJ_H

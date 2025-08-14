/**
 * @file constants.h
 * @brief Заголовочный файл с глобальными константами, перечислениями и мапами.
 *
 * Этот файл определяет ключевые строковые константы, перечисления для
 * действий пользователей и администраторов, а также структурные типы
 * и глобальные мапы для преобразования между строковыми представлениями
 * и перечислениями.
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "alias.h"
#include <map>
#include <set>
#include <QString>

/**
 * @struct CONSTANTS
 * @brief Класс, содержащий статические строковые константы.
 *
 * Этот класс-контейнер объединяет все строковые константы,
 * используемые для взаимодействия в протоколе приложения (например,
 * имена действий, ключи в JSON-сообщениях и т.д.).
 */
struct CONSTANTS
{
    //-------------------------------------------------------------------------
    // Действия (Actions)
    //-------------------------------------------------------------------------
    static chart ACT_CREATE_USER;            /**< Ключ для действия "создать пользователя". */
    static chart ACT_DISCONNECT;             /**< Ключ для действия "отключиться". */
    static chart ACT_EARLIER_MESS;           /**< Ключ для действия "получить более ранние сообщения". */
    static chart ACT_GET_USERS;              /**< Ключ для действия "получить список пользователей". */
    static chart ACT_INCOME_PRIVATE;         /**< Ключ для входящего личного сообщения. */
    static chart ACT_INCOME_PUBLIC;          /**< Ключ для входящего публичного сообщения. */
    static chart ACT_JOIN_ROOM;              /**< Ключ для действия "войти в комнату". */
    static chart ACT_LOGIN;                  /**< Ключ для действия "вход в систему". */
    static chart ACT_ROOM_LIST;              /**< Ключ для действия "получить список комнат". */
    static chart ACT_SEND_PRIVATE_MESSAGE;   /**< Ключ для действия "отправить личное сообщение". */
    static chart ACT_SEND_PUBLIC_MESSAGE;    /**< Ключ для действия "отправить публичное сообщение". */
    static chart ACT_SYSTEM;                 /**< Ключ для системного действия. */

    //-------------------------------------------------------------------------
    // Действия администратора (Admins)
    //-------------------------------------------------------------------------
    static chart ADMIN_ACT_BAN_USER;         /**< Ключ для действия "заблокировать пользователя". */
    static chart ADMIN_ACT_CREATE_ROOM;      /**< Ключ для действия "создать комнату". */
    static chart ADMIN_ACT_DELETE_USER;      /**< Ключ для действия "удалить пользователя". */
    static chart ADMIN_ACT_DELETE_ROOM;      /**< Ключ для действия "удалить комнату". */
    static chart ADMIN_ACT_FIND_USERS;       /**< Ключ для действия "найти пользователей". */
    static chart ADMIN_ACT_SYSTEM;           /**< Ключ для системных действий администратора. */
    static chart ADMIN_ACT_UNBAN_USER;       /**< Ключ для действия "разблокировать пользователя". */
    static chart ADMIN_ACT_UPDATE_ROLE;      /**< Ключ для действия "обновить роль пользователя". */
    static chart ADMIN_ACT_USER_LIST;        /**< Ключ для действия "получить список пользователей". */
    static chart ADMIN_ACT_USERLIST_PREDICATE; /**< Ключ для действия "получить список пользователей с предикатом". */
    static chart ADMIN_ACT_ROOM_LIST;        /**< Ключ для действия "получить список комнат (админ)". */


    //-------------------------------------------------------------------------
    // Ключи JSON-объектов (Map Parameters)
    //-------------------------------------------------------------------------
    static chart LF_ACTION;                  /**< Ключ для поля "действие". */
    static chart LF_DIRECTION;               /**< Ключ для поля "направление". */
    static chart LF_INITIATOR;               /**< Ключ для поля "инициатор". */
    static chart LF_LAST_MSG;                /**< Ключ для поля "последнее сообщение". */
    static chart LF_LOGIN;                   /**< Ключ для поля "логин". */
    static chart LF_PRIVATE_MESSAGE;         /**< Ключ для поля "личное сообщение". */
    static chart LF_PUBLIC_MESSAGE;          /**< Ключ для поля "публичное сообщение". */
    static chart LF_NAME;                    /**< Ключ для поля "имя". */
    static chart LF_PASSWORD;                /**< Ключ для поля "пароль". */
    static chart LF_REASON;                  /**< Ключ для поля "причина". */
    static chart LF_RESULT;                  /**< Ключ для поля "результат". */
    static chart LF_ROLE;                    /**< Ключ для поля "роль". */
    static chart LF_ROOMLIST;                /**< Ключ для поля "список комнат". */
    static chart LF_ROOMNAME;                /**< Ключ для поля "имя комнаты". */
    static chart LF_TOKEN;                   /**< Ключ для поля "токен". */
    static chart LF_USERS;                   /**< Ключ для поля "пользователи". */
    static chart LF_USER_RECIEVER;           /**< Ключ для поля "получатель". */
    static chart LF_VALUE;                   /**< Ключ для поля "значение". */

    //-------------------------------------------------------------------------
    // Результаты и специальные значения
    //-------------------------------------------------------------------------
    static chart RF_ERROR;                   /**< Значение для результата "ошибка". */
    static chart RF_SUCCESS;                 /**< Значение для результата "успех". */
    static chart RF_THREEDOTS;               /**< Значение для значения "троеточие". */

    static chart RF_ERR_PERMISSION_DENIDED;  /**< Значение для ошибки "в доступе отказано". */

    static chart RF_DIRECTION_SERVER;        /**< Значение для направления "сервер". */
    static chart RF_DIRECTION_CHATROOM;      /**< Значение для направления "чат-комната". */
    static chart UNKNOWN;                    /**< Значение для неизвестного значения. */

    static const char SERIAL_SYM;            /**< Символ-разделитель для сериализации. */

    //-------------------------------------------------------------------------
    // Роли пользователей
    //-------------------------------------------------------------------------
    static chart ROLE_MASTER;                /**< Значение для роли "мастер". */
    static chart ROLE_ADMINISTRATOR;         /**< Значение для роли "администратор". */
    static chart ROLE_SUPERUSER;             /**< Значение для роли "суперпользователь". */
    static chart ROLE_USER;                  /**< Значение для роли "пользователь". */

private:
    CONSTANTS() {} /**< @private Закрытый конструктор для предотвращения создания экземпляров. */
};

//-----------------------------------------------------------------------------
// Перечисления (Enums)
//-----------------------------------------------------------------------------

/**
 * @enum ACTIONS
 * @brief Перечисление для стандартных действий пользователя.
 */
enum class ACTIONS {
    LOGIN,
    DISCONNECT,
    CREATE_USER,
    PRIVATE_MESSAGE,
    PUBLIC_MESSAGE,
    GET_ROOM_USERS,
    GET_ROOMS_LIST,
    JOIN_ROOM,
    SYSTEM,
    INCOME_PUBLIC,
    INCOME_PRIVATE
};

/**
 * @enum ADMIN_ACTIONS
 * @brief Перечисление для действий администратора.
 */
enum class ADMIN_ACTIONS {
    BAN_USER,
    UNBAN_USER,
    DELETE_USER,
    UPDATE_ROLE,
    CREATE_ROOM,
    DELETE_ROOM,
    USER_LIST,
    ROOM_LIST,
    USERLIST_PREDICATE,
    SYSTEM
};

/**
 * @enum Role
 * @brief Перечисление для ролей пользователей.
 */
enum class Role {
    USER,
    SUPERUSER,
    ADMIN,
    MASTER
};

/**
 * @struct UserRole
 * @brief Структура, описывающая роль пользователя.
 */
struct UserRole{
    str_type password; /**< Хешированный пароль пользователя. */
    Role role;         /**< Роль пользователя. */
    bool is_active;    /**< Флаг, указывающий, активен ли пользователь. */
};

//-----------------------------------------------------------------------------
// Глобальные мапы и сеты
//-----------------------------------------------------------------------------

/**
 * @brief Мапа для преобразования имени действия в enum ACTIONS.
 * @details Используется для быстрого поиска типа действия по его строковому представлению.
 */
extern const std::map<str_type, ACTIONS> _NAME_ACTION;

/**
 * @brief Мапа для преобразования enum ACTIONS в его строковое имя.
 * @details Используется для получения строкового представления действия по его enum-типу.
 */
extern const std::map<ACTIONS, str_type> _ACTION_NAME;

/**
 * @brief Сет, содержащий имена действий, обрабатываемых сервером.
 */
extern const std::set<str_type> _ACT_SERVER;

/**
 * @brief Сет, содержащий имена действий, инициируемых пользователем.
 */
extern const std::set<str_type> _ACT_USER;

/**
 * @brief Мапа для преобразования имени административного действия в enum ADMIN_ACTIONS.
 */
extern const std::map<str_type, ADMIN_ACTIONS> _NAME_ADMIN_ACTION;

/**
 * @brief Мапа для преобразования enum ADMIN_ACTIONS в его строковое имя.
 */
extern const std::map<ADMIN_ACTIONS, str_type> _ACTION_ADMIN_NAME;

/**
 * @brief Сет, содержащий имена административных действий.
 */
extern const std::set<str_type> _ACT_ADMIN;

/**
 * @brief Мапа для преобразования имени роли в enum Role.
 */
extern const std::map<str_type, Role> _NAME_ROLE;

/**
 * @brief Мапа для преобразования enum Role в его строковое имя.
 */
extern const std::map<Role, str_type> _ROLE_NAME;

#endif // CONSTANTS_H

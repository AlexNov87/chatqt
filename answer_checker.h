/**
 * @file answer_checker.h
 * @brief Объявление класса ServerAnswerChecker и связанных функций.
 *
 * Этот файл содержит объявление класса ServerAnswerChecker,
 * который предоставляет статические методы для проверки и валидации
 * различных JSON-ответов от сервера.
 */

#ifndef ANSWER_CHECKER_H
#define ANSWER_CHECKER_H

#include "initializators_help.h"
#include "answer_obj.h"
#include <set>
#include <variant>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>

/**
 * @class ServerAnswerChecker
 * @brief Класс-утилита для проверки ответов, полученных от сервера.
 *
 * Все методы этого класса являются статическими. Они принимают JSON-объект
 * ответа и проверяют его на соответствие ожидаемой структуре, возвращая
 * либо валидированные данные, либо объект с описанием ошибки.
 */
class ServerAnswerChecker {
public:
    /**
     * @brief Проверяет ответ сервера со списком комнат.
     * @param obj JSON-объект ответа.
     * @return variant, содержащий либо QStringList со списком комнат,
     * либо json_obj с описанием ошибки.
     */
    static std::variant<json_obj, QStringList>
    CheckRoomListAnswer(const json_obj& obj);

    /**
     * @brief Проверяет ответ со списком участников комнаты.
     * @param obj JSON-объект ответа.
     * @return variant, содержащий либо QStringList со списком участников,
     * либо json_obj с описанием ошибки.
     */
    static std::variant<json_obj, QStringList>
    CheckMembersRoomListAnswer(const json_obj& obj);

    /**
     * @brief Проверяет ответ на попытку входа в комнату.
     * @param obj JSON-объект ответа.
     * @return optional, содержащий json_obj с ошибкой, если валидация не пройдена,
     * иначе std::nullopt.
     */
    static std::optional<json_obj>
    CheckJoinRoomAnswer(const json_obj& obj);

    /**
     * @brief Проверяет входящее личное сообщение.
     * @param obj JSON-объект сообщения.
     * @return optional, содержащий json_obj с ошибкой, если валидация не пройдена,
     * иначе std::nullopt.
     */
    static std::optional<json_obj>
    CheckPrivateMessageAnswer(const json_obj& obj);

    /**
     * @brief Проверяет входящее публичное сообщение.
     * @param obj JSON-объект сообщения.
     * @return optional, содержащий json_obj с ошибкой, если валидация не пройдена,
     * иначе std::nullopt.
     */
    static std::optional<json_obj>
    CheckPublicMessageAnswer(const json_obj& obj);

    /**
     * @brief Проверяет административный ответ со списком комнат.
     * @param obj JSON-объект ответа.
     * @return variant, содержащий либо map<str_type, str_type> с парами
     * "имя комнаты" -> "владелец", либо json_obj с описанием ошибки.
     */
    static std::variant<json_obj, std::map<str_type, str_type>>
    CheckAdminRooms(const json_obj& obj);

    /**
     * @brief Проверяет административный ответ со списком пользователей.
     * @param obj JSON-объект ответа.
     * @return variant, содержащий либо map<str_type, str_type> с парами
     * "имя пользователя" -> "роль", либо json_obj с описанием ошибки.
     */
    static std::variant<json_obj, std::map<str_type, str_type>>
    CheckAdminUsers(const json_obj& obj);
};

#endif // ANSWER_CHECKER_H

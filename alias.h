/**
 * @file alias.h
 * @brief Заголовочный файл с определениями алиасов и макросов.
 *
 * Этот файл содержит удобные псевдонимы типов (type aliases) и макросы
 * для упрощения работы с такими классами, как `QJson` и потокобезопасными
 * операциями.
 */

#ifndef ALIAS_H
#define ALIAS_H

                                                                                                             /**
 * @typedef chart
 * @brief Псевдоним для константного указателя на char (`const char*`).
 */
using chart = const char*;

/**
 * @def json_obj
 * @brief Макрос для QJsonObject.
 * @details Используется для сокращения имени класса QJsonObject, что делает
 * код более лаконичным при работе с JSON-объектами.
 */
#define json_obj QJsonObject

/**
 * @def json_arr
 * @brief Макрос для QJsonArray.
 * @details Используется для сокращения имени класса QJsonArray, что делает
 * код более лаконичным при работе с JSON-массивами.
 */
#define json_arr QJsonArray

/**
 * @def str_type
 * @brief Макрос для QString.
 * @details Используется как псевдоним для класса QString.
 */
#define str_type QString

//-----------------------------------------------------------------------------
// Макросы для управления блокировками (mutex)
//-----------------------------------------------------------------------------

/**
 * @def LG(mutex_name)
 * @brief Макрос для создания std::lock_guard.
 * @param mutex_name Имя мьютекса, который нужно заблокировать.
 * @details Этот макрос упрощает создание `std::lock_guard` для
 * автоматической блокировки и разблокировки обычного мьютекса.
 */
#define LG(mutex_name) std::lock_guard<std::mutex> lg(mutex_name)

/**
 * @def LGR(mutex_name)
 * @brief Макрос для создания std::lock_guard для рекурсивного мьютекса.
 * @param mutex_name Имя рекурсивного мьютекса, который нужно заблокировать.
 * @details Этот макрос упрощает создание `std::lock_guard` для
 * автоматической блокировки и разблокировки рекурсивного мьютекса.
 */
#define LGR(mutex_name) std::lock_guard<std::recursive_mutex> lg(mutex_name)

/**
 * @def shptr(name, type)
 * @brief Макрос для создания std::shared_ptr.
 * @param name Имя создаваемого указателя.
 * @param type Тип объекта, на который будет указывать shared_ptr.
 * @details Этот макрос упрощает синтаксис создания `std::shared_ptr` с помощью `std::make_shared`.
 */
#define shptr(name, type) std::shared_ptr<type> name = std::make_shared<type>

#endif // ALIAS_H

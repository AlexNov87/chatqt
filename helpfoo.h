/**
 * @file helpfoo.h
 * @brief Заголовочный файл с набором вспомогательных функций и утилит.
 *
 * Этот файл содержит объявления функций, шаблонов и пространств имён,
 * которые упрощают повседневные задачи, такие как работа с диалоговыми
 * окнами, файлами, JSON, сокетами и многопоточностью.
 */

#ifndef HELPFOO_H
#define HELPFOO_H

#include "answer_obj.h"
#include <thread>
#include <mutex>
#include <QString>
#include <QMessageBox>
#include <QFileDialog>
#include <unordered_set>
#include <QFile>
#include <QHash>
#include <QTcpSocket>
#include <ranges>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>

//-------------------------------------------------------------------------
// Функции для работы с диалоговыми окнами
//-------------------------------------------------------------------------

/**
 * @brief Открывает диалог для выбора файла конфигурации.
 * @param comment Текст, отображаемый в заголовке диалогового окна.
 * @return QString с путём к выбранному файлу или пустая строка, если файл не выбран.
 */
QString SelectConfigFile(QString comment);

/**
 * @brief Отображает модальное критическое сообщение об ошибке.
 * @param what Текст сообщения об ошибке.
 * @param framename Заголовок окна сообщения.
 */
void FatalErrorMessageBox(const QString& what, QString framename = "Error");

/**
 * @brief Отображает диалоговое окно с вопросом "Да/Нет".
 * @param question Текст вопроса.
 * @return true, если пользователь нажал "Да", иначе false.
 */
bool ChooseBox(const QString& question);

//-------------------------------------------------------------------------
// Пространство имён для работы с JSON
//-------------------------------------------------------------------------
namespace json {

/**
 * @brief Считывает JSON-документ из файла.
 * @param path Путь к файлу.
 * @return QJsonDocument, если чтение успешно, иначе пустой документ.
 */
QJsonDocument ReadJsonDocument(const QString &path);

/**
 * @brief Считывает JSON-документ из QByteArray.
 * @param array QByteArray, содержащий JSON-данные.
 * @return optional с QJsonDocument, если чтение успешно, иначе std::nullopt.
 */
std::optional<QJsonDocument> ReadJsonFromQByte(QByteArray array);

/**
 * @brief Считывает JSON-объект из файла конфигурации.
 * @param path Путь к файлу.
 * @return QJsonObject, если чтение успешно, иначе пустой объект.
 */
QJsonObject ReadJsonFromFileConfig(const QString& path);

/**
 * @brief Шаблонная функция для сохранения JSON-объекта в файл.
 *
 * Открывает диалог сохранения файла и записывает в него JSON-данные.
 * В случае успеха отображает информационное сообщение.
 *
 * @tparam JSON Тип JSON-объекта (QJsonObject или QJsonArray).
 * @param json Объект для сохранения.
 */
template<typename JSON>
void WritetoFileJson(JSON& json){
    QString write_pth = QFileDialog::getSaveFileName
        (nullptr, "Save JSON To file", QDir::currentPath(), "*.txt;*.conf;*.json");
    if(write_pth.isEmpty()){
        return;
    }
    QFile file(write_pth);
    if(!file.open(QIODevice::WriteOnly)){
        FatalErrorMessageBox("Can not open file to write");
        return;
    }
    QJsonDocument doc(json);
    file.write(doc.toJson());
    file.close();
    QMessageBox::information(nullptr, "SaveSucess", "File successfully was writen to: " + write_pth);
}

/**
 * @brief Шаблонная функция для преобразования JSON-объекта в QByteArray.
 * @tparam JSON Тип JSON-объекта (QJsonObject или QJsonArray).
 * @param json Объект для преобразования.
 * @return QByteArray, содержащий JSON-данные.
 */
template<typename JSON>
QByteArray WritetoQByteAnyJson(const JSON& json){
    QJsonDocument doc(json);
    return doc.toJson();
}

/**
 * @brief Шаблонная функция для сериализации ключей map в JSON-массив.
 * @tparam K Тип ключа map.
 * @tparam U Тип значения map.
 * @tparam MapType Тип контейнера (например, std::map).
 * @param map Контейнер, ключи которого нужно сериализовать.
 * @return QString, содержащий JSON-массив ключей.
 */
template<typename K, typename U, template<typename, typename> class MapType>
QString GetMapMembersJsonArrayView (const MapType<K, U>& map){
    QJsonArray arr;
    for(auto&& elem : map){ arr.append(elem.first);}
    return QString(WritetoQByteAnyJson(arr));
}

/**
 * @brief Проверяет, является ли JSON-объект объектом ошибки.
 *
 * Объект считается ошибкой, если он содержит поле "result" со значением "error".
 *
 * @param obj Проверяемый QJsonObject.
 * @return true, если это объект ошибки, иначе false.
 */
bool IsErrorJsonObject(const json_obj& obj);

/**
 * @brief Считывает JSON-объект из QByteArray.
 * @param arr QByteArray, содержащий данные.
 * @return QJsonObject, если данные валидны, иначе объект ошибки.
 */
json_obj ReadJsonObjectFromQbyteArray(QByteArray& arr);

} // namespace json

//-------------------------------------------------------------------------
// Другие вспомогательные функции
//-------------------------------------------------------------------------

/**
 * @brief Запускает функцию в отдельном потоке.
 *
 * Использует `std::jthread`, что обеспечивает автоматическое завершение потока.
 *
 * @tparam Foo Тип функтора.
 * @param foo Функтор, который нужно выполнить в потоке.
 */
template<typename Foo>
void AnotherThreadRunFoo(Foo foo){
    std::jthread jth ([=]{
        foo();
    });
}

/**
 * @brief Записывает данные в сокет и немедленно отправляет их.
 * @param socket Указатель на сокет.
 * @param arr Данные для отправки.
 */
void WriteToSocketWithFlush(QTcpSocket* socket, const QByteArray& arr);

/**
 * @brief Записывает данные в сокет с добавлением символа-разделителя.
 * @param socket Указатель на сокет.
 * @param arr Данные для отправки.
 */
void WriteToSocketWithFlushAddingSplitSym(QTcpSocket* socket, QByteArray& arr);

/**
 * @brief Отображает критическое сообщение и прерывает выполнение программы.
 * @param str Сообщение об ошибке.
 */
void Fatal(const str_type& str);

/**
 * @brief Вычисляет хеш для строки.
 * @param str Строка для хеширования.
 * @return Беззнаковое целое число - хеш.
 */
unsigned int PassHash(const str_type& str);

/**
 * @brief Создает неблокирующее окно с сообщением об ошибке.
 * @param obj JSON-объект, содержащий поля "initiator" и "reason".
 */
void NonBlockingErrorBox(const json_obj& obj);

#endif // HELPFOO_H

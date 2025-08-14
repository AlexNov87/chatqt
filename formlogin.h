/**
 * @file formlogin.h
 * @brief Объявление класса FormLogin для формы входа.
 */

#ifndef FORMLOGIN_H
#define FORMLOGIN_H
#include "structs.h"
#include <QDialog>
#include <QPushButton>

namespace Ui {
class FormLogin;
}

/**
 * @class FormLogin
 * @brief Диалоговое окно для ввода имени пользователя и пароля.
 *
 * Класс наследуется от QDialog и NamePassword. Он предоставляет
 * графический интерфейс для аутентификации и валидации вводимых данных.
 */
class FormLogin : public QDialog , public NamePassword
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор FormLogin.
     * @param label Текст, который будет отображаться в заголовке формы.
     */
    explicit FormLogin(const str_type& label);

    /**
     * @brief Деструктор класса FormLogin.
     */
    ~FormLogin();

private slots:
    /**
     * @brief Слот, проверяющий введённые данные перед закрытием диалога.
     *
     * Эта функция выполняет валидацию полей имени и пароля,
     * проверяя их на пустоту и максимальную длину.
     * При успешной проверке данные сохраняются и вызывается accept().
     */
    void CheckToAccept();

private:
    Ui::FormLogin *ui; /**< Указатель на объект UI-формы, сгенерированный Qt. */
};

#endif // FORMLOGIN_H

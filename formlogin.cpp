/**
 * @file formlogin.cpp
 * @brief Реализация класса FormLogin.
 */
#include "formlogin.h"
#include "ui_formlogin.h"

/**
 * @brief Конструктор класса FormLogin.
 *
 * Инициализирует UI, настраивает кнопку OK для вызова слота валидации,
 * и устанавливает текст заголовка формы.
 *
 * @param label Текст заголовка, который будет отображён в `lbl_header`.
 */
FormLogin::FormLogin(const str_type& label)
    : QDialog(nullptr)
    , ui(new Ui::FormLogin)
{
    ui->setupUi(this);
    QPushButton* okButton = ui->bb_choose->button(QDialogButtonBox::Ok);
    connect(okButton, &QPushButton::clicked, this, &FormLogin::CheckToAccept);
    ui->lbl_header->setText(label);
}

/**
 * @brief Деструктор FormLogin.
 *
 * Освобождает память, выделенную для объекта UI.
 */
FormLogin::~FormLogin()
{
    delete ui;
}

/**
 * @brief Слот для валидации данных.
 *
 * Эта функция вызывается при нажатии кнопки OK.
 * 1. Получает имя пользователя и пароль из полей ввода.
 * 2. Проверяет, не пусты ли поля. В случае ошибки отображает сообщение.
 * 3. Проверяет длину имени и пароля, ограничивая их 49 символами.
 * 4. При успешной валидации сохраняет имя, хеширует пароль и сохраняет его,
 * после чего закрывает диалог с кодом QDialog::Accepted.
 */
void FormLogin::CheckToAccept(){
    str_type nm = ui->le_admin_name->text();
    str_type pw1 = ui->le_password ->text();

    if(nm.isEmpty() || pw1.isEmpty()){
        FatalErrorMessageBox("One ore more fields are empty");
        return;
    }

    if(nm.size() > 49){
        FatalErrorMessageBox("Name is too long (max 49)");
        return;
    }

    if(pw1.size() > 49){
        FatalErrorMessageBox("Password is too long (max 49)");
        return;
    }

    _name = std::move(nm);
    unsigned int hsh = PassHash(pw1);
    _pass = QString::number(hsh);
    accept();
}

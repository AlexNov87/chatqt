#ifndef FORMLOGIN_H
#define FORMLOGIN_H
#include "structs.h"
#include <QDialog>
#include <QPushButton>

namespace Ui {
class FormLogin;
}

class FormLogin : public QDialog , public NamePassword
{
    Q_OBJECT

public:
    explicit FormLogin(const str_type& label);
    ~FormLogin();

private slots:
    void CheckToAccept();

private:
    Ui::FormLogin *ui;


};

#endif // FORMLOGIN_H

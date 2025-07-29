#ifndef FORMMASTER_H
#define FORMMASTER_H

#include <QDialog>
#include "helpfoo.h"

namespace Ui {
class Formmaster;
}

class Formmaster : public QDialog
{
    Q_OBJECT

public:
    explicit Formmaster();
    ~Formmaster();

    const str_type& AdminName(){
        return _admin_name;
    }

    const str_type& AdminPassword(){
        return _admin_pass;
    }

private slots:
    void on_pb_create_master_clicked();

private:
    Ui::Formmaster *ui;
    void SetOkState(bool state);
    void TurnOffOk();
    void TurnOnOk();

    str_type _admin_name;
    str_type _admin_pass;
};

#endif // FORMMASTER_H

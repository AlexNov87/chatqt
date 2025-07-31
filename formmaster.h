#ifndef FORMMASTER_H
#define FORMMASTER_H
#include "structs.h"
#include <QDialog>
#include <QPushButton>


namespace Ui {
class Formmaster;
}

class Formmaster : public QDialog , public NamePassword
{
    Q_OBJECT

public:
    explicit Formmaster(const str_type& label);
    ~Formmaster();
private slots:
    void on_bb_choose_accepted();
    void CheckToAccept();

private:
    Ui::Formmaster *ui;

};

#endif // FORMMASTER_H

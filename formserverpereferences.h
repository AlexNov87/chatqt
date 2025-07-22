#ifndef FORMSERVERPEREFERENCES_H
#define FORMSERVERPEREFERENCES_H

#include"structs_ch.h"
namespace Ui {
class FormServerPereferences;
}

class FormServerPereferences : public QWidget
{
    Q_OBJECT

public:
    explicit FormServerPereferences(std::shared_ptr<ConfigInit> inits);
    ~FormServerPereferences();
private:
    Ui::FormServerPereferences *ui;
};

#endif // FORMSERVERPEREFERENCES_H

#ifndef ADD_DANCE_D_H
#define ADD_DANCE_D_H

#include <QDialog>

namespace Ui {
class add_dance_d;
}

class add_dance_d : public QDialog
{
    Q_OBJECT
    
public:
    explicit add_dance_d(QWidget *parent = 0);
    ~add_dance_d();
    
private:
    Ui::add_dance_d *ui;
};

#endif // ADD_DANCE_D_H

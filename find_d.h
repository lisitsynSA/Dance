#ifndef FIND_D_H
#define FIND_D_H

#include <QDialog>

namespace Ui {
class find_d;
}

class find_d : public QDialog
{
    Q_OBJECT
    
public:
    explicit find_d(QWidget *parent = 0);
    ~find_d();
    
private:
    Ui::find_d *ui;
};

#endif // FIND_D_H

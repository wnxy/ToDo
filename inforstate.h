#ifndef INFORSTATE_H
#define INFORSTATE_H

#include <QDialog>

namespace Ui {
class InforState;
}

class InforState : public QDialog
{
    Q_OBJECT

public:
    explicit InforState(QWidget *parent = nullptr);
    ~InforState();

private:
    Ui::InforState *ui;
};

#endif // INFORSTATE_H

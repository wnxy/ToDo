#include "inforstate.h"
#include "ui_inforstate.h"

InforState::InforState(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InforState)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);   // 设置对话框标题栏隐藏
    this->setAttribute(Qt::WA_DeleteOnClose);       // 对话框关闭之后自动销毁
    this->show();
}

InforState::~InforState()
{
    delete ui;
}

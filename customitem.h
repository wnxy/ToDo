#ifndef CUSTOMITEM_H
#define CUSTOMITEM_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QDateTime>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QListWidgetItem>

#include "common.h"
#include "datamanager.h"

/**
 * @brief The NoteEdit class
 * 待办事项编辑控件
 */
class NoteEdit : public QWidget
{
    Q_OBJECT

public:
    NoteEdit(QWidget *parent = nullptr);
    ~NoteEdit();

    void setNoteEdit(const Notes &);
    void clear();
    void showNoteEdit(QPoint &pos);    // 设置窗口显示位置
    void hideNoteEdit();               // 隐藏窗口显示

private:
    void paintEvent(QPaintEvent *event);
    void setStyle(const QString &style);

private slots:
    void onFinBtnCheck();         // 待办事项编辑完成响应动作
    void onCancelBtnCheck();      // 取消编辑待办事项响应动作
    void changeFinBtnAct();       // 根据内容编辑区域是否为空设置“完成”按钮的活动状态

signals:
    void oneNoteFin(Notes);

private:
    QWidget *ctlWidget;
    QTextEdit *noteEditArea;      // 待办事项编辑区域
    QPushButton *finBtn;          // 编辑完成按钮
    QPushButton *cancelBtn;       // 取消编辑按钮
    QLabel *timeLabel;            // 实现时间显示label
    Notes *note;
};

/**
 * @brief The NoteDisplay class
 * 待办事项展示控件
 */
class NoteDisplay : public QWidget
{
    Q_OBJECT

public:
    NoteDisplay(QWidget *parent = nullptr);
    ~NoteDisplay();

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void setNoteID(const int &ID);                // 设置待办事项的ID
    void setNoteState();                          // 设置事项被选中
    void setNoteTitle(const QString &title);      // 设置待办项的标题
    void setNoteTime(const QString &time);        // 设置待办项的时间
    void setNoteStyle(const QString &style);      // 设置待办事项完成之后的样式
    void setNoteEnabled(bool);                    // 设置待办事项是否可编辑
    void setListWidgetItem(QListWidgetItem *item);// 设置待办事项插入ListWidget时的封装
    QListWidgetItem* getListWidgetItem();         // 获取待办事项插入ListWidget时的封装

private:
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void finToBeDone();                           // 待办事项完成

signals:
    void oneNoteUpdata(int, NoteDisplay *);
    void oneNoteDone(int, NoteDisplay *);         // 待办事项是否完成

private:
    QWidget *editWidget;
    QWidget *delWidget;
    QListWidgetItem *item;            // 待办事项插入ListWidget时的封装
    QPushButton *selectedBtn;         // 事项完成选中
    QPushButton *delBtn;              // 删除待办事项按钮
    QLineEdit *lineEdit;              // 待办事项标题
    QLabel *timeLabel;                // 实现时间显示label
    bool isDone;                      // 标记待办事项是否完成
    int noteID;                       // 笔记ID
};

/**
 * @brief The CustomItem class
 * 通用自定义内容控件，可以用来展示/编辑待办事项
 */
class CustomItem : public QWidget
{
    Q_OBJECT

public:
    CustomItem(QWidget *parent = nullptr, bool flag = false);
    ~CustomItem();

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void setNoteTitle(const QString &);

public:
//    void addNewItemData(int nRow, QString noteTitle);        // 数据添加接口

private slots:
    void onFinBtnCheck();         // 完成编辑响应函数
    void changeFinBtnAct();       // 根据内容编辑区域是否为空设置“完成”按钮的活动状态

signals:
    void oneNoteFin(Notes);

private:
    QWidget *ctlWidget;
    QWidget *noteWidget;
    QTextEdit *notesEditArea;     // 待办事项编辑区域
    QPushButton *finBtn;          // 编辑完成按钮
    QCheckBox *checkBox;          // 事项完成选择框
    QLabel *timeLabel;            // 实现时间显示label
    bool isClose;                 // 标记Item展开还是折叠
};

#endif // CUSTOMITEM_H

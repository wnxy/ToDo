#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMessageBox>

#include "systemtray.h"
#include "datamanager.h"
#include "customitem.h"

#ifdef Q_OS_WIN
#ifdef __cplusplus
    extern "C"{
#endif
#include "windows.h"
    bool enumUserWindowsCB(HWND hwnd, LPARAM lParam);
    void setParentDesktop(QWidget *pWidget);
#ifdef __cplusplus
    }
#endif
#endif // CWINDOWS_H

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ListWidgetItem : public QListWidgetItem
{
public:
    bool operator<(const QListWidgetItem &other) const
    {
        SortWeight a = this->data(Qt::UserRole).value<SortWeight>();
        SortWeight b = other.data(Qt::UserRole).value<SortWeight>();

        if(a.tag == b.tag)
        {
            return !compare(a.time, b.time);
        }
        return a.tag < b.tag;
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    void showNoteItem(Notes);
    void updateNoteItem(int, NoteDisplay *);            // 更新待办事项内容
    void updataNoteState(int, NoteDisplay *);           // 更新待办事项 完成/未完成

private:
    void paintEvent(QPaintEvent *);
    void initDataObject();                              // 初始化数据对象，后续进行数据库操作
    void clearDataObject();                             // 清理数据对象资源，释放农村
    void initNoteWindow();                              // 软件初始化，拉取待办数据，显示在主界面
    void addNoteItemData(const Notes &note, NoteDisplay *);   // 向主界面更新一条待办事项展示Item
    void setNoteItemStyle(NoteDisplay *);               // 设置主界面显示已完成的待办项时的样式

private slots:
    void on_addToBeDone_clicked();

private:
    Ui::MainWindow *ui;

    SystemTray *sysTray;                  // 系统托盘

    NoteEdit *noteEdit;                   // 待办事项编辑区域

    NoteDisplay *checkedNoteDisplay;      // 当前更新的待办事项

    DataManager *dataManager;             // 数据库操作对象

    bool isPress;                         // 记录鼠标是否被按下，实现窗口拖动

    QPoint pressPos;                      // 鼠标被按下时的位置
};
#endif // MAINWINDOW_H

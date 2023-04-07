#include "mainwindow.h"
#include "ui_mainwindow.h"

#ifdef Q_OS_WIN

bool enumUserWindowsCB(HWND hwnd, LPARAM lParam)
{
    long wflags = GetWindowLong(hwnd, GWL_STYLE);
    if (!(wflags & WS_VISIBLE))
    {
        return TRUE;
    }

    HWND sndWnd;
    if (!(sndWnd = FindWindowEx(hwnd, nullptr, L"SHELLDLL_DefView", nullptr)))
    {
        return TRUE;
    }
    HWND targetWnd;
    if (!(targetWnd = FindWindowEx(sndWnd, nullptr, L"SysListView32", L"FolderView")))
    {
        return TRUE;
    }

    HWND * resultHwnd = (HWND *)lParam;
    *resultHwnd = targetWnd;
    return FALSE;
}

void setParentDesktop(QWidget *pWidget)
{
    HWND resultHwnd = nullptr;
    EnumWindows((WNDENUMPROC)enumUserWindowsCB, (LPARAM)&resultHwnd);

    // 设置桌面为父窗口
    if (resultHwnd)
    {
        SetParent((HWND)pWidget->winId(), resultHwnd);
    }
}
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->toDoList->setSpacing(4);
    // Qt::WindowStaysOnTopHint 窗口总在最上方
    // Qt::FramelessWindowHint 窗口没有边框
    // Qt::Tool | Qt::X11BypassWindowManagerHint 隐藏任务栏显示
//    this->setWindowFlags(Qt::Tool);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::X11BypassWindowManagerHint);
    this->setWindowIcon(QIcon(":/Images/Resources/Images/sysIcon.ico"));
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowOpacity(0.5);

    setParentDesktop(this);
    sysTray = new SystemTray(this);
    connect(sysTray, &SystemTray::setAppAutoStartSign, this, &MainWindow::setApplicationAutoRun);

    initDataObject();
    initNoteWindow();
    setApplicationAutoRun();

    noteEdit = new NoteEdit();
    connect(noteEdit, &NoteEdit::oneNoteFin, this, &MainWindow::showNoteItem);

    checkedNoteDisplay = nullptr;

#ifdef QT_DEBUG
    qDebug() << this->isWindow();
    qDebug() << this->size();
#endif
    Log::getLogger()->LOG_DEBUG("Debug: MainWindow construct.")

}

MainWindow::~MainWindow()
{
    clearDataObject();
    delete noteEdit;
    delete ui;

    Log::getLogger()->LOG_DEBUG("Debug: MainWindow destruct.")
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        isPress = true;
        pressPos = event->pos();            // 记录鼠标按下的位置
        setCursor(Qt::OpenHandCursor);       // 改变光标
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        isPress = false;
        setCursor(Qt::ArrowCursor);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(this->isPress && isToDoMove)
    {
        this->move(event->globalPos() - this->pressPos);
    }
}

void MainWindow::closeEvent(QCloseEvent *)
{
    QApplication::quit();
    this->close();
    Log::getLogger()->LOG_DEBUG("Debug: Frameless Window closeEvent.")
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    int r = mainWinBGColor.red();
    int g = mainWinBGColor.green();
    int b = mainWinBGColor.blue();
    int a = mainWinBGAlpha;
    QColor c = QColor(r, g, b, a);       // 窗体背景色QColor(97, 111, 118, 100)
    painter.setBrush(QBrush(c));         // 窗体背景色
    painter.setPen(Qt::transparent);
    QRect rect = this->rect();                      // rect为绘制大小
    rect.setWidth(rect.width() - 1);
    rect.setHeight(rect.height() - 1);
    painter.drawRoundedRect(rect, 5, 5);          // 10为圆角角度
    //也可用QPainterPath 绘制代替 painter.drawRoundedRect(rect, 15, 15);
    //QPainterPath painterPath;
    //painterPath.addRoundedRect(rect, 15, 15);//15为圆角角度
    //painter.drawPath(painterPath);
    this->setFont(QFont(fontFamily, fontPointSize, 25));
    ui->ctlWidget->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4);")
                                 .arg(mainWinBGColor.red())
                                 .arg(mainWinBGColor.green())
                                 .arg(mainWinBGColor.blue())
                                 .arg(QString::number(mainWinBGAlpha)));
    QMainWindow::paintEvent(event);
    Log::getLogger()->LOG_DEBUG("Debug: MainWindow painter.")
}

void MainWindow::initDataObject()
{
    dataManager = new DataManager();
    if(dataManager->openDatabase())
    {
        // 判断存储待办事项的表是否存在，不存在就创建
        if(!dataManager->isTableExist(DATABASE_TABLENAME))
        {
            dataManager->createTable(DATABASE_TABLENAME);
        }
        Log::getLogger()->LOG_INFO("Info: Create dateManager and open the datebase.")
    }
    else
    {
        QMessageBox::critical(nullptr, "Cannot open database",
                              "Unable to establish a database connection.", QMessageBox::Cancel);
        Log::getLogger()->LOG_INFO("Info: Failed open the datebase.")
    }
}

void MainWindow::clearDataObject()
{
    dataManager->closeDatabase();
    delete dataManager;
    Log::getLogger()->LOG_INFO("Info: Succeed close database.")
}

void MainWindow::initNoteWindow()
{
    QVector<Notes *> notes;
    if(dataManager->queryAllData(DATABASE_TABLENAME, notes))
    {
        while(!notes.empty())
        {
            this->addNoteItemData(*(notes.back()), nullptr);
            notes.pop_back();
        }
    }
    Log::getLogger()->LOG_INFO("Info: Init the TODO win.")
}

/**
 * @brief MainWindow::addNewItemData
 * 软件主界面插入一条自定义Item
 * @param note
 * @param noteItem 标记是否是新建的待办项，如果值为null表示是新建待办项，如果不是表示更新已有的待办项
 */
void MainWindow::addNoteItemData(const Notes &note, NoteDisplay *noteItem)
{
    if(!noteItem)    // 新建待办项
    {
        NoteDisplay *noteDisplay = new NoteDisplay(this);
        noteDisplay->setNoteID(note.id);
        noteDisplay->setNoteTime(note.noteTime);
        noteDisplay->setNoteTitle(note.noteTitle);
        connect(noteDisplay, &NoteDisplay::oneNoteUpdata, this, &MainWindow::updateNoteItem);
        connect(noteDisplay, &NoteDisplay::oneNoteDone, this, &MainWindow::updataNoteState);
        connect(noteDisplay, &NoteDisplay::oneNoteDeleted, this, &MainWindow::delNoteItem);

        ListWidgetItem *item = new ListWidgetItem;
        item->setSizeHint(QSize(48, 48));
        SortWeight st{note.isFin, note.noteTime};
        item->setData(Qt::UserRole, QVariant::fromValue(st));
        ui->toDoList->addItem(item);
        ui->toDoList->setItemWidget(item, noteDisplay);
        ui->toDoList->sortItems();         // 自定义排序
        noteDisplay->setListWidgetItem(item);
        if(note.isFin)
        {
            this->setNoteItemStyle(noteDisplay);
        }
        Log::getLogger()->LOG_INFO("Info: Add one NoteItem.")
    }
    else              // 更新已有的待办项
    {
        noteItem->setNoteID(note.id);
        noteItem->setNoteTime(note.noteTime);
        noteItem->setNoteTitle(note.noteTitle);
        Log::getLogger()->LOG_INFO("Info: Updated one NoteItem.")
    }
}

void MainWindow::setNoteItemStyle(NoteDisplay *noteDisplay)
{
    noteDisplay->setNoteState();
    noteDisplay->setNoteStyle("background: transparent;border-width:0;border-style:outset;text-decoration:line-through;");
    noteDisplay->setNoteTime("Done");
    noteDisplay->setNoteEnabled(false);
//    noteDisplay->setEnabled(false);
    Log::getLogger()->LOG_INFO("Info: Set NoteItem style.")
}

void MainWindow::setApplicationAutoRun()
{
    /* if auto start for all users
     * HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run
     * else if auto start for current user
     * HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run
     */    
    QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

    QString appName = QApplication::applicationName();

    if (autoStart)
    {
        QString strAppPath = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
        strAppPath.replace(".exe", ".lnk");
        if(!QFile::exists(strAppPath))
        {
            QFile::link(QCoreApplication::applicationFilePath(), appName + ".lnk");
        }
        reg.setValue(appName, strAppPath);
        Log::getLogger()->LOG_INFO("Info: Set TODO auto run on computer start.")
    }
    else
    {
        reg.remove(appName);
        Log::getLogger()->LOG_INFO("Info: Cancel TODO auto run on computer start.")
    }
}

/**
 * @brief MainWindow::showNoteItem
 * 软件界面上显示一条待办事项
 * @param note
 */
void MainWindow::showNoteItem(Notes note)
{
    if(!note.id)     // 新建待办事项，将内容插入到数据库
    {
        dataManager->insertData(DATABASE_TABLENAME, note);
        this->addNoteItemData(note, nullptr);
#ifdef QT_DEBUG
        qDebug() << "Debug: Add new note data.";
#endif
        Log::getLogger()->LOG_INFO("Info: Add new note data.")
    }
    else             // 更新待办事项，将内容更新到数据库
    {
        dataManager->updateData(DATABASE_TABLENAME, note, note.id);
        checkedNoteDisplay->setNoteID(note.id);
        checkedNoteDisplay->setNoteTime(getCurrentDate(' '));
        checkedNoteDisplay->setNoteTitle(note.noteTitle);
#ifdef QT_DEBUG
        qDebug() << "Debug: Update note data.";
#endif
        Log::getLogger()->LOG_INFO("Info: Update note data.")
    }
//    ui->addToBeDone->setEnabled(true);
}

/**
 * @brief MainWindow::updateNoteItem
 * 更新已有待办项的内容
 * @param noteID
 */
void MainWindow::updateNoteItem(int noteID, NoteDisplay *noteDisplay)
{
    Notes note;
    checkedNoteDisplay = noteDisplay;
    dataManager->queryData(DATABASE_TABLENAME, note, noteID);
    QPoint pos(this->geometry().x() + 9, this->geometry().y() + this->geometry().height() / 2 - 45);
    noteEdit->setNoteEdit(note);
    noteEdit->showNoteEdit(pos);
    Log::getLogger()->LOG_INFO("Info: Updated note context.")
}

/**
 * @brief MainWindow::updataNoteState
 * 更新待办事项的完成状态
 * @param noteID 当前被操作的待办事项
 * @param isDone 表示待办事项 完成/未完成
 */
void MainWindow::updataNoteState(int noteID, NoteDisplay *noteDisplay)
{
    dataManager->updateState(DATABASE_TABLENAME, noteID, 1);
    this->setNoteItemStyle(noteDisplay);
    SortWeight st{1, getCurrentDate(' ')};
    noteDisplay->getListWidgetItem()->setData(Qt::UserRole, QVariant::fromValue(st));
    ui->toDoList->sortItems();         // 自定义排序
#ifdef QT_DEBUG
    qDebug() << "QDebug: Update note state.";
#endif
    Log::getLogger()->LOG_INFO("Info: Marked note is completed.")
}

void MainWindow::delNoteItem(int noteID, NoteDisplay *noteDisplay)
{
    dataManager->deleteData(DATABASE_TABLENAME, noteID);
    QListWidgetItem *item = noteDisplay->getListWidgetItem();
    delete item;
    ui->toDoList->sortItems();
    Log::getLogger()->LOG_INFO("Info: Delete one note.")
}

/**
 * @brief MainWindow::on_addToBeDone_clicked
 * 添加并编辑待办事项
 */
void MainWindow::on_addToBeDone_clicked()
{
    noteEdit->clear();
    QPoint pos(this->geometry().x() + 9, this->geometry().y() + this->geometry().height() / 2 - 45);
    noteEdit->showNoteEdit(pos);

//    ui->addToBeDone->setEnabled(false);
    Log::getLogger()->LOG_INFO("Info: Add one new note.")
}
//void MainWindow::on_addToBeDone_clicked()
//{
//    CustomItem *customItem = new CustomItem(this);
//    connect(customItem, &CustomItem::oneNoteFin, this, &MainWindow::showNoteItem);
//    customItem->show();

//    ui->addToBeDone->setEnabled(false);
//}


void MainWindow::on_pushButton_clicked()
{

}

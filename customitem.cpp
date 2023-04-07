#include "customitem.h"

NoteEdit::NoteEdit(QWidget *parent) : QWidget(parent)
{
    ctlWidget = new QWidget(this);
    noteEditArea = new QTextEdit(this);
//    noteEditArea->setFont(QFont("Microsoft YaHei UI", 10, 25));
    noteEditArea->setStyleSheet("background: transparent;border-width:0;border-style:outset;");
    this->setStyle(":/Resources/Qss/ScrollBarStyle.qss");          // 设置待办事项编辑区域滚动条样式

    finBtn = new QPushButton(ctlWidget);
    finBtn->setText(QString(tr("完成")));
    finBtn->setFlat(true);
    cancelBtn = new QPushButton(ctlWidget);
    cancelBtn->setText(QString(tr("取消")));
    cancelBtn->setFlat(true);
    timeLabel = new QLabel(ctlWidget);
//    timeLabel->setText("时间");
    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *subLayout = new QHBoxLayout(ctlWidget);

    subLayout->addWidget(timeLabel);
    subLayout->addWidget(cancelBtn);
    subLayout->addWidget(finBtn);
    subLayout->setMargin(0);

    layout->addWidget(noteEditArea);
    layout->addWidget(ctlWidget);
    layout->setMargin(5);
    layout->setSpacing(0);

//    subLayout->setSpacing(100);

    note = new Notes();

    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setMinimumSize(QSize(362, 90));
    this->setWindowModality(Qt::ApplicationModal);    // 设置该窗口为模态
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFont(QFont("Microsoft YaHei UI", 10, 25));
//    this->show();

    connect(finBtn, &QPushButton::clicked, this, &NoteEdit::onFinBtnCheck);
    connect(cancelBtn, &QPushButton::clicked, this, &NoteEdit::onCancelBtnCheck);
    connect(noteEditArea, &QTextEdit::textChanged, this, &NoteEdit::changeFinBtnAct);
}

NoteEdit::~NoteEdit()
{
    delete note;
}

void NoteEdit::setNoteEdit(const Notes &note)
{
    this->clear();
    noteEditArea->setText(note.noteContext);
    // 更新时间
    timeLabel->setText(getCurrentDate(' '));
    this->note->copy(note);
}

void NoteEdit::clear()
{
    noteEditArea->clear();
    timeLabel->clear();
}

void NoteEdit::showNoteEdit(QPoint &pos)
{
    this->noteEditArea->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
//    QTextCursor cursor = this->noteEditArea->textCursor();
//    cursor.movePosition(QTextCursor::End);
//    this->noteEditArea->setTextCursor(cursor);
    this->timeLabel->setText(getCurrentDate(' '));
    this->setGeometry(pos.x(), pos.y(), 362, 90);
    this->show();
}

void NoteEdit::hideNoteEdit()
{
    this->hide();
}

void NoteEdit::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);      // 反锯齿;
    int r = noteEditBGColor.red();
    int g = noteEditBGColor.green();
    int b = noteEditBGColor.blue();
    int a = noteEditBGAlpha;
    QColor c = QColor(r, g, b, a);       // 窗体背景色QColor(200, 207, 210)
    painter.setBrush(QBrush(c));    // 窗体背景色
    painter.setPen(Qt::transparent);
    QRect rect = this->rect();                          // rect为绘制大小
    rect.setWidth(rect.width() - 1);
    rect.setHeight(rect.height() - 1);
    painter.drawRoundedRect(rect, 5, 5);                // 5为圆角角度
    //也可用QPainterPath 绘制代替 painter.drawRoundedRect(rect, 15, 15);
    //QPainterPath painterPath;
    //painterPath.addRoundedRect(rect, 15, 15);         //15为圆角角度
    //painter.drawPath(painterPath);
    this->noteEditArea->setFont(QFont(fontFamily, fontPointSize, 25));
    this->noteEditArea->setTextColor(noteFontColor);
    QWidget::paintEvent(event);
}

void NoteEdit::setStyle(const QString &style)
{
    QFile file(style);
    file.open(QFile::ReadOnly);
    QString qss = QLatin1String(file.readAll());
    noteEditArea->verticalScrollBar()->setStyleSheet(qss);
    file.close();
}

void NoteEdit::onFinBtnCheck()
{
//    QScopedPointer<Notes> note(new Notes());
//    note.data()->noteContext = this->notes->toPlainText();
//    note.data()->noteTime = this->timeLabel->text();
//    note.data()->noteTitle = note.data()->noteContext.left(25);
//    note.data()->isFin = 0;
//    emit oneNoteFin(*(note.data()));
    if(note->empty())
    {
#ifdef QT_DEBUG
        qDebug() << "QDebug: New note.";
#endif
        Log::getLogger()->LOG_DEBUG("Debug: New note is empty.")
    }
    note->noteContext = this->noteEditArea->toPlainText();
    note->noteTime = this->timeLabel->text();
    note->noteTitle = note->noteContext.left(13);       // 标题长度为前13个字符
    note->isFin = 0;
    emit oneNoteFin(*note);
    note->clear();
//    this->close();
    this->hide();
}

void NoteEdit::onCancelBtnCheck()
{
    note->clear();
    this->hide();
}

void NoteEdit::changeFinBtnAct()
{
    if(noteEditArea->toPlainText().length())
    {
        this->finBtn->setEnabled(true);
    }
    else
    {
        this->finBtn->setEnabled(false);
    }
}

NoteDisplay::NoteDisplay(QWidget *parent) : QWidget(parent)
{
    editWidget = new QWidget(this);
    delWidget = new QWidget(this);
    selectedBtn = new QPushButton(editWidget);
    selectedBtn->setFlat(true);
    selectedBtn->setIcon(QIcon(":/Images/Resources/Images/unselected.svg"));
    lineEdit = new QLineEdit(editWidget);
    lineEdit->setFrame(false);
    lineEdit->setReadOnly(true);
    lineEdit->setStyleSheet("background: transparent;border-width:0;border-style:outset;");
//    lineEdit->setEnabled(false);
    timeLabel = new QLabel(editWidget);
    delBtn = new QPushButton(delWidget);
    delBtn->setFlat(true);
    delBtn->setIcon(QIcon(":/Images/Resources/Images/delete.svg"));
    delBtn->setVisible(false);
    QHBoxLayout *layout = new QHBoxLayout(this);
    QHBoxLayout *sublayout = new QHBoxLayout(editWidget);

    sublayout->addWidget(selectedBtn);
    sublayout->addWidget(lineEdit);
    sublayout->addWidget(timeLabel);
    sublayout->addWidget(delBtn);
    sublayout->setMargin(0);

    layout->addWidget(editWidget);
    layout->addWidget(delWidget);
    layout->setMargin(6);
    layout->setSpacing(0);

    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_Hover, true);     // 开启悬停事件
    this->installEventFilter(this);             // 安装事件过滤器

    noteID = 0;
    isDone = false;
    item = nullptr;
    connect(selectedBtn, &QPushButton::clicked, this, &NoteDisplay::finToBeDone);
    connect(delBtn, &QPushButton::clicked, this, &NoteDisplay::delToBeDone);
}

NoteDisplay::~NoteDisplay()
{

}

void NoteDisplay::mousePressEvent(QMouseEvent *event)
{
    if(!isDone)
    {
        if(event->button() == Qt::LeftButton)
        {
            setCursor(Qt::PointingHandCursor);      // 设置光标样式
        }
    }
}

void NoteDisplay::mouseReleaseEvent(QMouseEvent *event)
{
    if(!isDone)
    {
        if(event->button() == Qt::LeftButton)
        {
            setCursor(Qt::ArrowCursor);               // 设置光标样式
            emit oneNoteUpdata(this->noteID, this);   // 待办事项更新信号
        }
    }
}

void NoteDisplay::setNoteID(const int &ID)
{
    this->noteID = ID;
}

void NoteDisplay::setNoteState()
{
//    this->checkBox->setCheckState(Qt::Checked);
    isDone = true;
    this->selectedBtn->setIcon(QIcon(":/Images/Resources/Images/selected.svg"));
}

void NoteDisplay::setNoteTitle(const QString &title)
{
    this->lineEdit->setText(title);
}

void NoteDisplay::setNoteTime(const QString &time)
{
    this->timeLabel->setText(time);
}

void NoteDisplay::setNoteStyle(const QString &style)
{
    this->lineEdit->setStyleSheet(style);
}

void NoteDisplay::setNoteEnabled(bool flag)
{
//    this->editWidget->setEnabled(flag);
    this->selectedBtn->setEnabled(flag);
    this->lineEdit->setEnabled(flag);
    this->timeLabel->setEnabled(flag);
}

void NoteDisplay::setListWidgetItem(QListWidgetItem *item)
{
    this->item = item;
}

QListWidgetItem* NoteDisplay::getListWidgetItem()
{
    return this->item;
}

void NoteDisplay::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    int r = noteEditBGColor.red();
    int g = noteEditBGColor.green();
    int b = noteEditBGColor.blue();
    int a = mainWinBGAlpha;
    QColor c = QColor(r, g, b, a);       // 窗体背景色QColor(200, 207, 210, 100)
    painter.setBrush(QBrush(c));    // 窗体背景色
    painter.setPen(Qt::transparent);
    QRect rect = this->rect();                      // rect为绘制大小
    rect.setWidth(rect.width() - 1);
    rect.setHeight(rect.height() - 1);
    painter.drawRoundedRect(rect, 5, 5);          // 5为圆角角度
    //也可用QPainterPath 绘制代替 painter.drawRoundedRect(rect, 15, 15);
    //QPainterPath painterPath;
    //painterPath.addRoundedRect(rect, 15, 15);//15为圆角角度
    //painter.drawPath(painterPath);
    QWidget::paintEvent(event);
//#ifdef QT_DEBUG
//    qDebug() << "Debug: NoteDisplay paintEvent.";
//#endif
}

bool NoteDisplay::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == this)
    {
        if(event->type() == QEvent::HoverEnter)
        {
# ifdef QT_DEBUG
            qDebug() << "Debug: NoteDisplay mouse hover.";
#endif
            delBtn->setVisible(true);
            return true;
        }
        else if(event->type() == QEvent::HoverLeave)
        {
            delBtn->setVisible(false);
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void NoteDisplay::finToBeDone()
{
//    if(checkBox->isChecked())    // 被选中，表示待办事项完成
//    {
////        this->lineEdit->setStyleSheet("background: transparent;border-width:0;border-style:outset;text-decoration:line-through;");
////        this->setNoteTime("Done");
////        this->setEnabled(false);
//        emit oneNoteDone(this->noteID, this);
//    }
////    else                         // 取消选中，表示待办事项未完成
////    {
////        this->lineEdit->setStyleSheet("background: transparent;border-width:0;border-style:outset;text-decoration:none;");
////        emit oneNoteDone(this->noteID, false);
////    }
#ifdef QT_DEBUG
    qDebug() << "QDebug: finToBeDone state is changed.";
#endif

    emit oneNoteDone(this->noteID, this);
}

void NoteDisplay::delToBeDone()
{
#ifdef QT_DEBUG
    qDebug() << "Debug: delete a note.";
#endif

    emit oneNoteDeleted(this->noteID, this);
}

CustomItem::CustomItem(QWidget *parent, bool flag) : QWidget(parent)
{
    noteWidget = new QWidget(this);
    ctlWidget = new QWidget(this);
    notesEditArea = new QTextEdit(noteWidget);
    finBtn = new QPushButton(ctlWidget);
    finBtn->setText(QString(tr("完成")));
    finBtn->setFlat(true);
    finBtn->setEnabled(false);
    checkBox = new QCheckBox(noteWidget);
    timeLabel = new QLabel(ctlWidget);
    timeLabel->setText(getCurrentDate(' '));
    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *subLayout1 = new QHBoxLayout(noteWidget);
    QHBoxLayout *subLayout2 = new QHBoxLayout(ctlWidget);

    layout->addWidget(noteWidget);
    layout->addWidget(ctlWidget);
    layout->setMargin(0);
    layout->setSpacing(0);
    subLayout1->addWidget(checkBox);
    subLayout1->addWidget(notesEditArea);
    subLayout1->setMargin(0);
    subLayout2->addWidget(timeLabel);
    subLayout2->addItem(new QSpacerItem(220, 20));
    subLayout2->addWidget(finBtn);
    subLayout2->setMargin(0);

    connect(finBtn, &QPushButton::clicked, this, &CustomItem::onFinBtnCheck);
    connect(notesEditArea, &QTextEdit::textChanged, this, &CustomItem::changeFinBtnAct);
    if(!flag)
    {
        this->setMinimumSize(QSize(380, 100));
        this->ctlWidget->setVisible(true);
    }
    else
    {
        this->setMinimumSize(QSize(380, 30));
        this->ctlWidget->setVisible(false);
    }

#ifdef QT_DEBUG
    qDebug() << "CustomItem size:" << this->size();
#endif
}

CustomItem::~CustomItem()
{

}

void CustomItem::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        setCursor(Qt::PointingHandCursor);      // 设置光标样式
    }
}

void CustomItem::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        setCursor(Qt::ArrowCursor);             // 设置光标样式
        this->setMinimumSize(QSize(380, 100));
        this->ctlWidget->setVisible(true);
        this->notesEditArea->setEnabled(true);  // 自定义Item展开时，允许编辑文本区域
    }
}

//void CustomItem::addNewItemData(int nRow, QString noteTitle)
//{

//}

void CustomItem::onFinBtnCheck()
{
    QScopedPointer<Notes> note(new Notes());
    note.data()->noteContext = this->notesEditArea->toPlainText();
    note.data()->noteTime = this->timeLabel->text();
    note.data()->noteTitle = note.data()->noteContext.left(25);
    note.data()->isFin = 0;
    emit oneNoteFin(*(note.data()));

    this->ctlWidget->setVisible(false);
    this->setFixedSize(QSize(380, 30));
    this->notesEditArea->setEnabled(false);         // 自定义Item收起时，禁止编辑文本区域

#ifdef QT_DEBUG
   qDebug() << "CustomItem fix size:" << this->size();
#endif
}

void CustomItem::changeFinBtnAct()
{
    if(notesEditArea->toPlainText().length())
    {
        this->finBtn->setEnabled(true);
    }
    else
    {
        this->finBtn->setEnabled(false);
    }
}

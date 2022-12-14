#include "editarea.h"
#include "view_model.h"
#include "text_highlighter.h"
#include "text_linenumberarea.h"
#include <QPainter>
#include <QShortcut>

EditArea::EditArea(QWidget *parent) : QPlainTextEdit(parent)
{
    font = QFont("Microsoft YaHei",10);
    tab_step = 4;
    this->setFont(font);
    this->setTabStopWidth(tab_step*font.pointSize());
    //this->setLineWrapMode(QTextEdit::NoWrap);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->installEventFilter(this);

    text_highlighter = new Text_Highlighter(this->document());
    text_linenumberarea = new Text_LineNumberArea(this);
    cpp_parser = new Cpp_Parser(this->document());

    connect(this, &EditArea::blockCountChanged, this, &EditArea::updateLineNumberAreaWidth);
    connect(this, &EditArea::updateRequest, this, &EditArea::updateLineNumberArea);
    connect(this, &EditArea::cursorPositionChanged, this, &EditArea::highlightCurrentLine);
    connect(this, &EditArea::resizeEvent, this, &EditArea::lineNumberAreaResizeEvent);
    connect(this, &EditArea::textChanged, this, &EditArea::slot_textChanged);

    key_functions_before = {
        {Qt::Key_Return, [&](){ //Enter
             cpp_parser->calc_Layer(textCursor().position());
        }},
        {Qt::Key_Backspace, [&](){ //Backspace
             const int &pos = textCursor().position();
             if(cpp_parser->get_Extra_Backspace(pos)){
                 remove_Bracket_Pair(pos);
             }
        }},
        {Qt::Key_ParenLeft, [&](){ //(
            cpp_parser->calc_Make_Pair(textCursor().position());
        }},
        {Qt::Key_BracketLeft, [&](){ //[
            cpp_parser->calc_Make_Pair(textCursor().position());
        }},
        {Qt::Key_BraceLeft, [&](){ //{
            cpp_parser->calc_Make_Pair(textCursor().position());
        }},
        {Qt::Key_Apostrophe, [&](){ //'
            cpp_parser->calc_Make_Pair(textCursor().position());
        }},
        {Qt::Key_QuoteDbl, [&](){ //"
            cpp_parser->calc_Make_Pair(textCursor().position());
        }},
    };

    key_functions_after = {
        {Qt::Key_Return, [&](){ //Enter
            if(cpp_parser->get_Extra_Line()){
                append_Text("\n" + fill_Tab(cpp_parser->get_Current_Layer() - 1));
            }
            insertPlainText(fill_Tab(cpp_parser->get_Current_Layer()));
        }},
        {Qt::Key_ParenLeft, [&](){ //(
            if(cpp_parser->get_Make_Pair()){
                append_Text(")");
            }
        }},
        {Qt::Key_BracketLeft, [&](){ //[
            if(cpp_parser->get_Make_Pair()){
                append_Text("]");
            }
        }},
        {Qt::Key_BraceLeft, [&](){ //{
            if(cpp_parser->get_Make_Pair()){
                append_Text("}");
            }
        }},
        {Qt::Key_Apostrophe, [&](){ //'
            if(cpp_parser->get_Make_Pair()){
                append_Text("\'");
            }
        }},
        {Qt::Key_QuoteDbl, [&](){ //"
            if(cpp_parser->get_Make_Pair()){
                append_Text("\"");
            }
        }},
    };

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
    //set is changed
    is_changed = true;
}

EditArea::~EditArea(){
    delete text_highlighter;
    delete text_linenumberarea;
    delete cpp_parser;
}

EditArea* EditArea::EditArea_Factory(){
    EditArea* ret = new EditArea();
    ret->file_name = "新建文件";
    return ret;
}

bool EditArea::pathExists(){
    return file_fullname != "";
}

//Set is saved
bool EditArea::get_Is_Changed(){
    return is_changed;
}

void EditArea::set_Saved(){
    is_changed = false;
}

void EditArea::slot_textChanged(){
    is_changed = true;
}

//Fill Tabs
QString EditArea::fill_Tab(const int& len){
    if(len <= 0){
        return QString();
    }
    return QString("\t").repeated(len);
}

//Multi_Lines
int EditArea::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;
    return space;
}

void EditArea::updateLineNumberAreaWidth(int newBlockCount)
{
    Q_UNUSED(newBlockCount);
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void EditArea::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy){
        text_linenumberarea->scroll(0, dy);
    }
    else {
        text_linenumberarea->update(0, rect.y(), text_linenumberarea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect())){
        updateLineNumberAreaWidth(0);
    }
}

void EditArea::lineNumberAreaResizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);
    QRect cr = contentsRect();
    text_linenumberarea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void EditArea::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void EditArea::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(text_linenumberarea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + static_cast<int>(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, text_linenumberarea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }
        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

//Custom Shortcut Event
bool EditArea::eventFilter(QObject *target, QEvent *event){
    Q_UNUSED(target);
    Q_UNUSED(event);
    return false;
}

//Key Press Event
void EditArea::keyPressEvent(QKeyEvent * e){
    //Key Functions
    const int& key = e->key();
    if(key_functions_before.count(key)){
        key_functions_before[key]();
    }
    QPlainTextEdit::keyPressEvent(e);
    if(key_functions_after.count(key)){
        key_functions_after[key]();
    }
}

//Text Cursor Functions
void EditArea::move_Text_Cursor(const int &d){
    QTextCursor cursor = textCursor();
    int current_position = cursor.position();
    cursor.setPosition(current_position + d);
    setTextCursor(cursor);
}

void EditArea::move_Text_Cursor_Pos(const int &pos){
    QTextCursor cursor = textCursor();
    cursor.setPosition(pos);
    setTextCursor(cursor);
}

void EditArea::append_Text(const QString& str){
    this->insertPlainText(str);
    move_Text_Cursor(-str.length());
}

void EditArea::remove_Bracket_Pair(const int& pos){
    QString tmp = toPlainText();
    tmp = tmp.remove(pos, 1);
    setPlainText(tmp);
    move_Text_Cursor_Pos(pos);
}

//Parse Test
QString EditArea::debug_slot_Parse_Test(){
    cpp_parser->slot_Parse();
    return cpp_parser->debug_get_Parse_Result();
}

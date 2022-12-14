#ifndef EDITAREA_H
#define EDITAREA_H

#include <QPlainTextEdit>
#include <QTimer>
#include "text_highlighter.h"
#include "cpp_parser.h"

class Text_LineNumberArea;

class EditArea : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit EditArea(QWidget *parent = nullptr);
    virtual ~EditArea();
    static EditArea* EditArea_Factory();
    QString file_fullname;
    QString file_name;
    QString file_path;
    QString file_suffix;
    QString file_name_no_suffix;
    void setFontSize(int tsize);
    bool pathExists();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    //Check if the file is changed
    bool get_Is_Changed();
    void set_Saved();

signals:
    void resizeEvent(QResizeEvent *e);

public slots:
    void slot_textChanged();
    //Parse Test
    QString debug_slot_Parse_Test();

protected:
    //Virtual Functions
    virtual bool eventFilter(QObject *target, QEvent *event);
    virtual void keyPressEvent(QKeyEvent*);

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect&, int);
    void lineNumberAreaResizeEvent(QResizeEvent *e);

private:
    //StyleSheets
    QFont font;
    int tab_step;
    //Key Functions
    QMap<int, std::function<void()>> key_functions_before;
    QMap<int, std::function<void()>> key_functions_after;
    //Components
    Text_Highlighter* text_highlighter;
    Text_LineNumberArea* text_linenumberarea;
    Cpp_Parser* cpp_parser;
    void append_Text(const QString&);
    void move_Text_Cursor(const int&);
    void move_Text_Cursor_Pos(const int&);
    void remove_Bracket_Pair(const int&);
    QString fill_Tab(const int&);
    //Check if the file is changed
    bool is_changed;
};

#endif // EDITAREA_H

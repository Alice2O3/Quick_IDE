#ifndef CPP_PARSER_H
#define CPP_PARSER_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QRegularExpression>
#include <QStack>
#include <QPointer>

class Cpp_Parser : public QWidget
{
    Q_OBJECT
public:
    enum class Bracket_Type{
        SMALL, //()
        MIDDLE, //[]
        LARGE, //{}
    };
    enum class Quote_Type{
        SMALL, //'
        LARGE, //"
    };
    struct Quote_Info{
        QString name;
        QChar ch;
    };
    struct Bracket_Info{
        QString name;
        QChar left;
        QChar right;
    };
    explicit Cpp_Parser(QTextDocument *p, QWidget *parent = nullptr);
    virtual ~Cpp_Parser();
    static QString str_Parse(const QString&);
    static QString char_Parse(const QChar&);
    QString get_Raw_Code();
    //Layers
    void calc_Layer(const unsigned int&);
    int get_Current_Layer();
    bool get_Extra_Line();
    //Extra Backspace
    bool get_Extra_Backspace(const unsigned int&);
    //End Line
    void calc_Make_Pair(const unsigned int&);
    bool get_Make_Pair();
    //Debug Functions
    QString debug_get_Parse_Result();

signals:

public slots:
    void slot_Parse();

private:
    QString raw_code;
    QStringList raw_code_separated;
    QVector<bool> ignore_char;
    QSet<QChar> space_characters;
    QMap<Bracket_Type, Bracket_Info> bracket_names;
    QMap<Quote_Type, Quote_Info> quote_names;
    QMap<QChar, Bracket_Type> bracket_set_left;
    QMap<QChar, Bracket_Type> bracket_set_right;
    QMap<Bracket_Type, QStack<unsigned int>*> bracket_stack;
    QMap<Bracket_Type, QVector<QPair<unsigned int, unsigned int>>*> bracket_pair;
    QPointer<QTextDocument> text_document;
    //Regular Expressions
    QRegularExpression quoteExpression;
    QRegularExpression smallQuoteExpression;
    QRegularExpression singleCommentExpression;
    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;
    //Functions
    void clear_Result();
    void get_Ignore_Char();
    void ignore_Regular_Expression(const QRegularExpression& expression, const bool& ignore_last);
    void push_Char(const unsigned int&, const QChar&);
    bool between_Brackets(const unsigned int&, const Bracket_Type&);
    bool between_Quotes(const unsigned int&, const Quote_Type&);
    bool is_Ignored(const unsigned int&);
    //Layers
    int current_layer;
    bool extra_line;
    bool make_pair;
    //Debug Functions
    void debug_print_Result();
    QString parse_result;
};

#endif // CPP_PARSER_H

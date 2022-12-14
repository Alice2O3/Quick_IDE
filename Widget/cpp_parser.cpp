#include "cpp_parser.h"
#include <QDebug>
#include <QPointer>

Cpp_Parser::Cpp_Parser(QTextDocument *p, QWidget *parent) : QWidget(parent){
    text_document = p;
    //Regular Expressions
    quoteExpression = QRegularExpression("(\"[^\"\\\\]*(?:\\\\.[^\"\\\\]*)*\")");
    smallQuoteExpression = QRegularExpression("('[^'\\\\]*(?:\\\\.[^'\\\\]*)*')");
    singleCommentExpression = QRegularExpression("//[^\n]*");
    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
    //Brackets
    bracket_names = {
        {Bracket_Type::SMALL, {"Small Bracket", '(', ')'}},
        {Bracket_Type::MIDDLE, {"Middle Bracket", '[', ']'}},
        {Bracket_Type::LARGE, {"Large Bracket", '{', '}'}},
    };
    bracket_set_left = {
        {'(', Bracket_Type::SMALL},
        {'[', Bracket_Type::MIDDLE},
        {'{', Bracket_Type::LARGE},
    };
    bracket_set_right = {
        {')', Bracket_Type::SMALL},
        {']', Bracket_Type::MIDDLE},
        {'}', Bracket_Type::LARGE},
    };
    //Quotes
    quote_names = {
        {Quote_Type::SMALL, {"Apostrophe", '\''}},
        {Quote_Type::LARGE, {"QuoteDbl", '\"'}},
    };
    //Space Characters
    space_characters = {'\n', '\t', ' ', ';', '(', '[', '{', ')', ']', '}'};
    for(const Bracket_Type &p : bracket_names.keys()){
        bracket_stack[p] = new QStack<unsigned int>;
        bracket_pair[p] = new QVector<QPair<unsigned int, unsigned int>>;
    }
}

Cpp_Parser::~Cpp_Parser(){
    //qDebug() << "Destroy";
    for(const Bracket_Type &p : bracket_names.keys()){
        delete bracket_stack[p];
        delete bracket_pair[p];
    }
}

QString Cpp_Parser::get_Raw_Code(){
    return raw_code;
}

QString Cpp_Parser::debug_get_Parse_Result(){
    return parse_result;
}

QString Cpp_Parser::str_Parse(const QString& str){
    QString ret = str;
    ret.replace("\n", "\\n");
    ret.replace("\t", "\\t");
    ret.replace("\"", "\\\"");
    ret.replace("\'", "\\'");
    return ret;
}

QString Cpp_Parser::char_Parse(const QChar& c){
    return "\"" + str_Parse(QString(c)) + "\"";
}

void Cpp_Parser::slot_Parse(){
    raw_code = text_document->toPlainText();
    raw_code_separated = raw_code.split('\n');
    //qDebug() << raw_code_separated;
    clear_Result();
    get_Ignore_Char();
    const size_t &len = raw_code.length();
    for(unsigned int i=0; i<len; i++){
        push_Char(i, raw_code[i]);
    }
    //debug_Print_Result();
}

void Cpp_Parser::calc_Layer(const unsigned int& ind){
    slot_Parse();
    for(QPair<unsigned int, unsigned int> p : *bracket_pair[Bracket_Type::LARGE]){
        if(p.first < ind && ind <= p.second){
            current_layer++;
        }
    }
    if(is_Ignored(ind)){
        return;
    }
    if(between_Brackets(ind, Bracket_Type::LARGE)){
        extra_line = true;
    }
}

//Whether to fill brackets
bool Cpp_Parser::is_Ignored(const unsigned int& ind){
    return (ind > 0 && ignore_char[ind - 1] == true);
}

void Cpp_Parser::calc_Make_Pair(const unsigned int& ind){
    clear_Result();
    slot_Parse();
    if(is_Ignored(ind)){
        return;
    }
    size_t len = raw_code.length();
    if(ind >= len){
        make_pair = true;
    }
    if(space_characters.contains(raw_code[ind])){
        make_pair = true;
    }
}

bool Cpp_Parser::between_Brackets(const unsigned int& ind, const Bracket_Type& type){
    size_t len = raw_code.length();
    if(ind == 0 || ind >= len){
        return false;
    }
    if(raw_code[ind - 1] == bracket_names[type].left && raw_code[ind] == bracket_names[type].right){
        return true;
    }
    return false;
}

bool Cpp_Parser::between_Quotes(const unsigned int& ind, const Quote_Type& type){
    size_t len = raw_code.length();
    if(ind == 0 || ind >= len){
        return false;
    }
    if(raw_code[ind - 1] == quote_names[type].ch && raw_code[ind] == quote_names[type].ch){
        return true;
    }
    return false;
}

bool Cpp_Parser::get_Extra_Backspace(const unsigned int &ind){
    slot_Parse();
    //Between Quotes Over is_Ignored
    for(const Quote_Type &type : quote_names.keys()){
        if(between_Quotes(ind, type)){
            return true;
        }
    }
    if(is_Ignored(ind)){
        return false;
    }
    for(const Bracket_Type &type : bracket_names.keys()){
        if(between_Brackets(ind, type)){
            return true;
        }
    }
    return false;
}

int Cpp_Parser::get_Current_Layer(){
    return current_layer;
}

bool Cpp_Parser::get_Extra_Line(){
    return extra_line;
}

bool Cpp_Parser::get_Make_Pair(){
    return make_pair;
}

void Cpp_Parser::get_Ignore_Char(){
    ignore_char.fill(false, raw_code.length());
    ignore_Regular_Expression(quoteExpression, true);
    ignore_Regular_Expression(smallQuoteExpression, true);
    ignore_Regular_Expression(singleCommentExpression, false);
}

void Cpp_Parser::ignore_Regular_Expression(const QRegularExpression& expression, const bool& ignore_last){
    int start_ind = 0;
    for(QString line : raw_code_separated){
        QRegularExpressionMatchIterator it = expression.globalMatch(line);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            int start = start_ind + match.capturedStart(), end = start_ind + match.capturedEnd();
            //Set the segment of the ignore_char array as true
            memset(&ignore_char[start], true, end - start - ignore_last);
        }
        //(We need to consider \n)
        start_ind += line.length() + 1;
    }
}

void Cpp_Parser::clear_Result(){
    extra_line = false;
    make_pair = false;
    current_layer = 0;
    ignore_char.clear();
    parse_result.clear();
    for(const Bracket_Type &p : bracket_names.keys()){
        bracket_stack[p]->clear();
        bracket_pair[p]->clear();
    }
}

void Cpp_Parser::push_Char(const unsigned int& ind, const QChar& c){
    if(ignore_char[ind] == true){
        return;
    }
    if(bracket_set_left.count(c)){
        bracket_stack[bracket_set_left[c]]->push(ind);
    }
    if(bracket_set_right.count(c)){
        const Bracket_Type &type = bracket_set_right[c];
        QStack<unsigned int>* const &p = bracket_stack[type];
        if(p->empty()){
            return;
        }
        bracket_pair[type]->push_back(QPair<unsigned int, unsigned int>(p->top(), ind));
        p->pop();
    }
}

//Debug Functions
void Cpp_Parser::debug_print_Result(){
    //Show Results
    size_t len = raw_code.length();
    if(len == 0){
        return;
    }
    parse_result.append("Raw_Code:\n");
    parse_result.append(str_Parse(raw_code) + "\n");
    //Show Raw Code Index
    parse_result.append("Raw_Code_Indexed:\n");
    parse_result.append("[");
    for(unsigned int i=0; i<len - 1; i++){
        parse_result.append(QString::asprintf("(%d, ", i) + char_Parse(raw_code[i]) + "), ");
    }
    parse_result.append(QString::asprintf("(%d, ", len - 1) + char_Parse(raw_code[len - 1]) + ")]\n");
    //Show Ignored Char
    parse_result.append("Ignored_Char:\n");
    parse_result.append("[");
    for(unsigned int i=0; i<len - 1; i++){
        parse_result.append(QString::asprintf("(%d, %d)", i, ignore_char[i]) + ", ");
    }
    parse_result.append(QString::asprintf("(%d, %d)", len - 1, ignore_char[len - 1]) + "]\n");
    //Show Bracket Pairs
    for(const Bracket_Type &p : bracket_names.keys()){
        parse_result.append(bracket_names[p].name + ":\n");
        for(const QPair<unsigned int, unsigned int>& e : *bracket_pair[p]){
            parse_result.append(QString::asprintf("(%d, %d)\n", e.first, e.second));
        }
    }
}

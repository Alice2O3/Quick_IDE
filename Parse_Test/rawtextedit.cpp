#include "rawtextedit.h"

RawTextEdit::RawTextEdit(QWidget *parent) : QPlainTextEdit(parent){
    setReadOnly(true);
}

void RawTextEdit::setRawText(const QString& q){
    QString ret = q;
    ret.replace("\n", "\\n"), ret.replace("\t", "\\t");
    setPlainText(ret);
}

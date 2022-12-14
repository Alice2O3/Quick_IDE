#include "editarea.h"
#include "text_linenumberarea.h"
#include <QPainter>
#include <QTextBlock>

Text_LineNumberArea::Text_LineNumberArea(EditArea *editarea_) : QWidget(editarea_)
{
    editarea = editarea_;
}

QSize Text_LineNumberArea::sizeHint() const
{
    return QSize(editarea->lineNumberAreaWidth(), 0);
}

void Text_LineNumberArea::paintEvent(QPaintEvent *event)
{
    editarea->lineNumberAreaPaintEvent(event);
}

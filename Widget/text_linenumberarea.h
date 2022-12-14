#ifndef TEXT_LINENUMBERAREA_H
#define TEXT_LINENUMBERAREA_H

#include <QWidget>
#include "editarea.h"

class Text_LineNumberArea : public QWidget
{
    Q_OBJECT
public:
    Text_LineNumberArea(EditArea *editarea);
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);

private:
    EditArea *editarea;
};

#endif // TEXT_LINENUMBERAREA_H

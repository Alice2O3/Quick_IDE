#ifndef RAWTEXTEDIT_H
#define RAWTEXTEDIT_H

#include <QWidget>
#include <QPlainTextEdit>

class RawTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit RawTextEdit(QWidget *parent = nullptr);
    void setRawText(const QString&);

signals:

public slots:
};

#endif // RAWTEXTEDIT_H

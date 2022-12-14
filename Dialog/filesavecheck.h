#ifndef FILESAVECHECK_H
#define FILESAVECHECK_H

#include <QDebug>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QPlainTextEdit>
#include "../Widget/editarea.h"

class FileSaveCheck : public QWidget
{
public:
    virtual ~FileSaveCheck();
    static FileSaveCheck* getInstance(EditArea*);
    static bool hasInstance();
    virtual void closeEvent(QCloseEvent *event);
    QLabel *closeCheckLabel;
    QPushButton *yesCheckButton;
    QPushButton *noCheckButton;
    QPushButton *cancelCheckButton;
    EditArea *editarea;
public slots:
    void slot_yes();
    void slot_no();
private:
    static FileSaveCheck* instance;
    explicit FileSaveCheck(EditArea*, QWidget* parent = nullptr);
};

#endif // FILESAVECHECK_H

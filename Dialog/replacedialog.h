#ifndef _REPLACEDIALOG_H_
#define _REPLACEDIALOG_H_

#include "FindDialog.h"

class ReplaceDialog : public FindDialog
{
    Q_OBJECT

protected:
    QLabel m_replaceLbl;
    QLineEdit m_replaceEdit;
    QPushButton m_replaceBtn;
    QPushButton m_replaceAllBtn;
    void initControl();
    void connectSlot();
    explicit ReplaceDialog(QPlainTextEdit* pText, QWidget *parent = nullptr);

protected slots:
    void onReplaceClicked();
    void onReplaceAllClicked();

public:
    virtual void closeEvent(QCloseEvent *event);
    static ReplaceDialog* getInstance(QPlainTextEdit* e);

private:
    static ReplaceDialog* instance;
};

#endif // _REPLACEDIALOG_H_

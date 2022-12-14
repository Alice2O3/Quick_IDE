#ifndef _FINDDIALOG_H_
#define _FINDDIALOG_H_

#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QPlainTextEdit>
#include <QPointer>
#include <QString>
#include <QDebug>

class FindDialog : public QDialog
{
    Q_OBJECT

protected:
    QGroupBox m_radioGrpBx;

    QGridLayout m_layout;
    QHBoxLayout m_hbLayout;

    QLabel m_findLbl;
    QLineEdit m_findEdit;
    QPushButton m_findBtn;
    QPushButton m_findAllBtn;
    QPushButton m_closeBtn;
    QCheckBox m_matchChkBx;
    QRadioButton m_forwardBtn;
    QRadioButton m_backwardBtn;

    QPlainTextEdit* m_pText;

    QString str1;//last search
    QTextDocument::FindFlags policy = 0x0;//search policy

    void initControl();
    explicit FindDialog(QPlainTextEdit* pText, QWidget* parent = nullptr);

protected slots:
    void onFindClicked();
    void onFindAllClicked();
    void onCloseClicked();

public:
    virtual void closeEvent(QCloseEvent *event);
    void setPlainTextEdit(QPlainTextEdit* pText);
    QPlainTextEdit* getPlainTextEdit();
    static FindDialog* getInstance(QPlainTextEdit* e);

private:
    static FindDialog* instance;
};

#endif // _FINDDIALOG_H_

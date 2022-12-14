#include "FindDialog.h"
#include <QEvent>
#include <QTextCursor>
#include <QMessageBox>

FindDialog* FindDialog::instance = nullptr;
FindDialog* FindDialog::getInstance(QPlainTextEdit* e){
    if(instance == nullptr){
        instance = new FindDialog(e);
        instance->show();
    }
    return instance;
}

FindDialog::FindDialog(QPlainTextEdit* pText, QWidget *parent)
    : QDialog(parent, Qt::WindowCloseButtonHint | Qt::Drawer)
{
    initControl();
    setLayout(&m_layout);
    setWindowTitle("搜索");
    setPlainTextEdit(pText);
}

void FindDialog::initControl()
{
    m_findLbl.setText("查找:");
    m_findBtn.setText("查找下一个");
    m_findAllBtn.setText("查找全部");
    m_closeBtn.setText("取消");
    m_matchChkBx.setText("匹配大小写");
    m_backwardBtn.setText("向前");
    m_forwardBtn.setText("向后");
    m_forwardBtn.setChecked(true);
    m_radioGrpBx.setTitle("查找方向");

    m_hbLayout.addWidget(&m_forwardBtn);
    m_hbLayout.addWidget(&m_backwardBtn);

    m_radioGrpBx.setLayout(&m_hbLayout);

    m_layout.setSpacing(10);
    m_layout.addWidget(&m_findLbl, 0, 0);
    m_layout.addWidget(&m_findEdit, 0, 1);
    m_layout.addWidget(&m_findBtn, 0, 2);
    m_layout.addWidget(&m_matchChkBx, 1, 0);
    m_layout.addWidget(&m_radioGrpBx, 1, 1);
    m_layout.addWidget(&m_findAllBtn, 1, 2);

    connect(&m_findBtn, SIGNAL(clicked()), this, SLOT(onFindClicked()),Qt::UniqueConnection);
    connect(&m_findAllBtn, SIGNAL(clicked()), this, SLOT(onFindAllClicked()),Qt::UniqueConnection);
    connect(&m_closeBtn, SIGNAL(clicked()), this, SLOT(onCloseClicked()),Qt::UniqueConnection);

    str1 = "";
}

void FindDialog::setPlainTextEdit(QPlainTextEdit* pText)
{
    m_pText = pText;
}

QPlainTextEdit* FindDialog::getPlainTextEdit()
{
    return m_pText;
}

void FindDialog::onFindClicked()
{
    QString target = m_findEdit.text();

    if( (m_pText != NULL) && (target != "") )
    {
        QString text = m_pText->toPlainText();
        QTextCursor c = m_pText->textCursor();
        int index = -1;

        if( m_forwardBtn.isChecked() )
        {
            index = text.indexOf(target, c.position(), m_matchChkBx.isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

            if( index >= 0 )
            {
                c.setPosition(index);
                c.setPosition(index + target.length(), QTextCursor::KeepAnchor);

                m_pText->setTextCursor(c);
            }
        }

        if( m_backwardBtn.isChecked() )
        {
            index = text.lastIndexOf(target, c.position() - text.length() - 1, m_matchChkBx.isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

            if( index >= 0 )
            {
                c.setPosition(index + target.length());
                c.setPosition(index, QTextCursor::KeepAnchor);

                m_pText->setTextCursor(c);
            }
        }

        if( index < 0 )
        {
            QMessageBox msg(this);

            msg.setWindowTitle("查找");
            msg.setText("找不到更多 \"" + target + "\"...");
            msg.setIcon(QMessageBox::Information);
            msg.setStandardButtons(QMessageBox::Ok);

            msg.exec();
        }
    }
}

void FindDialog::onFindAllClicked()
{
    //清除上一次选中的
    QTextDocument *document1 = m_pText->document();//定义QTextDocunment对象
    QTextCursor highlight_cursor1(document1);
    QTextCharFormat color_format1(highlight_cursor1.charFormat());
    if(str1!=""){
        color_format1.setForeground(Qt::black);   //字体颜色
        color_format1.setBackground(Qt::white);  //背景颜色
        while (!highlight_cursor1.isNull())
        {
            //查找指定的文本，匹配整个单词
            highlight_cursor1 = document1->find(str1, highlight_cursor1,policy);
            if (!highlight_cursor1.isNull())
            {
                highlight_cursor1.mergeCharFormat(color_format1);
            }
        }
    }

    int flag=0;
    m_pText->moveCursor(QTextCursor::Start);
    QTextDocument *document = m_pText->document();//定义QTextDocunment对象
    bool found = false;
    QTextCursor highlight_cursor(document);
    QTextCharFormat color_format(highlight_cursor.charFormat());    

    //是否匹配大小写
    if(m_matchChkBx.isChecked()){
        policy = QTextDocument::FindCaseSensitively;
    }else{
        policy = 0x0;
    }

    QString str=m_findEdit.text();//获取输入的string
    str1 = str;
    color_format.setForeground(Qt::black);   //设置字体颜色
    color_format.setBackground(Qt::yellow);  //设置背景颜色
    while (!highlight_cursor.isNull() && !highlight_cursor.atEnd()&&str!="")
    {
        highlight_cursor = document->find(str, highlight_cursor,policy);
        if (!highlight_cursor.isNull())
        {
            if (!found)
                found = true;
            highlight_cursor.mergeCharFormat(color_format),flag++;
        }
    }
    if(flag==0&&str!="") {
        QMessageBox msg(this);
        msg.setWindowTitle("查找");
        msg.setText(tr("找不到'%1'").arg(str));
        msg.setIcon(QMessageBox::Information);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
}

void FindDialog::onCloseClicked()
{
    this->close();
}

void FindDialog::closeEvent(QCloseEvent *event){
    Q_UNUSED(event);
    instance = nullptr;
}

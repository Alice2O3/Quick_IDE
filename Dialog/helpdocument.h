#ifndef HELPDOCUMENT_H
#define HELPDOCUMENT_H
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>
#include <QTextBrowser>
#include <QHBoxLayout>

class HelpDocument : public QWidget
{
public:
    static HelpDocument* getInstance();
    virtual ~HelpDocument();
    virtual void closeEvent(QCloseEvent *event);
public slots:
    void onSelectedItem(QTreeWidgetItem *item, int index);
private:
    static HelpDocument* instance;
    explicit HelpDocument(QWidget *parent = nullptr);
    QHBoxLayout *layoutMain;
    QTreeWidget *tree;
    QTextBrowser *acceleratorkey;

    //Items
    QTreeWidgetItem *item1;
    QTreeWidgetItem *item2;
    QTreeWidgetItem *item3;
    QTreeWidgetItem *item4;

    QTreeWidgetItem *item21;
    QTreeWidgetItem *item22;
    QTreeWidgetItem *item23;
    QTreeWidgetItem *item24;

    QTreeWidgetItem *item31;
    QTreeWidgetItem *item32;
    QTreeWidgetItem *item33;
};

#endif // HELPDOCUMENT_H

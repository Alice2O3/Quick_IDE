#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QWidget>
#include <QBoxLayout>
#include <QLayout>
#include <QTabWidget>
#include <QFileDialog>
#include <QDebug>
#include <QTextCodec>
#include <string.h>
#include <QLineEdit>
#include <QTextEdit>
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTextCursor>
#include <QPlainTextEdit>
#include <QLabel>
#include <QDateTime>
#include <QTextCharFormat>
#include <QFontDialog>
#include <QToolBar>
#include "view_model.h"
#include "Dialog/finddialog.h"
#include "Dialog/replacedialog.h"
#include "Parse_Test/rawtextedit.h"
#include "Dialog/helpdocument.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    EditArea* getCurrentWidget();

private:
    Ui::MainWindow *ui;
    View_Model *view_model;

    QWidget *widgetMain;
    QTextEdit *widgetToolArea1;
    RawTextEdit *widgetToolParse;
    QTextEdit *widgetWelcomePage;
    QTextEdit *widgetWelcomePage2;//2
    //QTextEdit *textEdit;

    QTabWidget *tabwidgetEditArea;
    QTabWidget *tabwidgetToolArea;
    //Parse Test
    QPushButton *parse_button;

    QVBoxLayout *layoutMain;

    QMenu *Menu_File;
    QMenu *Menu_Edit;
    QMenu *Menu_Search;
    QMenu *Menu_Run;
    QMenu *Menu_Tool;
    QMenu *Menu_Help;

    QAction *File_New;
    QAction *File_Open;
    QAction *File_Save;
    QAction *File_SaveAs;
    QAction *File_SaveAll;
    QAction *File_Close;
    QAction *File_CloseAll;
    QAction *File_Exit;

    QAction *Edit_Undo;
    QAction *Edit_Redo;
    QAction *Edit_SelectAll;
    QAction *Edit_Copy;
    QAction *Edit_Paste;

    QAction *Search_Search;
    QAction *Search_Replace;

    QAction *Run_Compile;
    QAction *Run_Run;
    QAction *Run_CompileRun;

    QAction *Tool_EnvironmentOption;
    QAction *Tool_CompileOption;

    QAction *Help_DocumentOption;

public slots:
    void slot_addEditAreaTab(EditArea*);
    void slot_changeEditAreaTab(EditArea*);
    void slot_fileSaving();
    void slot_fileSavingAs();
    void slot_fileClose();
    void slot_fileClear();
    void slot_closeTab(int index);

    void slot_searchSearch();
    void slot_searchReplace();

    void slot_compile();
    void slot_run();
    void slot_compileRun();

    void slot_Undo();
    void slot_Redo();
    void slot_selectAll();
    void slot_Copy();
    void slot_Paste();
    //Close Program
    virtual void closeEvent(QCloseEvent *event);
    //Virtual Functions
    virtual bool eventFilter(QObject *target, QEvent *event);
    //Parse Test
    void debug_slot_Parse();

signals:
    void sig_search();
    void sig_replace();
    void signal_forceClose();

private:
    void init_Actions();
    void init_Layout();
    void init_Connect();
};

#endif // MAINWINDOW_H

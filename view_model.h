#ifndef VIEW_MODEL_H
#define VIEW_MODEL_H
#include <QFileDialog>
#include <QDebug>
#include <QTextCodec>
#include <QLineEdit>
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
#include <QQueue>
#include <QCloseEvent>
#include <QTimer>
#include "file_manager.h"
#include "file_compiler.h"
#include "Widget/editarea.h"
#include "Dialog/helpdocument.h"

class MainWindow;

class View_Model : public QWidget
{
    Q_OBJECT
public:
    static View_Model* getInstance();
    void Init(MainWindow*);
    //Components
    File_Manager* file_manager;
    File_Compiler* file_compiler;

public slots:
    void slot_addFile(EditArea*);
    void slot_removeFile(EditArea*);
    void slot_clearFile();
    void slot_saveAll();
    void slot_closeFile(EditArea*);
    void slot_fileCheck(EditArea*);
    void slot_saveQueue();
    //Help Document
    void slot_showHelp();
    //Close Event
    void slot_appClosed(QCloseEvent*);
    void slot_clearFinished();

signals:
    void signal_fileAdded(EditArea*);
    void signal_fileClosed(EditArea*);
    void signal_fileRemoved(EditArea*);
    void signal_fileSaved(EditArea*);
    void signal_fileSaveNext();
    void signal_fileCleared();
    void signal_appClosed(QCloseEvent*);
    void signal_clearFinished();

private:
    static View_Model* instance;
    explicit View_Model(QWidget *parent = nullptr);
    MainWindow* mainwindow;
    //Variables
    QSet<EditArea*> editarea_set;
    QQueue<EditArea*> save_queue;
    bool force_quit;
    QThread* close_thread;
};

#endif // VIEW_MODEL_H

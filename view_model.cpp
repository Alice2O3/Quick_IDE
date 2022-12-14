#include "mainwindow.h"
#include "Dialog/filesavecheck.h"
#include <QThread>

View_Model* View_Model::instance = nullptr;

View_Model* View_Model::getInstance(){
    if(instance == nullptr){
        instance = new View_Model;
    }
    return instance;
}

View_Model::View_Model(QWidget *parent) : QWidget(parent){

}

void View_Model::Init(MainWindow* mainwindow_){
    mainwindow = mainwindow_;
    //Set Components
    file_manager = new File_Manager;
    file_compiler = new File_Compiler;
    connect(this, &View_Model::signal_fileAdded, this, &View_Model::slot_addFile);
    connect(this, &View_Model::signal_fileRemoved, this, &View_Model::slot_removeFile);
    connect(this, &View_Model::signal_fileSaved, mainwindow, &MainWindow::slot_changeEditAreaTab);
    connect(this, &View_Model::signal_fileCleared, this, &View_Model::slot_clearFile);
    //Close File
    connect(this, &View_Model::signal_fileClosed, this, &View_Model::slot_closeFile);
    //Save Next
    connect(this, &View_Model::signal_fileSaveNext, this, &View_Model::slot_saveQueue);
    connect(this, &View_Model::signal_appClosed, this, &View_Model::slot_appClosed);
    connect(this, &View_Model::signal_clearFinished, this, &View_Model::slot_clearFinished);
    //Processing Quit
    force_quit = false;
    close_thread = new QThread();
    connect(close_thread, &QThread::started, mainwindow, &MainWindow::close);
}

void View_Model::slot_addFile(EditArea* editarea){
    editarea_set.insert(editarea);
    emit mainwindow->slot_addEditAreaTab(editarea);
}

void View_Model::slot_removeFile(EditArea* editarea){
    delete editarea;
    editarea_set.remove(editarea);
}

void View_Model::slot_clearFile(){
    if(!save_queue.empty()){
        return;
    }
    QSet<EditArea*> tmp_set = editarea_set;
    for(EditArea* e : tmp_set){
        slot_fileCheck(e);
    }
    slot_saveQueue();
}

void View_Model::slot_saveQueue(){
    if(!save_queue.empty()){
        EditArea* tmp = save_queue.front();
        save_queue.pop_front();
        file_manager->slot_fileConfirm(tmp);
    } else {
        emit signal_clearFinished();
    }
}

void View_Model::slot_clearFinished(){
    if(force_quit && editarea_set.isEmpty()){
        close_thread->start();
        return;
    }
    force_quit = false;
}

void View_Model::slot_saveAll(){
    for(EditArea* e : editarea_set){
        file_manager->slot_fileSaving(e);
    }
}

void View_Model::slot_showHelp(){
    HelpDocument::getInstance();
}

void View_Model::slot_closeFile(EditArea* editarea){
    if(!save_queue.empty()){
        return;
    }
    slot_fileCheck(editarea);
    slot_saveQueue();
}

//Check whether the file is saved before closing
void View_Model::slot_fileCheck(EditArea* editarea){
    if(editarea->get_Is_Changed()){
        save_queue.push_back(editarea);
    } else {
        emit View_Model::getInstance()->signal_fileRemoved(editarea);
    }
}

void View_Model::slot_appClosed(QCloseEvent * e){
    if(force_quit){
        e->accept();
        return;
    }
    force_quit = true;
    e->ignore();
    slot_clearFile();
}

//void View_Model::newFile()
//{

//    QMessageBox::StandardButton ok=QMessageBox::warning(this,tr("Warning"),
//    tr("创建新文件？"),
//    QMessageBox::Yes | QMessageBox::Default,
//    QMessageBox::No);//通过QMessagebox里面的东西来

//    m_fileName.clear();
//    textEdit->setText(QString());
//    if( m_fileName == ""&& ok==QMessageBox::StandardButton::Yes)
//        m_fileName = showFileDialog(QFileDialog::AcceptSave);
//}

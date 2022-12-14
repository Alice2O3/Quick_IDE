#include "view_model.h"
#include <QDebug>
#include <QDir>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>

File_Manager::File_Manager(QWidget *parent) : QWidget(parent){
    file_filter = "C/C++ source files (*.c *.cpp *.cc)";
    last_path = QDir::currentPath();
}

void File_Manager::slot_fileNew()
{
    EditArea* ret = EditArea::EditArea_Factory();
    emit View_Model::getInstance()->signal_fileAdded(ret);
}

void File_Manager::slot_fileOpen()
{
    QString read_path = QFileDialog::getOpenFileName(this, "选择要打开的文件", getUpperDirectory(last_path), file_filter);  //限制只能打开c类型文件
    if(read_path.isEmpty()){ //如果没有选择路径
        return;
    }

    last_path = read_path;
    QFile fp(last_path);
    if(!fp.open(QIODevice::ReadOnly|QIODevice::Text)){
        return;
    }

    QTextCodec *codec = QTextCodec::codecForName("gbk"); //让中文文件也能打开
    content.clear();
    while(!fp.atEnd()){
        QByteArray line = fp.readLine();
        //将读取到的行数据转换为Unicode
        QString str = codec->toUnicode(line);
        //将得到的字符添加到content中显示
        content += str;
    }
    fp.close();

    EditArea* ret = new EditArea;
    getFileInfo(ret, last_path);
    ret->setPlainText(content);
    ret->set_Saved();
    emit View_Model::getInstance()->signal_fileAdded(ret);
}

void File_Manager::getFileInfo(EditArea* editarea, const QString& path){
    QString file_name = path.split("/").last();
    QString suffix = "." + file_name.split(".").last();
    editarea->file_fullname = path;
    editarea->file_name = file_name;
    editarea->file_path = getUpperDirectory(path);
    editarea->file_suffix = suffix;
    editarea->file_name_no_suffix = file_name.mid(0, file_name.length() - suffix.length());
}

QString File_Manager::getUpperDirectory(const QString& path){
    QDir root_path = QDir(path);
    root_path.cdUp();
    return root_path.absolutePath();
}

void File_Manager::slot_fileSaving(EditArea* editarea){
    if(!editarea->pathExists()){
        //保存为
        slot_fileSavingAs(editarea);
        return;
    }

    //保存
    QString content = editarea->toPlainText();
    QFile file(editarea->file_fullname);
    if(!file.open(QFile::WriteOnly|QFile::Truncate)){
        QMessageBox::critical(this, "错误", "文件保存失败!");
        return;
    }
    QTextStream stream(&file);
    stream << content;

    //Refresh data
    editarea->set_Saved();

    file.close();
}

void File_Manager::slot_fileSavingAs(EditArea* editarea)
{
    if(editarea->pathExists()){
        last_path = editarea->file_fullname;
    }
    QString save_path = QFileDialog::getSaveFileName(this, "保存为", getUpperDirectory(last_path), file_filter);
    if(save_path == ""){
        return;
    }
    last_path = save_path;

    QFile file(last_path);
    if(!file.open(QFile::WriteOnly|QFile::Truncate)){
        QMessageBox::critical(this, "错误", "文件保存失败!");
        return;
    }

    QTextStream stream(&file);
    content = editarea->toPlainText();
    stream << content;
    file.close();

    //Refresh data
    editarea->set_Saved();
    getFileInfo(editarea, last_path);

    emit View_Model::getInstance()->signal_fileSaved(editarea);
}

void File_Manager::slot_fileConfirm(EditArea* editarea){
    FileSaveCheck::getInstance(editarea);
}

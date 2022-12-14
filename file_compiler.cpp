#include "view_model.h"
#include <QProcess>
#include <QCoreApplication>

File_Compiler::File_Compiler(QWidget *parent) : QWidget(parent){

}

QString File_Compiler::Compile(EditArea* editarea)
{
    View_Model::getInstance()->file_manager->slot_fileSaving(editarea);
    QProcess process;
    if (editarea->file_suffix == ".c") {
        process.start(QString("gcc \"") + editarea->file_fullname + QString("\" -o \"")
                       + editarea->file_path + QString("/") + editarea->file_name_no_suffix + QString(".exe\""));
    } else if (editarea->file_suffix == ".cpp") {
        process.start(QString("g++ \"") + editarea->file_fullname + QString("\" -o \"")
                       + editarea->file_path + QString("/") + editarea->file_name_no_suffix + QString(".exe\""));
    } else {
        return QString("The suffix of the file should only be '.c' or '.cpp'.");
    }
    process.waitForFinished();
    QByteArray result_qbt = process.readAllStandardError();
    QString result = QString::fromLocal8Bit(result_qbt);
    return result;
}

void File_Compiler::Run(EditArea* editarea)
{
    QString exe_full_name = editarea->file_path + QString("/") + editarea->file_name_no_suffix + QString(".exe");
    QString command = QString("\"" + exe_full_name + "\" & pause");
    QByteArray command_qbt = command.toLocal8Bit();
    system(command_qbt.data());
}

QString File_Compiler::compileRun(EditArea* editarea)
{
    QString result = Compile(editarea);
    if (result == QString("")) {
        Run(editarea);
    }
    return result;
}

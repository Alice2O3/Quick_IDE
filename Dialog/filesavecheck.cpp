#include "view_model.h"
#include "filesavecheck.h"

FileSaveCheck* FileSaveCheck::instance = nullptr;
FileSaveCheck* FileSaveCheck::getInstance(EditArea* e){
    if(instance == nullptr){
        instance = new FileSaveCheck(e);
        instance->show();
    }
    return instance;
}

bool FileSaveCheck::hasInstance(){
    return instance != nullptr;
}

FileSaveCheck::FileSaveCheck(EditArea* e, QWidget* parent) : QWidget(parent){
    editarea = e;
    this->setMaximumSize(400, 130);
    this->setMinimumSize(400, 130);
    this->resize(400, 130);
    this->setWindowTitle("提示");
    closeCheckLabel = new QLabel(this);
    closeCheckLabel->setText("当前“" + editarea->file_name + "”还未保存，是否保存？");
    closeCheckLabel->setGeometry(10, 5, 400, 50);
    yesCheckButton = new QPushButton(this);
    noCheckButton = new QPushButton(this);
    cancelCheckButton = new QPushButton(this);
    yesCheckButton->setText("是");
    noCheckButton->setText("否");
    cancelCheckButton->setText("取消");
    yesCheckButton->setGeometry(30, 60, 90, 30);
    noCheckButton->setGeometry(150, 60, 90, 30);
    cancelCheckButton->setGeometry(270, 60, 90, 30);
    QObject::connect(yesCheckButton, &QPushButton::clicked, this, &FileSaveCheck::slot_yes);
    QObject::connect(noCheckButton, &QPushButton::clicked, this, &FileSaveCheck::slot_no);
    QObject::connect(cancelCheckButton, &QPushButton::clicked, this, &FileSaveCheck::close);
}

FileSaveCheck::~FileSaveCheck(){
    delete closeCheckLabel;
    delete yesCheckButton;
    delete noCheckButton;
    delete cancelCheckButton;
}

void FileSaveCheck::slot_yes()
{
    emit View_Model::getInstance()->file_manager->slot_fileSaving(editarea);
    emit View_Model::getInstance()->signal_fileRemoved(editarea);
    this->close();
}

void FileSaveCheck::slot_no()
{
    emit View_Model::getInstance()->signal_fileRemoved(editarea);
    this->close();
}

void FileSaveCheck::closeEvent(QCloseEvent *event){
    Q_UNUSED(event);
    instance = nullptr;
    emit View_Model::getInstance()->signal_fileSaveNext();
}

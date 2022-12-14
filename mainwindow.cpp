#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    view_model = View_Model::getInstance();
    view_model->Init(this);

    init_Layout();
    init_Actions();
    init_Connect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_Actions(){
    Menu_File = menuBar()->addMenu("文件");
    Menu_Edit = menuBar()->addMenu("编辑");
    Menu_Search = menuBar()->addMenu("搜索");
    Menu_Run = menuBar()->addMenu("运行");
    Menu_Help = menuBar()->addMenu("帮助");

    File_New = new QAction("新建文件", this);
    File_New->setShortcut(tr("Ctrl+N"));
    Menu_File->addAction(File_New);

    File_Open = new QAction("打开文件", this);
    Menu_File->addAction(File_Open);

    Menu_File->addSeparator();

    File_Save = new QAction("保存此文件", this);
    File_Save->setShortcut(tr("Ctrl+S"));
    Menu_File->addAction(File_Save);

    File_SaveAs = new QAction("作为…保存此文件", this);
    Menu_File->addAction(File_SaveAs);

    File_SaveAll = new QAction("保存所有文件", this);
    Menu_File->addAction(File_SaveAll);

    Menu_File->addSeparator();

    File_Close = new QAction("关闭此文件", this);
    Menu_File->addAction(File_Close);

    File_CloseAll = new QAction("关闭所有文件", this);
    Menu_File->addAction(File_CloseAll);

    Menu_File->addSeparator();

    File_Exit = new QAction("退出", this);
    Menu_File->addAction(File_Exit);

    Edit_Undo = new QAction("撤销", this);
    Edit_Undo->setShortcut(tr("Ctrl+Z"));
    Menu_Edit->addAction(Edit_Undo);

    Edit_Redo = new QAction("重做", this);
    Edit_Redo->setShortcut(tr("Ctrl+Y"));
    Menu_Edit->addAction(Edit_Redo);

    Edit_SelectAll = new QAction("选择全部", this);
    Edit_SelectAll->setShortcut(tr("Ctrl+A"));
    Menu_Edit->addAction(Edit_SelectAll);

    Menu_Edit->addSeparator();

    Edit_Copy = new QAction("复制", this);
    Edit_Copy->setShortcut(tr("Ctrl+C"));
    Menu_Edit->addAction(Edit_Copy);

    Edit_Paste = new QAction("粘贴", this);
    Edit_Paste->setShortcut(tr("Ctrl+V"));
    Menu_Edit->addAction(Edit_Paste);

    Search_Search = new QAction("搜索", this);
    Menu_Search->addAction(Search_Search);

    Search_Replace = new QAction("替换", this);
    Menu_Search->addAction(Search_Replace);

    Run_Compile = new QAction("编译", this);
    Run_Compile->setShortcut(tr("F9"));
    Menu_Run->addAction(Run_Compile);

    Run_Run = new QAction("运行", this);
    Run_Run->setShortcut(tr("F10"));
    Menu_Run->addAction(Run_Run);

    Run_CompileRun = new QAction("编译并运行", this);
    Run_CompileRun->setShortcut(tr("F11"));
    Menu_Run->addAction(Run_CompileRun);

    Help_DocumentOption = new QAction("帮助文档", this);
    Menu_Help->addAction(Help_DocumentOption);
}

void MainWindow::init_Layout(){
    //增加搜索时的代码高亮
    QPalette p = palette();
    p.setColor(QPalette::Inactive, QPalette::Highlight, p.color(QPalette::Active, QPalette::Highlight));
    p.setColor(QPalette::Inactive, QPalette::HighlightedText, p.color(QPalette::Active, QPalette::HighlightedText));
    this->setPalette(p);

    this->resize(1200,900);
    this->setMinimumSize(600,500);

    tabwidgetEditArea = new QTabWidget();
    tabwidgetEditArea->setTabsClosable(true);
    tabwidgetEditArea->setMovable(true);
    tabwidgetEditArea->setDocumentMode(true);

    widgetToolArea1 = new QTextEdit();
    widgetToolArea1->setReadOnly(true);
    widgetToolParse = new RawTextEdit();
    tabwidgetToolArea = new QTabWidget();
    tabwidgetToolArea->setTabPosition(QTabWidget::South);
    tabwidgetToolArea->addTab(widgetToolArea1,"编译结果");
    tabwidgetToolArea->setCurrentWidget(widgetToolArea1);

    layoutMain = new QVBoxLayout();
    layoutMain->addWidget(tabwidgetEditArea);
    layoutMain->addWidget(tabwidgetToolArea);

    tabwidgetEditArea->setMinimumHeight(250);
    tabwidgetToolArea->setMaximumHeight(250);
    tabwidgetToolArea->setMinimumHeight(150);

    widgetMain = new QWidget();
    widgetMain->setLayout(layoutMain);
    this->setCentralWidget(widgetMain);
}

void MainWindow::init_Connect(){
    connect(File_New, &QAction::triggered, view_model->file_manager, &File_Manager::slot_fileNew);
    connect(File_Open, &QAction::triggered, view_model->file_manager, &File_Manager::slot_fileOpen);
    connect(File_SaveAll, &QAction::triggered, view_model, &View_Model::slot_saveAll);
    connect(File_Save, &QAction::triggered, this, &MainWindow::slot_fileSaving);
    connect(File_SaveAs, &QAction::triggered, this, &MainWindow::slot_fileSavingAs);
    connect(File_Close, &QAction::triggered, this, &MainWindow::slot_fileClose); //关闭当前文件
    connect(File_CloseAll, &QAction::triggered, this, &MainWindow::slot_fileClear); //关闭所有文件
    connect(tabwidgetEditArea, QTabWidget::tabCloseRequested, this, &MainWindow::slot_closeTab); //从tab栏上的关闭按钮关掉tab文件
    connect(File_Exit, &QAction::triggered, this, &MainWindow::close);
    //Undo and Redo
    connect(Edit_Undo, &QAction::triggered, this, &MainWindow::slot_Undo);
    connect(Edit_Redo, &QAction::triggered, this, &MainWindow::slot_Redo);
    connect(Edit_SelectAll, &QAction::triggered, this, &MainWindow::slot_selectAll);
    connect(Edit_Copy, &QAction::triggered, this, &MainWindow::slot_Copy);
    connect(Edit_Paste, &QAction::triggered, this, &MainWindow::slot_Paste);
    //Find and Replace
    connect(Search_Search, &QAction::triggered, this, &MainWindow::slot_searchSearch);
    connect(Search_Replace, &QAction::triggered, this, &MainWindow::slot_searchReplace);
    //Compile
    connect(Run_Compile, &QAction::triggered, this, &MainWindow::slot_compile);
    connect(Run_Run, &QAction::triggered, this, &MainWindow::slot_run);
    connect(Run_CompileRun, &QAction::triggered, this, &MainWindow::slot_compileRun);
    //Help Document
    connect(Help_DocumentOption, &QAction::triggered, view_model, &View_Model::slot_showHelp);

    this->installEventFilter(this);
}

//Functions
EditArea* MainWindow::getCurrentWidget(){
    return static_cast<EditArea*>(tabwidgetEditArea->currentWidget());
}

void MainWindow::slot_addEditAreaTab(EditArea* editarea){
    tabwidgetEditArea->addTab(editarea, editarea->file_name);
}

void MainWindow::slot_changeEditAreaTab(EditArea* editarea){
    tabwidgetEditArea->setTabText(tabwidgetEditArea->indexOf(editarea), editarea->file_name);
}

void MainWindow::slot_closeTab(int index)
{ //关闭文件的槽2，接收关闭tab的index
    EditArea* current_editarea = static_cast<EditArea*>(tabwidgetEditArea->widget(index));
    emit view_model->signal_fileClosed(current_editarea);
}

void MainWindow::slot_searchSearch()
{//处理点击搜索信号的中转
    EditArea* current_editarea = getCurrentWidget();
    if(current_editarea == nullptr){
        return;
    }
    FindDialog::getInstance((QPlainTextEdit*)current_editarea);
}

void MainWindow::slot_searchReplace()
{//处理点击替换信号的中转
    EditArea* current_editarea = getCurrentWidget();
    if(current_editarea == nullptr){
        return;
    }
    ReplaceDialog::getInstance((QPlainTextEdit*)current_editarea);
}

//Remove null pointer check
void MainWindow::slot_fileSaving(){
    EditArea* current_editarea = getCurrentWidget();
    if(current_editarea == nullptr){
        return;
    }
    view_model->file_manager->slot_fileSaving(current_editarea);
}

void MainWindow::slot_fileSavingAs(){
    EditArea* current_editarea = getCurrentWidget();
    if(current_editarea == nullptr){
        return;
    }
    view_model->file_manager->slot_fileSavingAs(current_editarea);
}

void MainWindow::slot_fileClose()
{ //关闭文件的槽
    EditArea* current_editarea = getCurrentWidget();
    if(current_editarea == nullptr){
        return;
    }
    emit view_model->signal_fileClosed(current_editarea);
}

void MainWindow::slot_fileClear()
{ //关闭所有文件的槽
    emit view_model->signal_fileCleared();
}

//Compile Functions
void MainWindow::slot_compile()
{
    EditArea* current_editarea = getCurrentWidget();
    if (current_editarea == nullptr) {
        return;
    }
    QString compile_result = view_model->file_compiler->Compile(current_editarea);
    widgetToolArea1->setText(compile_result);
}

void MainWindow::slot_run()
{
    EditArea* current_editarea = getCurrentWidget();
    if (current_editarea == nullptr) {
        return;
    }
    view_model->file_compiler->Run(current_editarea);
}

void MainWindow::slot_compileRun()
{
    EditArea* current_editarea = getCurrentWidget();
    if (current_editarea == nullptr) {
        return;
    }
    QString compile_result = view_model->file_compiler->compileRun(current_editarea);
    widgetToolArea1->setText(compile_result);
}

//Edit Functions
void MainWindow::slot_Undo()
{
    EditArea* current_editarea = getCurrentWidget();
    if (current_editarea == nullptr) {
        return;
    }
    current_editarea->undo();
}

void MainWindow::slot_Redo()
{
    EditArea* current_editarea = getCurrentWidget();
    if (current_editarea == nullptr) {
        return;
    }
    current_editarea->redo();
}

void MainWindow::slot_selectAll()
{
    EditArea* current_editarea = getCurrentWidget();
    if (current_editarea == nullptr) {
        return;
    }
    current_editarea->selectAll();
}

void MainWindow::slot_Copy()
{
    EditArea* current_editarea = getCurrentWidget();
    if (current_editarea == nullptr) {
        return;
    }
    current_editarea->copy();
}

void MainWindow::slot_Paste()
{
    EditArea* current_editarea = getCurrentWidget();
    if (current_editarea == nullptr) {
        return;
    }
    current_editarea->paste();
}

//Custom Close Event
void MainWindow::closeEvent(QCloseEvent *e){
    emit view_model->signal_appClosed(e);
}

//Custom Shortcut Event
bool MainWindow::eventFilter(QObject *target, QEvent *event){
    Q_UNUSED(target);
    Q_UNUSED(event);
    return false;
}

//Parser Test Functions
void MainWindow::debug_slot_Parse(){
    qDebug() << "Parse";
    EditArea* current_editarea = getCurrentWidget();
    if (current_editarea == nullptr) {
        return;
    }
    widgetToolParse->setPlainText(current_editarea->debug_slot_Parse_Test());
}

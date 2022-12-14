#include "helpdocument.h"
#include <QDebug>

HelpDocument* HelpDocument::instance = nullptr;
HelpDocument* HelpDocument::getInstance(){
    if(instance == nullptr){
        instance = new HelpDocument;
        instance->show();
    }
    return instance;
}

HelpDocument::HelpDocument(QWidget *parent) : QWidget(parent)
{
    this->setMinimumSize(1000, 500);
    this->setMaximumSize(1000, 500);
    this->resize(1000, 500);
    this->setWindowTitle("帮助文档");

    //创建QTreeWidget对象，指定父对象
    tree = new QTreeWidget();

    tree->setHeaderLabel("功能导航");
    tree->setHeaderHidden(true);
    //设置不同层次菜单的缩进
    tree->setIndentation(10);

    //Items
    item1 = new QTreeWidgetItem(tree,QStringList("○欢迎"));
    item2 = new QTreeWidgetItem(tree,QStringList("○界面功能"));
    item3 = new QTreeWidgetItem(tree,QStringList("○基本功能"));
    item4 = new QTreeWidgetItem(tree,QStringList("○快捷键"));

    item21 = new QTreeWidgetItem(item2,QStringList("    文件"));
    item22 = new QTreeWidgetItem(item2,QStringList("    编辑"));
    item23 = new QTreeWidgetItem(item2,QStringList("    搜索"));
    item24 = new QTreeWidgetItem(item2,QStringList("    工具"));

    item31 = new QTreeWidgetItem(item3,QStringList("    编译"));
    item32 = new QTreeWidgetItem(item3,QStringList("    运行"));
    item33 = new QTreeWidgetItem(item3,QStringList("    编译运行"));

    acceleratorkey = new QTextBrowser();
    acceleratorkey->setFontPointSize(15);
    acceleratorkey->setText("\n"
                     "\n"
                     "\n"
                     "\n"
                     "    欢迎使用LIGHT IDE帮助文档！本文档用于帮助您了解该IDE的界面功能和其他功能以更好地使用IDE。您可通过左侧树状列表获取您需要的帮助内容。\n"
                     "    如果您在使用过程中遇到其他问题无法解决，欢迎您通过邮箱xxxxxx向我们反馈。\n");

    QObject::connect(tree, &QTreeWidget::itemClicked, this, &HelpDocument::onSelectedItem, Qt::UniqueConnection);

    layoutMain = new QHBoxLayout(this);
    layoutMain->addWidget(tree);
    layoutMain->addWidget(acceleratorkey);
    layoutMain->setStretch(0, 1);
    layoutMain->setStretch(1, 3);

    this->setLayout(layoutMain);
}

HelpDocument::~HelpDocument(){
    delete tree;
    delete acceleratorkey;
    delete layoutMain;
}

void HelpDocument::closeEvent(QCloseEvent *event){
    Q_UNUSED(event);
    instance = nullptr;
}

void HelpDocument::onSelectedItem(QTreeWidgetItem* item, int index)
{
    Q_UNUSED(index);
    if(item == item1)
    {
        acceleratorkey->setText("\n"
                                "\n"
                                "\n"
                                "\n"
                                "    欢迎使用LIGHT IDE帮助文档！本文档用于帮助您了解该IDE的界面功能和其他功能以更好地使用IDE。您可通过左侧树状列表获取您需要的帮助内容。\n"
                                "    如果您在使用过程中遇到其他问题无法解决，欢迎您通过邮箱xxxxxx向我们反馈。\n");
    }
    if(item == item21)
    {
        acceleratorkey->setText("\n"
                                "\n"
                                "\n"
                                "                       文件操作\n"
                                "    文件操作包含新建文件，打开文件和保存文件。\n"
                                "    新建文件：创建一个新的文件，同时选择创建文件的类型和名字。下一步选择文件保存的位置，点击确定后IDE生成对应文件，并给出main函数框架。\n"
                                "    打开文件：打开窗口，选择已有的文件，点击确定后打开对应文件。\n"
                                "    保存文件：保存当前的文件，可以选择保存或者另存为，选择要保存的位置，点击确定后保存对应文件。\n");
    }
    if(item == item22)
    {
        acceleratorkey->setText("\n"
                                "\n"
                                "\n"
                                "                       编辑操作\n"
                                "    代码文本编辑包含复制、剪切、粘贴、删除、退格、撤销以及取消撤销。在代码文本编辑功能，用户可以利用以上功能完成对代码文本的编辑。\n"
                                "    复制：将选中的文本内容保存至粘贴板，被选中部分仍存在且标蓝。\n"
                                "    剪切：将选中的文本内容保存至粘贴板，被选中部分被删除。\n"
                                "    粘贴：将粘贴板中最新一次的粘贴内容添加到光标所在处，或取代已选中的文本部分。\n"
                                "    删除：每执行一次就删除光标的后一个字符，或删除已选中的文本部分。\n"
                                "    退格：每执行一次就删除光标的前一个字符，或删除已选中的文本部分。\n"
                                "    撤销：取消上一步的操作。\n"
                                "    取消撤销：将被撤销的操作恢复。\n");
    }
    if(item == item23)
    {
        acceleratorkey->setText("\n"
                                "\n"
                                "\n"
                                "                       搜索功能\n"
                                "    搜索功能，包含搜索和替换。\n"
                                "    搜索：选择搜索选项，显示搜索窗口，在窗口的输入框中输入想要搜索的文本内容（可选择搜索时区分大小写或否，可选择搜索方向是向前或向后，可选择搜索区域为全部文本或当前选中部分文本），点击窗口中搜索按钮后会将符合要求的第一处文本标蓝，第二次点击将符合要求的第二处文本标蓝，以此类推。\n"
                                "    替换：选择替换选项，显示替换窗口，在窗口的两个输入框中分别输入想要搜索的文本内容和替换为的文本内容（可选择搜索时区分大小写或否，可选择搜索方向是向前或向后，可选择搜索区域为全部文本或当前选中部分文本），点击窗口中搜索按钮后会将符合要求的第一处文本改为想要替换为的内容并对第二处需要替换的文本标蓝，第二次点击将符合要求的第二处文本替换并对第三处需要替换的文本标蓝，以此类推。\n");
    }
    if(item == item24)
    {
        acceleratorkey->setText("\n"
                                "\n"
                                "\n"
                                "                       工具功能\n"
                                "    工具包含环境选项和编译选项，通过工具功能，可以完成对编译环境、编辑器和编译过程等一系列设置。点击环境选项，可以选择关联文件格式和编辑器外观功能有关的属性。点击编译选项，可以选择编译器和构建过程命令等与编译相关的属性。\n");
    }
    if(item == item31)
    {
        acceleratorkey->setText("\n"
                                "\n"
                                "\n"
                                "                       编译功能\n"
                                "    当用户点击编译时，IDE会执行项目编译，在下方的编译输出区域显示编译器反馈的提示代码。完成编译后返回编译结果（成功/失败）和编译信息。");
    }
    if(item == item32)
    {
        acceleratorkey->setText("\n"
                                "\n"
                                "\n"
                                "                       运行功能\n"
                                "    当用户点击运行时，会查找编译目录下是否有已生成的可执行文件，如有，则运行可执行文件，若无，则弹窗提示用户是否选择执行编译，完成后再运行可执行文件。\n");
    }
    if(item == item33)
    {
        acceleratorkey->setText("\n"
                                "\n"
                                "\n"
                                "                      编译运行功能\n"
                                "    当用户点击编译运行时，IDE会先对项目代码进行编译操作，若代码和上次编译时没有区别则直接运行生成的可执行文件并显示控制窗口。\n");
    }
    if(item == item4)
    {
        acceleratorkey->setText("\n"
                         "\n"
                         "           快捷键\n\n"
                         "新建文件           Ctrl+N\n保存文件           Ctrl+S\n撤销               Ctrl+Z\n重做               Ctrl+Y\n选择全部           Ctrl+A\n复制               Ctrl+C\n粘贴               Ctrl+V\n编译               F9\n运行               F10\n编译并运行         F11\n\n");
    }
}



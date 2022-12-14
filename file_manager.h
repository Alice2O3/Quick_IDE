#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <QObject>
#include <QDir>
#include "Widget/editarea.h"
#include "Dialog/filesavecheck.h"

namespace File_Manager_Global{
    const size_t buffer_size = 1024;
};

class File_Manager : public QWidget
{
    Q_OBJECT
public:
    explicit File_Manager(QWidget *parent = nullptr);
    static void getFileInfo(EditArea* editarea, const QString& path);
    static QString getUpperDirectory(const QString& path);

public slots:
    void slot_fileNew();
    void slot_fileOpen();
    void slot_fileSaving(EditArea*);
    void slot_fileSavingAs(EditArea*);
    void slot_fileConfirm(EditArea*);

private:
    QString last_path;
    QString content;
    char buffer[File_Manager_Global::buffer_size]{};
    QString file_filter;
};

#endif // FILE_MANAGER_H

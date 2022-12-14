#ifndef FILE_COMPILER_H
#define FILE_COMPILER_H

#include <QWidget>
#include "Widget/editarea.h"

class File_Compiler : public QWidget
{
    Q_OBJECT
public:
    explicit File_Compiler(QWidget *parent = nullptr);

public slots:
    QString Compile(EditArea*);
    void Run(EditArea*);
    QString compileRun(EditArea*);
};

#endif // FILE_COMPILER_H

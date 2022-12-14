#-------------------------------------------------
#
# Project created by QtCreator 2022-08-26T09:38:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Quick_IDE
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    view_model.cpp \
    file_manager.cpp \
    Widget/editarea.cpp \
    file_compiler.cpp \
    Widget/text_highlighter.cpp \
    Widget/text_linenumberarea.cpp \
    Dialog/finddialog.cpp \
    Dialog/replacedialog.cpp \
    Parse_Test/rawtextedit.cpp \
    Widget/cpp_parser.cpp \
    Dialog/helpdocument.cpp \
    Dialog/filesavecheck.cpp

HEADERS += \
        mainwindow.h \
    view_model.h \
    file_manager.h \
    Widget/editarea.h \
    file_compiler.h \
    Widget/text_highlighter.h \
    Widget/text_linenumberarea.h \
    Dialog/finddialog.h \
    Dialog/replacedialog.h \
    Parse_Test/rawtextedit.h \
    Widget/cpp_parser.h \
    Dialog/helpdocument.h \
    Dialog/filesavecheck.h

FORMS += \
        mainwindow.ui

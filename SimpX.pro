QT       += core gui opengl gamepad

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# LIBS += -lopengl32

INCLUDEPATH += src src/code_editor/include

SOURCES += \
    src/code_editor/src/internal/QCXXHighlighter.cpp \
    src/code_editor/src/internal/QCodeEditor.cpp \
    src/code_editor/src/internal/QFramedTextAttribute.cpp \
    src/code_editor/src/internal/QGLSLCompleter.cpp \
    src/code_editor/src/internal/QGLSLHighlighter.cpp \
    src/code_editor/src/internal/QJSONHighlighter.cpp \
    src/code_editor/src/internal/QLanguage.cpp \
    src/code_editor/src/internal/QLineNumberArea.cpp \
    src/code_editor/src/internal/QLuaCompleter.cpp \
    src/code_editor/src/internal/QLuaHighlighter.cpp \
    src/code_editor/src/internal/QPythonCompleter.cpp \
    src/code_editor/src/internal/QPythonHighlighter.cpp \
    src/code_editor/src/internal/QStyleSyntaxHighlighter.cpp \
    src/code_editor/src/internal/QSyntaxStyle.cpp \
    src/code_editor/src/internal/QXMLHighlighter.cpp \
    src/fileset.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/ppuwidget.cpp \
    src/simp4completer.cpp \
    src/simp4highlighter.cpp \
    src/simpleton4.cpp \
    src/simpleton4asm.cpp \
    src/simpleton4disasm.cpp \
    src/simpx.cpp \
    src/utils.cpp

HEADERS += \
    src/code_editor/include/QCXXHighlighter \
    src/code_editor/include/QCodeEditor \
    src/code_editor/include/QFramedTextAttribute \
    src/code_editor/include/QGLSLCompleter \
    src/code_editor/include/QGLSLHighlighter \
    src/code_editor/include/QHighlightBlockRule \
    src/code_editor/include/QHighlightRule \
    src/code_editor/include/QJSONHighlighter \
    src/code_editor/include/QLanguage \
    src/code_editor/include/QLineNumberArea \
    src/code_editor/include/QLuaCompleter \
    src/code_editor/include/QLuaHighlighter \
    src/code_editor/include/QPythonCompleter \
    src/code_editor/include/QPythonHighlighter \
    src/code_editor/include/QStyleSyntaxHighlighter \
    src/code_editor/include/QSyntaxStyle \
    src/code_editor/include/QXMLHighlighter \
    src/code_editor/include/internal/QCXXHighlighter.hpp \
    src/code_editor/include/internal/QCodeEditor.hpp \
    src/code_editor/include/internal/QFramedTextAttribute.hpp \
    src/code_editor/include/internal/QGLSLCompleter.hpp \
    src/code_editor/include/internal/QGLSLHighlighter.hpp \
    src/code_editor/include/internal/QHighlightBlockRule.hpp \
    src/code_editor/include/internal/QHighlightRule.hpp \
    src/code_editor/include/internal/QJSONHighlighter.hpp \
    src/code_editor/include/internal/QLanguage.hpp \
    src/code_editor/include/internal/QLineNumberArea.hpp \
    src/code_editor/include/internal/QLuaCompleter.hpp \
    src/code_editor/include/internal/QLuaHighlighter.hpp \
    src/code_editor/include/internal/QPythonCompleter.hpp \
    src/code_editor/include/internal/QPythonHighlighter.hpp \
    src/code_editor/include/internal/QStyleSyntaxHighlighter.hpp \
    src/code_editor/include/internal/QSyntaxStyle.hpp \
    src/code_editor/include/internal/QXMLHighlighter.hpp \
    src/fileset.h \
    src/mainwindow.h \
    src/ppuwidget.h \
    src/simp4completer.h \
    src/simp4highlighter.h \
    src/simpleton4.h \
    src/simpleton4asm.h \
    src/simpleton4disasm.h \
    src/simpx.h \
    src/utils.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    SimpX_en_GB.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md \
    README_simpleton_en.md

RESOURCES += \
    resources/main.qrc \
    resources/qcodeeditor_resources.qrc

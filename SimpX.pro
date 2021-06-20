QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# LIBS += -lopengl32
INCLUDEPATH += code_editor/include

SOURCES += \
    code_editor/src/internal/QCXXHighlighter.cpp \
    code_editor/src/internal/QCodeEditor.cpp \
    code_editor/src/internal/QFramedTextAttribute.cpp \
    code_editor/src/internal/QGLSLCompleter.cpp \
    code_editor/src/internal/QGLSLHighlighter.cpp \
    code_editor/src/internal/QJSONHighlighter.cpp \
    code_editor/src/internal/QLanguage.cpp \
    code_editor/src/internal/QLineNumberArea.cpp \
    code_editor/src/internal/QLuaCompleter.cpp \
    code_editor/src/internal/QLuaHighlighter.cpp \
    code_editor/src/internal/QPythonCompleter.cpp \
    code_editor/src/internal/QPythonHighlighter.cpp \
    code_editor/src/internal/QStyleSyntaxHighlighter.cpp \
    code_editor/src/internal/QSyntaxStyle.cpp \
    code_editor/src/internal/QXMLHighlighter.cpp \
    fileset.cpp \
    main.cpp \
    mainwindow.cpp \
    ppuwidget.cpp \
    simp4completer.cpp \
    simp4highlighter.cpp \
    simpleton4.cpp \
    simpleton4asm.cpp \
    simpleton4disasm.cpp \
    simpx.cpp \
    utils.cpp

HEADERS += \
    code_editor/include/QCXXHighlighter \
    code_editor/include/QCodeEditor \
    code_editor/include/QFramedTextAttribute \
    code_editor/include/QGLSLCompleter \
    code_editor/include/QGLSLHighlighter \
    code_editor/include/QHighlightBlockRule \
    code_editor/include/QHighlightRule \
    code_editor/include/QJSONHighlighter \
    code_editor/include/QLanguage \
    code_editor/include/QLineNumberArea \
    code_editor/include/QLuaCompleter \
    code_editor/include/QLuaHighlighter \
    code_editor/include/QPythonCompleter \
    code_editor/include/QPythonHighlighter \
    code_editor/include/QStyleSyntaxHighlighter \
    code_editor/include/QSyntaxStyle \
    code_editor/include/QXMLHighlighter \
    code_editor/include/internal/QCXXHighlighter.hpp \
    code_editor/include/internal/QCodeEditor.hpp \
    code_editor/include/internal/QFramedTextAttribute.hpp \
    code_editor/include/internal/QGLSLCompleter.hpp \
    code_editor/include/internal/QGLSLHighlighter.hpp \
    code_editor/include/internal/QHighlightBlockRule.hpp \
    code_editor/include/internal/QHighlightRule.hpp \
    code_editor/include/internal/QJSONHighlighter.hpp \
    code_editor/include/internal/QLanguage.hpp \
    code_editor/include/internal/QLineNumberArea.hpp \
    code_editor/include/internal/QLuaCompleter.hpp \
    code_editor/include/internal/QLuaHighlighter.hpp \
    code_editor/include/internal/QPythonCompleter.hpp \
    code_editor/include/internal/QPythonHighlighter.hpp \
    code_editor/include/internal/QStyleSyntaxHighlighter.hpp \
    code_editor/include/internal/QSyntaxStyle.hpp \
    code_editor/include/internal/QXMLHighlighter.hpp \
    fileset.h \
    mainwindow.h \
    ppuwidget.h \
    simp4completer.h \
    simp4highlighter.h \
    simpleton4.h \
    simpleton4asm.h \
    simpleton4disasm.h \
    simpx.h \
    utils.h

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

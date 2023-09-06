QT       += core gui

QT += widgets

CONFIG += c++17

SOURCES += \
    $$PWD/src/ActivationFunc.cpp \
    $$PWD/src/ConstructorWidget.cpp \
    $$PWD/src/GlobalSettingsWidget.cpp \
    $$PWD/src/InitializerFunc.cpp \
    $$PWD/src/MainWidget.cpp \
    $$PWD/src/NNLayerParams.cpp \
    $$PWD/src/NNLayerWidget.cpp \
    $$PWD/src/NNParam.cpp \
    $$PWD/src/Optimizer.cpp \
    $$PWD/src/ParamWidget.cpp \
    $$PWD/src/ParamsEditorWidget.cpp \
    $$PWD/src/Scheduler.cpp \
    $$PWD/src/main.cpp

HEADERS += \
    $$PWD/src/ActivationFunc.h \
    $$PWD/src/ConstructorWidget.h \
    $$PWD/src/GlobalSettingsWidget.h \
    $$PWD/src/InitializerFunc.h \
    $$PWD/src/MainWidget.h \
    $$PWD/src/NNLayerParams.h \
    $$PWD/src/NNLayerWidget.h \
    $$PWD/src/NNParam.h \
    $$PWD/src/Optimizer.h \
    $$PWD/src/ParamWidget.h \
    $$PWD/src/ParamsEditorWidget.h \
    $$PWD/src/Scheduler.h

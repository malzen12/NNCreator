QT       += core gui

QT += widgets

CONFIG += c++17

SOURCES += \
    $$PWD/src/ConstructorWidget.cpp \
    $$PWD/src/GlobalSettingsWidget.cpp \
    $$PWD/src/MainWidget.cpp \
    $$PWD/src/NNLayerWidget.cpp \
    $$PWD/src/Optimizer.cpp \
    $$PWD/src/ParamWidget.cpp \
    $$PWD/src/ParamsEditorWidget.cpp \
    $$PWD/src/Scheduler.cpp \
    $$PWD/src/main.cpp \
    $$PWD/src/LayerParams/ActivationFunc.cpp \
    $$PWD/src/LayerParams/InitializerFunc.cpp \
    $$PWD/src/LayerParams/NNParam.cpp \
    $$PWD/src/LayerParams/NNLayerParams.cpp \
    src/LayerParams/ActivationLayerParams.cpp \
    src/LayerParams/ConcatinateLayerParams.cpp \
    src/LayerParams/Conv1dLayerParams.cpp \
    src/LayerParams/EmbeddingLayerParams.cpp \
    src/LayerParams/LinearLayerParams.cpp \
    src/LayerParams/NormalizationLayerParams.cpp \
    src/LayerParams/PoolLayerParams.cpp \
    src/LayerParams/ReshapeLayerParams.cpp

HEADERS += \
    $$PWD/src/ConstructorWidget.h \
    $$PWD/src/GlobalSettingsWidget.h \
    $$PWD/src/MainWidget.h \
    $$PWD/src/NNLayerWidget.h \
    $$PWD/src/Optimizer.h \
    $$PWD/src/ParamWidget.h \
    $$PWD/src/ParamsEditorWidget.h \
    $$PWD/src/Scheduler.h \
    $$PWD/src/LayerParams/ActivationFunc.h \
    $$PWD/src/LayerParams/InitializerFunc.h \
    $$PWD/src/LayerParams/NNParam.h \
    $$PWD/src/LayerParams/NNLayerParams.h \
    src/LayerParams/ActivationLayerParams.h \
    src/LayerParams/ConcatinateLayerParams.h \
    src/LayerParams/Conv1dLayerParams.h \
    src/LayerParams/EmbeddingLayerParams.h \
    src/LayerParams/LinearLayerParams.h \
    src/LayerParams/NormalizationLayerParams.h \
    src/LayerParams/PoolLayerParams.h \
    src/LayerParams/ReshapeLayerParams.h

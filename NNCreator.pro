QT       += core gui

QT += widgets

CONFIG += c++17

INCLUDEPATH += \
                $$PWD/src \
                $$PWD/src/LayerParams \
                $$PWD/src/EnumParams

SOURCES += \
    $$PWD/src/ConstructorWidget.cpp \
    $$PWD/src/GlobalSettingsWidget.cpp \
    $$PWD/src/MainWidget.cpp \
    $$PWD/src/NNLayerWidget.cpp \
    $$PWD/src/ParamWidget.cpp \
    $$PWD/src/ParamsEditorWidget.cpp \
    $$PWD/src/main.cpp \
    $$PWD/src/EnumParams/ActivationFunc.cpp \
    $$PWD/src/EnumParams/Dropout.cpp \
    $$PWD/src/EnumParams/InitializerFunc.cpp \
    $$PWD/src/EnumParams/Normalization.cpp \
    $$PWD/src/EnumParams/Optimizer.cpp \
    $$PWD/src/EnumParams/Scheduler.cpp \
    $$PWD/src/LayerParams/NNParam.cpp \
    $$PWD/src/LayerParams/NNLayerParams.cpp \
    $$PWD/src/LayerParams/ActivationLayerParams.cpp \
    $$PWD/src/LayerParams/ConcatinateLayerParams.cpp \
    $$PWD/src/LayerParams/Conv1dLayerParams.cpp \
    $$PWD/src/LayerParams/DropoutLayerParams.cpp \
    $$PWD/src/LayerParams/EmbeddingLayerParams.cpp \
    $$PWD/src/LayerParams/LinearLayerParams.cpp \
    $$PWD/src/LayerParams/NormalizationLayerParams.cpp \
    $$PWD/src/LayerParams/PoolLayerParams.cpp \
    $$PWD/src/LayerParams/ReshapeLayerParams.cpp \
    $$PWD/src/LayerParams/Conv2dLayerParams.cpp \
    src/EnumParams/Recurrent.cpp \
    src/LayerParams/FlattenLayerParams.cpp \
    src/LayerParams/Pool2dLayerParams.cpp \
    src/LayerParams/RecurrentLayerParams.cpp

HEADERS += \
    $$PWD/src/ConstructorWidget.h \
    $$PWD/src/GlobalSettingsWidget.h \
    $$PWD/src/MainWidget.h \
    $$PWD/src/NNLayerWidget.h \
    $$PWD/src/ParamWidget.h \
    $$PWD/src/ParamsEditorWidget.h \
    $$PWD/src/EnumParams/ActivationFunc.h \
    $$PWD/src/EnumParams/Dropout.h \
    $$PWD/src/EnumParams/InitializerFunc.h \
    $$PWD/src/EnumParams/Normalization.h \
    $$PWD/src/EnumParams/Optimizer.h \
    $$PWD/src/EnumParams/Scheduler.h \
    $$PWD/src/LayerParams/NNParam.h \
    $$PWD/src/LayerParams/NNLayerParams.h \
    $$PWD/src/LayerParams/ActivationLayerParams.h \
    $$PWD/src/LayerParams/ConcatinateLayerParams.h \
    $$PWD/src/LayerParams/Conv1dLayerParams.h \
    $$PWD/src/LayerParams/DropoutLayerParams.h \
    $$PWD/src/LayerParams/EmbeddingLayerParams.h \
    $$PWD/src/LayerParams/LinearLayerParams.h \
    $$PWD/src/LayerParams/NormalizationLayerParams.h \
    $$PWD/src/LayerParams/PoolLayerParams.h \
    $$PWD/src/LayerParams/ReshapeLayerParams.h \
    $$PWD/src/LayerParams/Conv2dLayerParams.h \
    src/EnumParams/Recurrent.h \
    src/LayerParams/FlattenLayerParams.h \
    src/LayerParams/Pool2dLayerParams.h \
    src/LayerParams/RecurrentLayerParams.h

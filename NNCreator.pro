QT       += core gui

QT += widgets

CONFIG += c++17

INCLUDEPATH += \
                $$PWD/src \
                $$PWD/src/LayerParams \
                $$PWD/src/LayerParams/Linear \
                $$PWD/src/LayerParams/Convolution \
                $$PWD/src/EnumParams

SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/MainWidget.cpp \
    $$PWD/src/GlobalSettingsWidget.cpp \
    $$PWD/src/CreateNNWidget.cpp \
    $$PWD/src/ConstructorWidget.cpp \
    $$PWD/src/ParamsEditorWidget.cpp \
    $$PWD/src/GlobalWarningWidget.cpp \
    $$PWD/src/ConstructorWorkSpace.cpp \
    $$PWD/src/ConstructorWidgetHelper.cpp \
    $$PWD/src/Branch.cpp \
    $$PWD/src/ParamWidget.cpp \
    $$PWD/src/NNWidget.cpp \
    $$PWD/src/NNLayerWidget.cpp \
    $$PWD/src/NNAdapterWidget.cpp \
    $$PWD/src/EnumParams/ActivationFunc.cpp \
    $$PWD/src/EnumParams/Dropout.cpp \
    $$PWD/src/EnumParams/InitializerFunc.cpp \
    $$PWD/src/EnumParams/Normalization.cpp \
    $$PWD/src/EnumParams/Optimizer.cpp \
    $$PWD/src/EnumParams/Scheduler.cpp \
    $$PWD/src/EnumParams/PaddingMode.cpp \
    $$PWD/src/EnumParams/Recurrent.cpp \
    $$PWD/src/LayerParams/FabricLayer.cpp \
    $$PWD/src/LayerParams/FabricAdapter.cpp \
    $$PWD/src/LayerParams/FabricManager.cpp \
    $$PWD/src/LayerParams/IFabric.cpp \
    $$PWD/src/LayerParams/NNParam.cpp \
    $$PWD/src/LayerParams/NNLayerParams.cpp \
    $$PWD/src/LayerParams/Linear/LinearLayerParams.cpp \
    $$PWD/src/LayerParams/Linear/BilinearLayerParams.cpp \
    $$PWD/src/LayerParams/Convolution/Conv1dLayerParams.cpp \
    $$PWD/src/LayerParams/Convolution/Conv2dLayerParams.cpp \
    $$PWD/src/LayerParams/Convolution/Conv3dLayerParams.cpp


HEADERS += \
    $$PWD/src/MainWidget.h \
    $$PWD/src/Macros.h \
    $$PWD/src/GlobalSettingsWidget.h \
    $$PWD/src/CreateNNWidget.h \
    $$PWD/src/ConstructorWidget.h \
    $$PWD/src/ParamsEditorWidget.h \
    $$PWD/src/GlobalWarningWidget.h \
    $$PWD/src/ConstructorWorkSpace.h \
    $$PWD/src/ConstructorWidgetHelper.h \
    $$PWD/src/Branch.h \
    $$PWD/src/ParamWidget.h \
    $$PWD/src/NNWidget.h \
    $$PWD/src/NNLayerWidget.h \
    $$PWD/src/NNAdapterWidget.h \
    $$PWD/src/EnumParams/ActivationFunc.h \
    $$PWD/src/EnumParams/Dropout.h \
    $$PWD/src/EnumParams/InitializerFunc.h \
    $$PWD/src/EnumParams/Normalization.h \
    $$PWD/src/EnumParams/Optimizer.h \
    $$PWD/src/EnumParams/Scheduler.h \
    $$PWD/src/EnumParams/PaddingMode.h \
    $$PWD/src/EnumParams/Recurrent.h \
    $$PWD/src/LayerParams/FabricLayer.h \
    $$PWD/src/LayerParams/FabricAdapter.h \
    $$PWD/src/LayerParams/FabricManager.h \
    $$PWD/src/LayerParams/IFabric.h \
    $$PWD/src/LayerParams/NNParam.h \
    $$PWD/src/LayerParams/NNLayerParams.h \
    $$PWD/src/LayerParams/Linear/LinearLayerParams.h \
    $$PWD/src/LayerParams/Linear/BilinearLayerParams.h \
    $$PWD/src/LayerParams/Convolution/Conv1dLayerParams.h \
    $$PWD/src/LayerParams/Convolution/Conv2dLayerParams.h \
    $$PWD/src/LayerParams/Convolution/Conv3dLayerParams.h


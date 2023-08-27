#pragma once

#include "ConstructorWidget.h"
#include "ParamsEditorWidget.h"

class MainWidget: public QWidget
{
    Q_OBJECT

public:
    MainWidget();

private:
    void initGUI();
    void createConnections();

    ConstructorWidget* m_pConstructorWidget;
    ParamsEditorWidget* m_pSettingsEdit;
};


#pragma once

#include "ConstructorWidget.h"
#include "SettingsEdit.h"

class MainWidget: public QWidget
{
    Q_OBJECT

public:
    MainWidget();

private:
    void initGUI();
    void createConnections();

    ConstructorWidget* m_pConstructorWidget;
    SettingsEdit* m_pSettingsEdit;
};


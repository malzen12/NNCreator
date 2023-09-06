#pragma once

#include "GlobalSettingsWidget.h"
#include "ConstructorWidget.h"
#include "ParamsEditorWidget.h"

class MainWidget: public QWidget
{
    Q_OBJECT

public:
    MainWidget();

private slots:
    void onMakeNetXml(const std::vector<NNParam>& vParams);
    void onMakeTrainXml(const std::vector<NNParam>& vParams);

private:
    void initGUI();
    void createConnections();

    GlobalSettingsWidget* m_pGlobalSettingsWidget;
    ConstructorWidget* m_pConstructorWidget;
    ParamsEditorWidget* m_pSettingsEdit;
};


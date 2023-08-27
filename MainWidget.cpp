#include "MainWidget.h"

#include <QHBoxLayout>

MainWidget::MainWidget()
    : m_pConstructorWidget{new ConstructorWidget},
      m_pSettingsEdit{new ParamsEditorWidget}
{
    initGUI();
    createConnections();
}


void MainWidget::initGUI()
{
    auto pLayout = new QHBoxLayout{this};

    pLayout->addWidget(m_pConstructorWidget, 80);
    pLayout->addWidget(m_pSettingsEdit, 20);

    resize(1000, 1000);
}

void MainWidget::createConnections()
{
    auto bRes = true;

    bRes &= static_cast<bool>(connect(m_pConstructorWidget, SIGNAL(paramsChanged(NNLayerParams)), m_pSettingsEdit, SLOT(onSetParams(NNLayerParams))));
    bRes &= static_cast<bool>(connect(m_pSettingsEdit, SIGNAL(paramsChanged(NNLayerParams)), m_pConstructorWidget, SLOT(onSetParams(NNLayerParams))));
    bRes &= static_cast<bool>(connect(m_pSettingsEdit, SIGNAL(deleteActive()), m_pConstructorWidget, SLOT(onDeleteActive())));

    assert(bRes);
}

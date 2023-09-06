#include "MainWidget.h"

#include <QHBoxLayout>

MainWidget::MainWidget()
    : m_pGlobalSettingsWidget{new GlobalSettingsWidget},
      m_pConstructorWidget{new ConstructorWidget},
      m_pSettingsEdit{new ParamsEditorWidget}
{
    initGUI();
    createConnections();
}

void MainWidget::onMakeNetXml(const std::vector<NNParam>& vParams)
{
    return ;
}

void MainWidget::onMakeTrainXml(const std::vector<NNParam>& vParams)
{
    return ;
}

void MainWidget::initGUI()
{
    auto pLayout = new QHBoxLayout{this};

    pLayout->addWidget(m_pGlobalSettingsWidget, 10);
    pLayout->addWidget(m_pConstructorWidget, 80);
    pLayout->addWidget(m_pSettingsEdit, 10);

    resize(1500, 1000);
}

void MainWidget::createConnections()
{
    auto bRes = true;

    bRes &= static_cast<bool>(connect(m_pConstructorWidget, SIGNAL(paramsChanged(NNLayerParams)), m_pSettingsEdit, SLOT(onSetParams(NNLayerParams))));

    bRes &= static_cast<bool>(connect(m_pSettingsEdit, SIGNAL(paramsChanged(NNLayerParams)), m_pConstructorWidget, SLOT(onSetParams(NNLayerParams))));
    bRes &= static_cast<bool>(connect(m_pSettingsEdit, SIGNAL(deleteActive()), m_pConstructorWidget, SLOT(onDeleteActive())));

    bRes &= static_cast<bool>(connect(m_pGlobalSettingsWidget, SIGNAL(makeNetXml(std::vector<NNParam>)), SLOT(onMakeNetXml(std::vector<NNParam>))));
    bRes &= static_cast<bool>(connect(m_pGlobalSettingsWidget, SIGNAL(makeTrainXml(std::vector<NNParam>)), SLOT(onMakeTrainXml(std::vector<NNParam>))));

    bRes &= static_cast<bool>(connect(m_pGlobalSettingsWidget, SIGNAL(inputSizeChanged(std::vector<std::size_t>)), m_pConstructorWidget, SLOT(onSetInputSize(std::vector<std::size_t>))));

    assert(bRes);
}

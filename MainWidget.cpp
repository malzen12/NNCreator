#include "MainWidget.h"

#include <QHBoxLayout>

MainWidget::MainWidget()
    : m_pConstructorWidget{new ConstructorWidget},
      m_pSettingsEdit{new SettingsEdit}
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

    assert(bRes);
}

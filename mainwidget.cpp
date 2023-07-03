#include "mainwidget.h"

#include <QMouseEvent>

MainWidget::MainWidget()
    : m_pMenu{new QMenu{this}}
{
    initGUI();
    createConnections();

    setMouseTracking(false);
}

void MainWidget::mousePressEvent(QMouseEvent* pEvent)
{
    if (pEvent->button() == Qt::RightButton)
    {
        auto Pos = mapToGlobal(pEvent->pos());

        m_pMenu->popup(Pos);
    }
    QWidget::mousePressEvent(pEvent);
}

void MainWidget::mouseMoveEvent(QMouseEvent* pEvent)
{
    for (auto pLayer : m_vLayers)
    {
        if (pLayer->isGrabbed())
            pLayer->move(pEvent->pos());
    }
}

void MainWidget::onAddLayer(const QPoint& crPoint)
{
    auto pLayer = new NNLayerWidget{m_vLayers.size()};

    pLayer->setParent(this);
    pLayer->move(crPoint);
    pLayer->show();
    this->stackUnder(pLayer);

    auto bRes = true;

    bRes = static_cast<bool>(connect(pLayer, SIGNAL(layerDeleted(int)), SLOT(onDeleteLayer(int))));

    assert(bRes);

    m_vLayers.push_back(pLayer);
}

void MainWidget::onProcActions(QAction* /*pAction*/)
{
    onAddLayer(mapFromGlobal(m_pMenu->pos()));
}

void MainWidget::onDeleteLayer(int iId)
{
    auto itLayer = std::find_if(m_vLayers.begin(), m_vLayers.end(), [iId](NNLayerWidget* pWdg)
    {
        return pWdg->getId() == iId;
    });

    if (m_vLayers.end() == itLayer)
        return ;

    m_vLayers.erase(itLayer);
}

void MainWidget::initGUI()
{
    m_pMenu->addAction(new QAction{"Add Layer"});
    setContextMenuPolicy(Qt::CustomContextMenu);
}

void MainWidget::createConnections()
{
    auto bRes = true;

    bRes = static_cast<bool>(connect(m_pMenu, SIGNAL(triggered(QAction*)), SLOT(onProcActions(QAction*))));

    assert(bRes);
}

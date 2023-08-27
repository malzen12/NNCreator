#include "ConstructorWidget.h"

#include <iostream>

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

NNLayerWidget* get_layer(std::size_t sId, std::vector<NNLayerWidget*> vLayers)
{
    auto itLayer = std::find_if(vLayers.begin(), vLayers.end(), [sId](NNLayerWidget* pWdg)
    {
        return pWdg->getId() == sId;
    });

    if (vLayers.end() == itLayer)
        throw std::runtime_error("invalid id passed");

    return *itLayer;
}

ConstructorWidget::ConstructorWidget()
    : m_pMenu{new QMenu{this}},
      m_sActive{0}
{
    initGUI();
    createConnections();

    setMouseTracking(false);
}

void ConstructorWidget::onSetSettings(std::size_t sId, const NNLayerSettings& crSettings)
{
    auto pLayerWidget = get_layer(sId, m_vLayers);

    pLayerWidget->setSettings(crSettings);
}

void ConstructorWidget::onDeleteLayer(NNLayerWidget* pDeletingLayer)
{
    for (auto pLayer : m_vLayers)
        pLayer->removeForward(pDeletingLayer);

    pDeletingLayer->deleteLayer();

    update();
}

void ConstructorWidget::onAddLayer(const QPoint& crPoint)
{
    auto pLayer = new NNLayerWidget{m_vLayers.size()};

    pLayer->setParent(this);
    pLayer->move(crPoint);
    pLayer->show();
    this->stackUnder(pLayer);

    auto bRes = true;

    bRes = static_cast<bool>(connect(pLayer, SIGNAL(becomeActive(std::size_t)), SLOT(onChangeActive(std::size_t))));

    assert(bRes);

    m_vLayers.push_back(pLayer);

    auto pActiveLayer = get_layer(m_sActive, m_vLayers);
    pActiveLayer->addForward(pLayer);

    onChangeActive(m_vLayers.back()->getId());

    update();
}

void ConstructorWidget::onProcActions(QAction* /*pAction*/)
{
    onAddLayer(mapFromGlobal(m_pMenu->pos()));
}

void ConstructorWidget::onChangeActive(std::size_t sId)
{
    if (m_sActive == sId)
        return ;

    makeActive(m_sActive, false);

    m_sActive = sId;

    makeActive(m_sActive, true);
}

void ConstructorWidget::initGUI()
{
    m_pMenu->addAction(new QAction{"Add Layer"});
    setContextMenuPolicy(Qt::CustomContextMenu);
}

void ConstructorWidget::createConnections()
{
    auto bRes = true;

    bRes = static_cast<bool>(connect(m_pMenu, SIGNAL(triggered(QAction*)), SLOT(onProcActions(QAction*))));

    assert(bRes);
}

void ConstructorWidget::makeActive(std::size_t sId, bool bActive)
{
    auto pLayerWidget = get_layer(sId, m_vLayers);

    pLayerWidget->makeActive(bActive);

    if (bActive)
        emit settingsChanged(pLayerWidget->getSettings());
}

void ConstructorWidget::mousePressEvent(QMouseEvent* pEvent)
{
    if (pEvent->button() == Qt::RightButton)
    {
        auto Pos = mapToGlobal(pEvent->pos());

        m_pMenu->popup(Pos);
    }
    QWidget::mousePressEvent(pEvent);
}

void ConstructorWidget::mouseMoveEvent(QMouseEvent* pEvent)
{
    for (auto pLayer : m_vLayers)
    {
        if (pLayer->isGrabbed())
            pLayer->move(pEvent->pos() - pLayer->getGrabbedPos());
    }
}

void ConstructorWidget::paintEvent(QPaintEvent* pEvent)
{
    QWidget::paintEvent(pEvent);

    QPainter Painter(this);

    Painter.setPen(QColor{0,0,0});

    for (auto pLayer : m_vLayers)
    {
        for(auto pForward : pLayer->getForward())
        {
            auto From = pLayer->pos() + pLayer->rect().center();
            auto To = pForward->pos() + pForward->rect().center();

            std::cout << From.x() << "+" << From.y() << " - " << To.x() << "+" << To.y() << std::endl;
            Painter.drawLine(From, To);
        }
    }
}

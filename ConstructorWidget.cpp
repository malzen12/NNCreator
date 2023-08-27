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

static std::size_t get_increment()
{
    static std::size_t sIncrement = 0;
    return ++sIncrement;
}

ConstructorWidget::ConstructorWidget()
    : m_pMenu{new QMenu{this}},
      m_sActive{0}
{
    initGUI();
    createConnections();

    setMouseTracking(false);
}

void ConstructorWidget::onSetParams(const NNLayerParams& crSettings)
{
    auto pLayerWidget = get_layer(m_sActive, m_vLayers);

    pLayerWidget->setSettings(crSettings);
}

void ConstructorWidget::onDeleteActive()
{
    if (m_vLayers.empty())
        return ;

    auto pDeletingLayer = get_layer(m_sActive, m_vLayers);

    for (auto pLayer : m_vLayers)
        pLayer->removeForward(pDeletingLayer);

    pDeletingLayer->deleteLayer();

    auto itLayer = std::find(m_vLayers.begin(), m_vLayers.end(), pDeletingLayer);

    if (m_vLayers.end() == itLayer)
        throw std::runtime_error("invalid id passed");

    m_vLayers.erase(itLayer);

    ///< @todo disconnect

    m_sActive = m_vLayers.front()->getId();

    update();
}

void ConstructorWidget::onAddLayer(const QPoint& crPoint, const NNLayerParams& crParams)
{
    auto pLayer = new NNLayerWidget{get_increment(), crParams};

    pLayer->setParent(this);
    pLayer->move(crPoint);
    pLayer->show();
    this->stackUnder(pLayer);

    auto bRes = true;

    bRes = static_cast<bool>(connect(pLayer, SIGNAL(becomeActive(std::size_t)), SLOT(onChangeActive(std::size_t))));
    bRes = static_cast<bool>(connect(pLayer, SIGNAL(makeForward(std::size_t)), SLOT(onMakeForward(std::size_t))));

    assert(bRes);

    if (!m_vLayers.empty())
    {
        auto pActiveLayer = get_layer(m_sActive, m_vLayers);
        pActiveLayer->addForward(pLayer);
    }

    m_vLayers.push_back(pLayer);

    onChangeActive(m_vLayers.back()->getId());

    update();
}

void ConstructorWidget::onProcActions(QAction* pAction)
{
    if ("Make linear" == pAction->text())
        onAddLayer(mapFromGlobal(m_pMenu->pos()), NNLayerParams::makeLinear());
    else if ("Make conv1d" == pAction->text())
        onAddLayer(mapFromGlobal(m_pMenu->pos()), NNLayerParams::makeConv1d());
    else if ("Make pool" == pAction->text())
        onAddLayer(mapFromGlobal(m_pMenu->pos()), NNLayerParams::makePool());
    else if ("Make embedding" == pAction->text())
        onAddLayer(mapFromGlobal(m_pMenu->pos()), NNLayerParams::makeEmbedding());
    else if ("Make reshape" == pAction->text())
        onAddLayer(mapFromGlobal(m_pMenu->pos()), NNLayerParams::makeReshape());
    else if ("Make normalization" == pAction->text())
        onAddLayer(mapFromGlobal(m_pMenu->pos()), NNLayerParams::makeNormalization());
    else if ("Make activation" == pAction->text())
        onAddLayer(mapFromGlobal(m_pMenu->pos()), NNLayerParams::makeActivation());
    else
        throw std::runtime_error("unknown action passed");
}

void ConstructorWidget::onChangeActive(std::size_t sId)
{
    if (m_sActive == sId)
        return ;

    if (m_vLayers.size() > 1)
        makeActive(m_sActive, false);

    m_sActive = sId;

    makeActive(m_sActive, true);
}

void ConstructorWidget::onMakeForward(std::size_t sId)
{
    auto pActive = get_layer(m_sActive, m_vLayers);
    auto pForward = get_layer(sId, m_vLayers);

    if (!pActive || !pForward)
        return;

    pActive->addForward(pForward);
    update();
}

void ConstructorWidget::initGUI()
{
    m_pMenu->addAction(new QAction{"Make linear"});
    m_pMenu->addAction(new QAction{"Make conv1d"});
    m_pMenu->addAction(new QAction{"Make pool"});
    m_pMenu->addAction(new QAction{"Make embedding"});
    m_pMenu->addAction(new QAction{"Make reshape"});
    m_pMenu->addAction(new QAction{"Make normalization"});
    m_pMenu->addAction(new QAction{"Make activation"});

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
        emit paramsChanged(pLayerWidget->getSettings());
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
    update();
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
            auto From = pLayer->pos() + QPoint{pLayer->rect().center().x(), pLayer->rect().bottom()};
            auto To = pForward->pos() + QPoint{pForward->rect().center().x(), 0};

            Painter.drawLine(From, To);
            Painter.drawEllipse(To, 5, 5);
        }
    }
}

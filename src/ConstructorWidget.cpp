#include "ConstructorWidget.h"

#include <iostream>
#include <fstream>
#include <queue>
#include <set>

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

static std::size_t get_increment()
{
    static std::size_t sIncrement = 0;
    return ++sIncrement;
}

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

void print_params(const std::string& strParam, const std::string& strTag, std::ofstream& rXml)
{
    rXml << "<" << strTag << ">" << strParam << "</" << strTag << ">" << std::endl;
}

void print_params(std::size_t sParam, const std::string& strTag, std::ofstream& rXml)
{
    rXml << "<" << strTag << ">" << sParam << "</" << strTag << ">" << std::endl;
}

void print_params(const NNParam& crParam, std::ofstream& rXml)
{
    rXml << "<" << crParam.getName() << ">";

    if (crParam.getType() == QVariant::Type::String)
        rXml << crParam.getValue().toString().toStdString();
    else if (crParam.getType() == QVariant::Type::UInt)
        rXml << crParam.getValue().toUInt();
    else if (crParam.getType() == QVariant::Type::List)
    {
        auto lVal = crParam.getValue().toList();
        for (const auto& crVal : lVal)
            rXml << "<v>" << crVal.toUInt() << "</v>";

    }
    else
        rXml << crParam.getValue().toString().toStdString();

    rXml << "</" << crParam.getName() << ">" << std::endl;
}

void print_params(const std::shared_ptr<NNLayerParams>& spParams, std::ofstream& rXml)
{
    print_params(spParams->getName(), "type", rXml);

    for (const auto& crParam : spParams->getParams())
        print_params(crParam, rXml);
}

void print_params(const std::vector<std::size_t>& vParams, const std::string& strTag, std::ofstream& rXml)
{
    rXml << "<" << strTag << ">" << std::endl;

    for (const auto& crParam : vParams)
        print_params(crParam, "v", rXml);

    rXml << "</" << strTag << ">" << std::endl;
}

ConstructorWidget::ConstructorWidget()
    : m_pMenu{new QMenu{this}},
      m_sActive{0}
{
    initGUI();
    createConnections();

    setMouseTracking(false);
}

void ConstructorWidget::onSetParams(const std::shared_ptr<NNLayerParams>& spParams)
{
    auto pLayerWidget = get_layer(m_sActive, m_vLayers);

    pLayerWidget->setParams(spParams);

    checkSizes();
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

void ConstructorWidget::onSetInputSize(const std::vector<std::size_t>& vInputSize)
{
    m_vInputSize = vInputSize;
}

void ConstructorWidget::onMakeXml()
{
    auto pStart = findStart();

    std::ofstream Xml;
    Xml.open(m_strPath);

    ///< @todo replace cout to fout
    Xml << "<?xml version=\"1.0\"?>" << std::endl;
    Xml << "<data>" << std::endl;
    print_params(m_vInputSize, "data_size", Xml);

    Xml << "<layers>" << std::endl;
    bfs(pStart, [&Xml](NNLayerWidget* pCurrent, NNLayerWidget* /*pNext*/)
    {
        Xml << "<l>" << std::endl;
        print_params(pCurrent->getId(), "id", Xml);
        print_params(pCurrent->getParams(), Xml);

        Xml << "<forwards>" << std::endl;
        for (auto pLayer : pCurrent->getForward())
            print_params(pLayer->getId(), "v", Xml);
        Xml << "</forwards>" << std::endl;

        Xml << "</l>" << std::endl;
    }, {});
    Xml << "</layers>" << std::endl;
    Xml << "</data>" << std::endl;

    Xml.close();
}

void ConstructorWidget::onSetOutputPath(const QString& qstrPath)
{
    m_strPath = qstrPath.toStdString();
}

void ConstructorWidget::onAddLayer(const QPoint& crPoint, const std::shared_ptr<NNLayerParams>& spParams)
{
    auto pLayer = new NNLayerWidget{get_increment(), spParams};

    pLayer->setParent(this);
    pLayer->move(crPoint);
    pLayer->show();
    this->stackUnder(pLayer);

    auto bRes = true;

    bRes &= static_cast<bool>(connect(pLayer, SIGNAL(becomeActive(std::size_t)), SLOT(onChangeActive(std::size_t))));
    bRes &= static_cast<bool>(connect(pLayer, SIGNAL(makeForward(std::size_t)), SLOT(onMakeForward(std::size_t))));

    assert(bRes);

    if (!m_vLayers.empty())
    {
        auto pActiveLayer = get_layer(m_sActive, m_vLayers);
        pActiveLayer->addForward(pLayer);
    }

    m_vLayers.push_back(pLayer);

    onChangeActive(m_vLayers.back()->getId());

    checkSizes();

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
    else if ("Make concatinate")
        onAddLayer(mapFromGlobal(m_pMenu->pos()), NNLayerParams::makeConcatinate());
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
    m_pMenu->addAction(new QAction{"Make concatinate"});

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
        emit paramsChanged(pLayerWidget->getParams());
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

void ConstructorWidget::checkSizes()
{
    auto pStart = findStart();

    for (auto pLayer : m_vLayers)
        pLayer->resetInputSize();

    pStart->addInputSize(m_vInputSize);

    bfs(pStart, {}, [](NNLayerWidget* pCurrent, NNLayerWidget* pNext)
    {
        if (pNext)
            pNext->addInputSize(pCurrent->calcOutputSize());
    });
}

NNLayerWidget* ConstructorWidget::findStart()
{
    for (auto pChecking : m_vLayers)
    {
        auto bStart = std::all_of(m_vLayers.begin(), m_vLayers.end(), [pChecking](NNLayerWidget* pLayer)
        {
            const auto& vForwards = pLayer->getForward();
            auto it = std::find(vForwards.begin(), vForwards.end(), pChecking);
            return it == vForwards.end();
        });
        if (bStart)
            return pChecking;
    }
    throw std::runtime_error("circular dependensis");
}

void ConstructorWidget::bfs(NNLayerWidget* pStart, bfs_proc fCurrentProc, bfs_proc fForwardsProc)
{
    std::set<NNLayerWidget*> VisitedSet;

    std::queue<NNLayerWidget*> qToVisit;
    qToVisit.push(pStart);

    VisitedSet.insert(pStart);

    while (!qToVisit.empty())
    {
        auto pCurrent = qToVisit.front();
        qToVisit.pop();

        if (fCurrentProc)
            fCurrentProc(pCurrent, nullptr);

        for (auto pNext : pCurrent->getForward())
        {
            if (VisitedSet.count(pNext))
                continue;

            VisitedSet.insert(pNext);

            if (fForwardsProc)
                fForwardsProc(pCurrent, pNext);

            qToVisit.push(pNext);
        }
    }
}

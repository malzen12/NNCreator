#include "ConstructorWidget.h"

#include <sstream>
#include <queue>
#include <set>
#include <unordered_set>

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

void print_params(const std::string& strParam, const std::string& strTag, std::stringstream& rStream)
{
    rStream << "<" << strTag << ">" << strParam << "</" << strTag << ">" << std::endl;
}

void print_params(std::size_t sParam, const std::string& strTag, std::stringstream& rStream)
{
    rStream << "<" << strTag << ">" << sParam << "</" << strTag << ">" << std::endl;
}

void print_params(const std::vector<std::size_t>& vParams, const std::string& strTag, std::stringstream& rStream)
{
    rStream << "<" << strTag << ">" << std::endl;

    for (const auto& crParam : vParams)
        print_params(crParam, "v", rStream);

    rStream << "</" << strTag << ">" << std::endl;
}

ConstructorWidget::ConstructorWidget()
    : m_pMenu{new QMenu{this}},
      m_sActive{0}
{
    initGUI();
    createConnections();

    setMouseTracking(false);
}

std::string ConstructorWidget::makeXmlString() const
{
    auto pStart = findStart();

    std::stringstream Stream;
    print_params(m_vInputSize, "data_size", Stream);

    Stream << "<layers>" << '\n';
    bfs(pStart, [&Stream](NNLayerWidget* pCurrent, NNLayerWidget* /*pNext*/)
    {
        Stream << "<l>" << std::endl;
        print_params(pCurrent->getId(), "id", Stream);

        Stream << pCurrent->getParams()->makeXmlString();

        Stream << "<forwards>" << '\n';
        for (auto pLayer : pCurrent->getForward())
            print_params(pLayer->getId(), "v", Stream);
        Stream << "</forwards>" << '\n';

        Stream << "</l>" << '\n';
    }, {});
    Stream << "</layers>" << std::endl;

    return Stream.str();
}

std::string ConstructorWidget::makePyString() const
{
    auto pStart = findStart();

    std::stringstream Stream;
    bool isEnum;

    Stream << R"(class Net(nn.Module):)" << '\n';
    int tab = 0;
    ++tab;
    Stream << std::string(tab, '\t')
           << R"(def __init__(self):)" << '\n';
    ++tab;
    Stream << std::string(tab, '\t')
           << R"(super().__init__())" << '\n';


    //  Stream <<
    //R"(class Net(nn.Module):
    //    def __init__(self):
    //        super().__init__()
    //)";
    //  int tab = 2;




    auto createName = [&Stream](const NNLayerWidget* pParam){
        Stream << "self." << pParam->getParams()->getName()
               << "_" << std::to_string(pParam->getId());
    };
    auto createNameEnum = [&Stream](const NNLayerWidget* pParam){
        Stream << "self." << pParam->getParams()->getParams().front().getValue().toString().toStdString()
               << "_" << std::to_string(pParam->getId());
    };
    auto createNameEnumRecurrent = [&Stream](const NNLayerWidget* pParam){
        Stream << "self." << pParam->getParams()->getParams().front().getValue().toString().toStdString()
               << "_" << std::to_string(pParam->getId());
    };
    auto createNameMyRecurrent = [&Stream](const NNLayerWidget* pParam){
        Stream << "self." << "Linear"
               << "_" << std::to_string(pParam->getId()) << "h";
    };

    for(const auto& pCurrent: m_vLayers){
        isEnum = pCurrent->getParams()->getParams().front().isEnum();
        if(!isEnum){
            if(pCurrent->getParams()->getName() == "MyRecurrent"){
                Stream << std::string(tab, '\t');
                createNameMyRecurrent(pCurrent);
            }else{
                Stream << std::string(tab, '\t');
                createName(pCurrent);
            }
        }
        else{
            auto enamName = pCurrent->getParams()->getParams().front().getName();
            if(enamName == "Recurrent"){
                Stream << std::string(tab, '\t');
                createNameEnumRecurrent(pCurrent);
            }else if(enamName == "Normalization"){
                continue;
            }
        }




        if(!isEnum){
            if(pCurrent->getParams()->getName() == "MyRecurrent"){
                Stream  << " = nn."
                        << "Linear"
                        << "(";
                auto it = pCurrent->getParams()->getParams().cbegin();
                Stream << it->makePyString();
                ++it;
                for(auto ite = pCurrent->getParams()->getParams().cend(); std::next(it) != ite; ++it){
                    Stream <<", " << it->makePyString();
                }
                Stream << ")\n";
            }else{
                Stream  << " = nn."
                        << pCurrent->getParams()->getName()
                        << "(";
                auto it = pCurrent->getParams()->getParams().cbegin();
                Stream << it->makePyString();
                ++it;
                for(auto ite = pCurrent->getParams()->getParams().cend(); std::next(it) != ite; ++it){
                    Stream <<", " << it->makePyString();
                }
                Stream << ")\n";
            }



        }
        else{
            Stream  << " = nn."
                    << pCurrent->getParams()->getParams().front().getValue().toString().toStdString()
                    << "(";
            auto it = pCurrent->getParams()->getParams().cbegin();
            ++it;
            Stream << it->makePyString() << ", ";
            ++it;
            Stream << it->makePyString() << ", ";
            ++it;
            Stream << it->makePyString() << ", ";
            ++it;
            if(pCurrent->getParams()->getParams().front().getValue() == "RNN")
                Stream << R"('tanh')" << ", ";
            Stream << it->makePyString() << ", ";
            ++it;
            Stream << R"(False, 0.0)" << ", ";
            Stream << it->makePyString();
            Stream << ")\n";

        }

    }

    Stream << '\n';
    --tab;
    Stream << std::string(tab, '\t')
           << R"(def forward(self, x):)" << '\n';
    ++tab;
    auto createNameForward = [tab, &Stream, createName, createNameEnumRecurrent, createNameEnum, createNameMyRecurrent](const NNLayerWidget* pParam){
        bool isEnum = pParam->getParams()->getParams().front().isEnum();
        if(!isEnum){
            if(pParam->getParams()->getName() == "MyRecurrent"){
                int n = std::next(pParam->getParams()->getParams().cbegin())->getValue().toInt();
                for(int i = 0; i < n; ++i ){
                    Stream << std::string(tab, '\t')
                           << "x = ";
                    createNameMyRecurrent(pParam);
                    Stream << "(x)" << '\n';
                }
            }else{
                Stream << std::string(tab, '\t')
                       << "x = ";
                createName(pParam);
                Stream << "(x)" << '\n';
            }

        }else{
            auto enamName = pParam->getParams()->getParams().cbegin()->getName();
            if(enamName == "Recurrent"){
                Stream << std::string(tab, '\t')
                       << "x, y = ";
                createNameEnumRecurrent(pParam);
                Stream << "(x)" << '\n';
            }else if(enamName == "Normalization"){
                Stream << std::string(tab, '\t');
                createNameEnum(pParam);
                Stream  << " = nn."
                        << pParam->getParams()->getParams().front().getValue().toString().toStdString()
                        << R"((x.size()))" << '\n';
                Stream << std::string(tab, '\t')
                       << "x = ";
                createNameEnum(pParam);
                Stream << "(x)" << '\n';
            }
        }
    };

    createNameForward(pStart);

    bfs(pStart, [tab, &Stream, createNameForward](NNLayerWidget* pCurrent, NNLayerWidget* /*pNext*/)
    {
        for (auto pLayer : pCurrent->getForward()){
            createNameForward(pLayer);
        }
    }, {});

    Stream << std::string(tab, '\t')
           << R"(return x)" << '\n';

    return Stream.str();
}

bool ConstructorWidget::isEmpty () const noexcept
{
    return m_vLayers.empty();
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
    else if ("Make conv2d" == pAction->text())
        onAddLayer(mapFromGlobal(m_pMenu->pos()), NNLayerParams::makeConv2d());
    else if ("Make pool" == pAction->text())
        onAddLayer(mapFromGlobal(m_pMenu->pos()), NNLayerParams::makePool());
    else if ("Make pool2d" == pAction->text())
        onAddLayer(mapFromGlobal(m_pMenu->pos()), NNLayerParams::makePool2d());
    else if ("Make embedding" == pAction->text())
        onAddLayer(mapFromGlobal(m_pMenu->pos()), NNLayerParams::makeEmbedding());
    else if ("Make reshape" == pAction->text())
        onAddLayer(mapFromGlobal(m_pMenu->pos()), NNLayerParams::makeReshape());
    else if ("Make normalization" == pAction->text())
        onAddLayer(mapFromGlobal(m_pMenu->pos()), NNLayerParams::makeNormalization());
    else if ("Make activation" == pAction->text())
        onAddLayer(mapFromGlobal(m_pMenu->pos()), NNLayerParams::makeActivation());
    else if ("Make concatinate" == pAction->text())
        onAddLayer(mapFromGlobal(m_pMenu->pos()), NNLayerParams::makeConcatinate());
    else if ("Make dropout" == pAction->text())
        onAddLayer(mapFromGlobal(m_pMenu->pos()), NNLayerParams::makeDropout());
    else if ("Make flatten" == pAction->text())
        onAddLayer(mapFromGlobal(m_pMenu->pos()), NNLayerParams::makeFlatten());
    else if ("Make recurrent" == pAction->text())
        onAddLayer(mapFromGlobal(m_pMenu->pos()), NNLayerParams::makeRecurrent());
    else if ("Make MyRecurrent" == pAction->text())
        onAddLayer(mapFromGlobal(m_pMenu->pos()), NNLayerParams::makeMyRecurrent());
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
    m_pMenu->addAction(new QAction{"Make conv2d"});
    m_pMenu->addAction(new QAction{"Make pool"});
    m_pMenu->addAction(new QAction{"Make pool2d"});
    m_pMenu->addAction(new QAction{"Make embedding"});
    m_pMenu->addAction(new QAction{"Make reshape"});
    m_pMenu->addAction(new QAction{"Make normalization"});
    m_pMenu->addAction(new QAction{"Make activation"});
    m_pMenu->addAction(new QAction{"Make concatinate"});
    m_pMenu->addAction(new QAction{"Make dropout"});
    m_pMenu->addAction(new QAction{"Make flatten"});
    m_pMenu->addAction(new QAction{"Make recurrent"});
    m_pMenu->addAction(new QAction{"Make MyRecurrent"});

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

    bfs(pStart, {}, [](NNLayerWidget* pCurrent, NNLayerWidget* pNext){
        if (pNext)
            pNext->addInputSize(pCurrent->calcOutputSize());
    });
}

NNLayerWidget* ConstructorWidget::findStart() const
{
#if 0 // start solution without cheking how many roots    always O(N^2) speed O(1) memory
    for (const auto pChecking : m_vLayers){
        bool bStart = std::all_of(m_vLayers.begin(), m_vLayers.end(), [pChecking](NNLayerWidget* pLayer){
            const auto& vForwards = pLayer->getForward();
            auto it = std::find(vForwards.begin(), vForwards.end(), pChecking);
            return it == vForwards.end();
        });
        if (bStart)
            return pChecking;
    }
    throw std::runtime_error("circular dependensis");
#elif 1 // start solution with cheking how many roots    always O(N^2) speed O(1) memory
    if(m_vLayers.size() == 1){
        return m_vLayers.front();
    }
    int count = 0;
    auto pResult = *(m_vLayers.begin());
    for (const auto pChecking : m_vLayers){
        bool bStart = std::all_of(m_vLayers.begin(), m_vLayers.end(), [pChecking](NNLayerWidget* pLayer){
            const auto& vForwards = pLayer->getForward();
            auto it = std::find(vForwards.begin(), vForwards.end(), pChecking);
            return it == vForwards.end();
        });
        if (bStart){
            ++count;
            pResult = pChecking;
        }

    }
    switch(count){
    case 1:
        return pResult;
    case 0:
        throw std::runtime_error("circular dependensis");
        return{};
    default:
        throw std::runtime_error("Too many roots");
        return{};
    }
#elif 1 // solution with cheking how many roots. Roots stored in container(list or vector)    in worst O(N^2) speed O(N) memory   avarage O(N^2) speed O(N) memory
    if(m_vLayers.size() == 1){
        return m_vLayers.front();
    }
    std::list<NNLayerWidget*> ans;
    for (const auto pChecking : m_vLayers){
        bool bStart = std::all_of(m_vLayers.begin(), m_vLayers.end(), [pChecking](NNLayerWidget* pLayer){
                const auto& vForwards = pLayer->getForward();
                auto it = std::find(vForwards.begin(), vForwards.end(), pChecking);
                return it == vForwards.end();
    });
        if (bStart)
            ans.push_back(pChecking);
    }
    switch(ans.size()){
    case 1:
        return ans.front();
    case 0:
        throw std::runtime_error("circular dependensis");
        return{};
    default:
        throw std::runtime_error("Too many roots");
        return{};
    }
#elif 1 // solution with cheking how many roots. Not roots stored in unordered_set    in worst O(N^2) speed O(N) memory   avarage O(N^1,5) speed O(N) memory
    if(m_vLayers.size() == 1){
        return m_vLayers.front();
    }
    std::unordered_set<NNLayerWidget*> hasParent;
    hasParent.reserve(m_vLayers.size());
    std::for_each(m_vLayers.cbegin(), m_vLayers.cend(), [&hasParent](const auto pLayer){
        const auto& vForwards = pLayer->getForward();
        std::for_each(vForwards.cbegin(), vForwards.cend(), [&hasParent](const auto i){
            hasParent.insert(i);
        });
    });
    switch(m_vLayers.size() - hasParent.size()){
    case 1:
        return *std::find_if_not(m_vLayers.cbegin(), m_vLayers.cend(),
                                 [&hasParent](const auto pLayer){return hasParent.find(pLayer) != hasParent.cend();});
    case 0:
        throw std::runtime_error("circular dependensis");
        return{};
    default:
        throw std::runtime_error("Too many roots");
        return{};
    }
#elif 1 // solution with cheking how many roots. Not roots stored in unordered_map    O(N^2) speed O(N) memory
    if(m_vLayers.size() == 1){
        return m_vLayers.front();
    }
    std::unordered_map<NNLayerWidget*, int> hasParent;
    hasParent.reserve(m_vLayers.size());
    std::for_each(m_vLayers.cbegin(), m_vLayers.cend(), [&hasParent](const auto pLayer){
        const auto& vForwards = pLayer->getForward();
        std::for_each(vForwards.cbegin(), vForwards.cend(), [&hasParent](const auto i){
            ++hasParent[i];
        });
    });
    switch(m_vLayers.size() - hasParent.size()){
    case 1:
        return *std::find_if_not(m_vLayers.cbegin(), m_vLayers.cend(),
                                 [&hasParent](const auto pLayer){return hasParent.find(pLayer) != hasParent.cend();});
    case 0:
        throw std::runtime_error("circular dependensis");
        return{};
    default:
        throw std::runtime_error("Too many roots");
        return{};
    }
#else // solution with cheking how many roots. Not roots stored in unordered_map    O(N^2) speed O(N) memory
    if(m_vLayers.size() == 1){
        return m_vLayers.front();
    }
    std::unordered_map<NNLayerWidget*, int> hasParent;
    hasParent.reserve(m_vLayers.size());
    std::for_each(m_vLayers.cbegin(), m_vLayers.cend(), [&hasParent](const auto pLayer){
        hasParent[pLayer] = 0;
    });

    std::for_each(m_vLayers.cbegin(), m_vLayers.cend(), [&hasParent](const auto pLayer){
        const auto& vForwards = pLayer->getForward();
        std::for_each(vForwards.cbegin(), vForwards.cend(), [&hasParent](const auto i){
            ++hasParent[i];
        });
    });
    auto sizeParent = std::count_if(hasParent.begin(), hasParent.end(), [&hasParent](const auto& it){return it.second == 0;});
    switch(sizeParent){
    case 1:
        return *std::find_if_not(m_vLayers.cbegin(), m_vLayers.cend(),
                                 [&hasParent](const auto pLayer){return hasParent[pLayer] != 0;});
    case 0:
        throw std::runtime_error("circular dependensis");
        return{};
    default:
        throw std::runtime_error("Too many roots");
        //std::vector<> ans;
        //for_each(hasParent.begin(), hasParent.end(), [&hasParent, &ans](const auto& it){if(it.second == 0) ans.push_back(it.first);});
        return{};
    }
#endif

}

void ConstructorWidget::bfs(NNLayerWidget* pStart, bfs_proc fCurrentProc, bfs_proc fForwardsProc) const
{
    std::set<NNLayerWidget*> VisitedSet;

    std::queue<NNLayerWidget*> qToVisit;
    qToVisit.push(pStart);

    VisitedSet.insert(pStart);

    while(!qToVisit.empty())
    {
        auto pCurrent = qToVisit.front();
        qToVisit.pop();

        if (fCurrentProc)
            fCurrentProc(pCurrent, nullptr);

        for (auto pNext : pCurrent->getForward())
        {
            if (VisitedSet.find(pNext) != VisitedSet.cend())
                continue;

            VisitedSet.insert(pNext);

            if (fForwardsProc)
                fForwardsProc(pCurrent, pNext);

            qToVisit.push(pNext);
        }
    }
}

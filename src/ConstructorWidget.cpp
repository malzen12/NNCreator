#include "ConstructorWidget.h"

#include <sstream>
#include <queue>
#include <set>
#include <unordered_set>
#include <unordered_map>

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>


#include <iostream>
#include <QShortcut>

#include "Branch.h"

static std::size_t get_increment()
{
  static std::size_t sIncrement = 0;
  return sIncrement++;
}

NNLayerWidget* get_layer(std::size_t sId, std::unordered_map<std::size_t, NNLayerWidget*> vLayers)
{
  auto itLayer = std::find_if(vLayers.begin(), vLayers.end(),
                              [sId](const auto pWdg){
                                return pWdg.second->getId() == sId;
                              });

  if (vLayers.end() == itLayer)
    throw std::runtime_error("invalid id passed _get_layer_");

  return itLayer->second;
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
    m_pFabricLayer{new FabricLayer}
{
  initGUI();
  createConnections();

  setMouseTracking(false);
}

std::string ConstructorWidget::makeXmlString() const
{
  std::stringstream Stream;
  print_params(m_vInputSize, "data_size", Stream);

  Stream << "<layers>" << '\n';
  for(const auto [id, pCurrent]: m_vLayers){
    Stream << "<l>" << std::endl;
    print_params(id, "id", Stream);

    Stream << pCurrent->getParams()->makeXmlString();

    Stream << "<forwards>" << '\n';
    for (auto pLayer : pCurrent->getForward())
      print_params(pLayer->getId(), "v", Stream);
    Stream << "</forwards>" << '\n';

    Stream << "</l>" << '\n';
  }
  Stream << "</layers>" << std::endl;

  return Stream.str();
}

std::string ConstructorWidget::makePyString()
{
  emit startCalculation();
  auto result{graph.print()};
  emit compliteCalculation();
  return result;
}


bool ConstructorWidget::isEmpty () const noexcept
{
  return m_vLayers.empty();
}

void ConstructorWidget::onSetParams(const std::shared_ptr<NNLayerParams>& spParams)
{
  emit startCalculation();
  auto active = *m_ActiveSet.cbegin();
  auto pLayerWidget = m_vLayers[active];

  pLayerWidget->setParams(spParams);

  checkSizes();
  m_pWorkSpace->update();
  emit compliteCalculation();
}

void ConstructorWidget::onDeleteActive()
{
  emit startCalculation();
  if (m_vLayers.empty())
    return ;

  while(!m_ActiveSet.empty()){
    const auto active = *m_ActiveSet.cbegin();
    auto pDeletingLayer = m_vLayers[active];

    for (auto [_, pLayer] : m_vLayers)
      pLayer->removeForward(pDeletingLayer);

    pDeletingLayer->deleteLayer();
    graph.deleteVertex(active);
    m_vLayers.erase(active);
    onDelFromActive(active);
  }
  if(!m_vLayers.empty()){
    m_ActiveSet.insert(m_vLayers.cbegin()->first);
    makeActive(m_vLayers.cbegin()->first, true);
  }

  checkSizes();
  m_pWorkSpace->update();
  emit compliteCalculation();
}

void ConstructorWidget::onDeleteEdge(std::size_t sId)
{
  emit startCalculation();
  if (m_vLayers.empty())
    return ;
  auto pDeletingLayer = m_vLayers[sId];


  for(const auto active : m_ActiveSet){
    m_vLayers[active]->removeForward(pDeletingLayer);
    graph.deleteEdge(active, sId);
  }

  checkSizes();
  m_pWorkSpace->update();
  emit compliteCalculation();
}


void ConstructorWidget::onSetInputSize(const std::vector<std::size_t>& vInputSize)
{
  m_vInputSize = vInputSize;
}

void ConstructorWidget::onAddLayer(const QPoint& crPoint, const std::shared_ptr<NNLayerParams>& spParams)
{
  emit startCalculation();
  auto id = NNLayerWidget::createId();
  auto pLayer = new NNLayerWidget{id, spParams};
  m_pWorkSpace->add(crPoint, pLayer);

  auto bRes = true;

  bRes &= static_cast<bool>(connect(pLayer, SIGNAL(becomeActive(std::size_t)), SLOT(onChangeActive(std::size_t))));
  bRes &= static_cast<bool>(connect(pLayer, SIGNAL(makeForward(std::size_t)), SLOT(onMakeForward(std::size_t))));
  bRes &= static_cast<bool>(connect(pLayer, SIGNAL(addToActive(std::size_t)), SLOT(onAddToActive(std::size_t))));
  bRes &= static_cast<bool>(connect(pLayer, SIGNAL(delFromActive(std::size_t)), SLOT(onDelFromActive(std::size_t))));

  assert(bRes);

  graph.addVertex(id);
  m_vLayers[id] = pLayer;
  onChangeActive(id);

  checkSizes();

  m_pWorkSpace->update();
  emit compliteCalculation();
}

void ConstructorWidget::onAddLayer(const QPoint& crPoint, NNLayerWidget* pLayer)
{
  emit startCalculation();
  auto id = pLayer->getId();
  m_pWorkSpace->add(crPoint, pLayer);

  auto bRes = true;

  bRes &= static_cast<bool>(connect(pLayer, SIGNAL(becomeActive(std::size_t)), SLOT(onChangeActive(std::size_t))));
  bRes &= static_cast<bool>(connect(pLayer, SIGNAL(makeForward(std::size_t)), SLOT(onMakeForward(std::size_t))));
  bRes &= static_cast<bool>(connect(pLayer, SIGNAL(addToActive(std::size_t)), SLOT(onAddToActive(std::size_t))));
  bRes &= static_cast<bool>(connect(pLayer, SIGNAL(delFromActive(std::size_t)), SLOT(onDelFromActive(std::size_t))));

  assert(bRes);

  graph.addVertex(id);
  m_vLayers[id] = pLayer;
  onChangeActive(id);

  checkSizes();

  m_pWorkSpace->update();
  emit compliteCalculation();
}
void ConstructorWidget::onProcActions(QAction* pAction)
{
  if ("Make linear" == pAction->text())
    onAddLayer(mapFromGlobal(m_pMenu->pos()), NNLayerParams::makeLinear());
  else if ("Make bilinear" == pAction->text())
    onAddLayer(mapFromGlobal(m_pMenu->pos()), NNLayerParams::makeBilinear());
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
  if(m_ActiveSet.find(sId) != m_ActiveSet.cend())
    return;
  while(!m_ActiveSet.empty()){
    auto id = *m_ActiveSet.cbegin();
    m_ActiveSet.erase(id);
//    onDelFromActive(id);
    if(sId != id)
      makeActive(id, false);
  }
//  emit changeActiveLayer(nullptr);
//  emit paramsChanged(nullptr);

  if(sId == SIZE_MAX)
    return;

//  onAddToActive(sId);
  m_ActiveSet.insert(sId);
  makeActive(sId, true);
}

void ConstructorWidget::onMakeForward(std::size_t sId)
{
  emit startCalculation();

  for(const auto active : m_ActiveSet){
    auto pActive = m_vLayers[active];
    auto pForward = m_vLayers[sId];

    if (!pActive || !pForward)
      return;
    graph.addEdge(active, sId);
    pActive->addForward(pForward);
  }
  if(m_ActiveSet.size() == 1){
    auto pActive = m_vLayers[*m_ActiveSet.cbegin()];
    emit changeActiveLayer(pActive);
  }

  checkSizes();
  m_pWorkSpace->update();
  emit compliteCalculation();
}

void ConstructorWidget::onAddToActive(std::size_t sId)
{
  m_ActiveSet.insert(sId);
  if(m_ActiveSet.size() == 1){
    auto pLayerWidget = m_vLayers[sId];
    emit changeActiveLayer(pLayerWidget);
    emit paramsChanged(pLayerWidget->getParams());
  }else{
    emit changeActiveLayer(nullptr);
    emit paramsChanged(nullptr);
  }
}

void ConstructorWidget::onDelFromActive(std::size_t sId)
{
  m_ActiveSet.erase(sId);
  if(m_ActiveSet.size() != 1){
    emit changeActiveLayer(nullptr);
    emit paramsChanged(nullptr);
  }else{
    auto pLayerWidget = m_vLayers[*m_ActiveSet.cbegin()];
    emit changeActiveLayer(pLayerWidget);
    emit paramsChanged(pLayerWidget->getParams());
  }
}

void ConstructorWidget::onActiveAll()
{
  for(const auto [id, pLayer]: m_vLayers){
    pLayer->toActive();
  }
}

void ConstructorWidget::onCreateWidget(NNLayerWidget* pLayer)
{
  QPoint point{this->rect().center()};
  std::cout << point.x() << "\t" << point.y() << std::endl;

  onAddLayer(point, pLayer);
}

void ConstructorWidget::initGUI()
{
  auto pLayout = new QGridLayout{this};
  m_pWorkSpace = new ConstructorWorkSpace{m_vLayers, m_ActiveSet, m_pMenu};
  m_pWorkSpace->setAutoFillBackground(false);
  auto temp = new QScrollArea;
  temp->setWidget(m_pWorkSpace);
  m_pWorkSpace->show();
  pLayout->addWidget(temp);
  pLayout->setMargin(0);

  m_pMenu->addAction(new QAction{"Make linear"});
  m_pMenu->addAction(new QAction{"Make bilinear"});
  m_pMenu->addAction(new QAction{"Make conv1d"});
  m_pMenu->addAction(new QAction{"Make conv2d"});
  m_pMenu->addAction(new QAction{"Make pool"});
  m_pMenu->addAction(new QAction{"Make pool2d"});
  m_pMenu->addAction(new QAction{"Make embedding"});
  m_pMenu->addAction(new QAction{"Make reshape"});
  m_pMenu->addAction(new QAction{"Make normalization"});
  m_pMenu->addAction(new QAction{"Make activation"});
  m_pMenu->addAction(new QAction{"Make dropout"});
  m_pMenu->addAction(new QAction{"Make flatten"});
  m_pMenu->addAction(new QAction{"Make recurrent"});
  m_pMenu->addAction(new QAction{"Make MyRecurrent"});

  setContextMenuPolicy(Qt::CustomContextMenu);
}

void ConstructorWidget::createConnections()
{
  auto bRes = true;

  bRes &= static_cast<bool>(connect(m_pMenu, SIGNAL(triggered(QAction*)), SLOT(onProcActions(QAction*))));
  bRes &= static_cast<bool>(connect(&graph, SIGNAL(notValid(std::string)), SIGNAL(notValid(std::string))));
  bRes &= static_cast<bool>(connect(new QShortcut{QKeySequence{Qt::CTRL + Qt::Key_A}, this}, &QShortcut::activated, [&](){onActiveAll();}));

  bRes &= static_cast<bool>(connect(this, SIGNAL(treeWidgetItem(std::vector<QString>)), m_pFabricLayer, SLOT(create(std::vector<QString>))));
  bRes &= static_cast<bool>(connect(m_pFabricLayer, SIGNAL(createWidget(NNLayerWidget*)), this, SLOT(onCreateWidget(NNLayerWidget*))));

  assert(bRes);
}

void ConstructorWidget::makeActive(std::size_t sId, bool bActive)
{
  if(sId == SIZE_MAX){
    emit changeActiveLayer(nullptr);
    emit paramsChanged(nullptr);
    return;
  }

  auto pLayerWidget = m_vLayers[sId];

  pLayerWidget->makeActive(bActive);

  if (bActive){
    emit changeActiveLayer(pLayerWidget);
    emit paramsChanged(pLayerWidget->getParams());
  }
}


void ConstructorWidget::checkSizes()
{
  for (auto [id, pLayer] : m_vLayers)
    pLayer->resetInputSize();

  graph.checkSizes(m_vInputSize);
}

void ConstructorWidget::resizeEvent(QResizeEvent*)
{
  m_pWorkSpace->setMinimumSize(m_pWorkSpace->parentWidget()->size());
  m_pWorkSpace->resize(m_pWorkSpace->getQSize());
}

void ConstructorWidget::showEvent(QShowEvent*)
{
  resizeEvent(nullptr);
}

void ConstructorWidget::mouseReleaseEvent(QMouseEvent* /*pEvent*/)
{
  onChangeActive(SIZE_MAX);
}

//void ConstructorWidget::mouseMoveEvent(QMouseEvent* pEvent)
//{
//  NNLayerWidget* pLayer;
//  if (auto temp = std::find_if(
//          m_vLayers.begin(), m_vLayers.end(),
//          [](const auto pLayer) { return pLayer.second->isGrabbed(); });
//      temp != m_vLayers.end())
//    pLayer = temp->second;
//  else
//    return;

//  const int k_margin = 5;

//  QPoint layersPos{m_pWorkSpace->size().width(), m_pWorkSpace->size().height()};
//  QSize layersPosEnd{};

//  for(const auto i : m_ActiveSet){
//    auto pCurrent = m_vLayers[i];
//    layersPos.rx() = std::min(layersPos.x(), pCurrent->pos().x());
//    layersPos.ry() = std::min(layersPos.y(), pCurrent->pos().y());
//    layersPosEnd.rwidth() = std::max(layersPosEnd.width(), pCurrent->pos().x() + pCurrent->size().width());
//    layersPosEnd.rheight() = std::max(layersPosEnd.height(), pCurrent->pos().y() + pCurrent->size().height());
//  }
//  QSize layersSize{layersPosEnd};
//  layersSize.rwidth() -= layersPos.x();
//  layersSize.rheight() -= layersPos.y();
//  auto temp = layersPos - pLayer->pos();
//  auto temp2 = pEvent->pos();
//  auto temp3 = mapFromGlobal(mapToGlobal(pEvent->pos()));
//  auto eventWithCorPos = temp3 - pLayer->getGrabbedPos() + temp ;
//  QPoint shift{eventWithCorPos};

//  if(eventWithCorPos.x() - k_margin < 0)
//    shift.rx() = k_margin;
//  if(eventWithCorPos.y() - k_margin < 0)
//    shift.ry() = k_margin;
//  if(shift.x() != eventWithCorPos.x() || shift.y() != eventWithCorPos.y())
//    layersPos = shift;

//  for(const auto i : m_ActiveSet){
//    auto pCurrent = m_vLayers[i];
//    auto pos = pCurrent->pos();
//    pCurrent->move(pos - layersPos + shift);
//  }


//  QPoint resize{0, 0};
//  auto mainSize = m_pWorkSpace->size();
//  if(auto tmp = (layersPos.x() + layersSize.width()) - mainSize.width() + k_margin; tmp > 0)
//    resize.rx() = tmp;
//  if(auto tmp = (layersPos.y() + layersSize.height()) - mainSize.height() + k_margin; tmp > 0)
//    resize.ry() = tmp;
//  if(resize.x() != 0 || resize.y() != 0){
//    QPoint point = resize + QPoint{m_pWorkSpace->size().width(), m_pWorkSpace->size().height()};
//    m_pWorkSpace->setWidth(point.x());
//    m_pWorkSpace->setHeight(point.y());
//    m_pWorkSpace->resize(point.x(), point.y());
//  }


//  m_pWorkSpace->update();
//}


void ConstructorWidget::bfs(NNLayerWidget* pStart, bfs_proc fCurrentProc, bfs_proc fForwardsProc) const
{
  std::set<NNLayerWidget*> VisitedSet;
  std::queue<NNLayerWidget*> qToVisit;
  qToVisit.push(pStart);
  VisitedSet.insert(pStart);

  while(!qToVisit.empty()){
    auto pCurrent = qToVisit.front();
    qToVisit.pop();

    if (fCurrentProc)
      fCurrentProc(pCurrent, nullptr);

    for (auto pNext : pCurrent->getForward()){
      if (VisitedSet.find(pNext) != VisitedSet.cend())
        continue;

      VisitedSet.insert(pNext);

      if (fForwardsProc)
        fForwardsProc(pCurrent, pNext);

      qToVisit.push(pNext);
    }
  }
}

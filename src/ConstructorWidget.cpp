#include "ConstructorWidget.h"

#include <iostream>
#include <queue>
#include <set>
#include <sstream>

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QShortcut>

#include "Macros.h"

void print_params(const std::string& strParam, const std::string& strTag, std::stringstream& rStream) {
  rStream << "<" << strTag << ">" << strParam << "</" << strTag << ">" << std::endl;
}

void print_params(std::size_t sParam, const std::string& strTag, std::stringstream& rStream) {
  rStream << "<" << strTag << ">" << sParam << "</" << strTag << ">" << std::endl;
}

void print_params(const std::vector<std::size_t>& vParams, const std::string& strTag,
                  std::stringstream& rStream) {
  rStream << "<" << strTag << ">" << std::endl;
  for (const auto& crParam : vParams) print_params(crParam, "v", rStream);
  rStream << "</" << strTag << ">" << std::endl;
}

ConstructorWidget::ConstructorWidget()
    : m_pFabricLayer{new FabricLayer} {
  initGUI();
  createConnections();

  setMouseTracking(false);
}

auto ConstructorWidget::makeXmlString() const -> std::string {
  std::stringstream Stream;
  print_params(m_vInputSize, "data_size", Stream);
  Stream << "<layers>" << '\n';
  for (const auto [id, pCurrent] : m_layers) {
    Stream << "<l>" << std::endl;
    print_params(id, "id", Stream);
    Stream << pCurrent->getParams()->makeXmlString();
    Stream << "<forwards>" << '\n';
    for (auto id : m_graph.getForward(id)) print_params(id, "v", Stream);
    Stream << "</forwards>" << '\n';
    Stream << "</l>" << '\n';
  }
  Stream << "</layers>" << std::endl;
  return Stream.str();
}

auto ConstructorWidget::makePyString() -> std::string {
  emit startCalculation();
  auto result{m_graph.print()};
  emit compliteCalculation();
  return result;
}

bool ConstructorWidget::isEmpty() const noexcept { return m_layers.empty(); }

void ConstructorWidget::onSetParams(const std::shared_ptr<NNLayerParams>& spParams) {
  emit startCalculation();
  auto active = *m_activeSet.cbegin();
  auto pLayerWidget = m_layers[active];

  pLayerWidget->setParams(spParams);

  checkSizes();
  m_pWorkSpace->update();
  emit compliteCalculation();
}

void ConstructorWidget::onDeleteActive() {
  emit startCalculation();
  if (m_layers.empty()) return;
  while (! m_activeSet.empty()) {
    const auto active = *m_activeSet.cbegin();
    auto pDeletingLayer = m_layers[active];
    pDeletingLayer->deleteLayer();
    m_graph.deleteVertex(active);
    m_layers.erase(active);
    m_activeSet.erase(active);
  }
  if (! m_layers.empty()) {
    const auto id = m_layers.cbegin()->first;
    m_layers.at(id)->toActive();
  }

  checkSizes();
  m_pWorkSpace->update();
  emit compliteCalculation();
}

void ConstructorWidget::onDeleteEdge(KeyType sId) {
  emit startCalculation();
  if (m_layers.empty()) return;
  for (const auto active : m_activeSet) m_graph.deleteEdge(active, sId);
  checkSizes();
  m_pWorkSpace->update();
  emit compliteCalculation();
}

void ConstructorWidget::onSetInputSize(const Vector& vInputSize) { m_vInputSize = vInputSize; }

void ConstructorWidget::onAddLayer(const QPoint& crPoint, NNLayerWidget* pLayer) {
  emit startCalculation();
  auto id = pLayer->getId();
  m_pWorkSpace->add(crPoint, pLayer);
  // clang-format off
  CONNECT_CHECK(connect(pLayer, SIGNAL(becomeActive(KeyType)),  SLOT(onChangeActive(KeyType))));
  CONNECT_CHECK(connect(pLayer, SIGNAL(makeForward(KeyType)),   SLOT(onMakeForward(KeyType))));
  CONNECT_CHECK(connect(pLayer, SIGNAL(addToActive(KeyType)),   SLOT(onAddToActive(KeyType))));
  CONNECT_CHECK(connect(pLayer, SIGNAL(delFromActive(KeyType)), SLOT(onDelFromActive(KeyType))));
  
  CONNECT_CHECK(connect(pLayer,       SIGNAL(collectIsMoved(KeyType)),
                        m_pWorkSpace, SLOT(onSendIsMoved(KeyType))));
  CONNECT_CHECK(connect(pLayer,       SIGNAL(setIsMoved(bool)),
                        m_pWorkSpace, SLOT(onSetIsMoved(bool))));
  CONNECT_CHECK(connect(pLayer,       SIGNAL(grabbed(KeyType, bool)),
                        m_pWorkSpace, SLOT(onSetGrabbed(KeyType, bool))));
  CONNECT_CHECK(connect(pLayer,       SIGNAL(waitToDrag()),
                        m_pWorkSpace, SLOT(dragInit())));
  // clang-format on
  m_graph.addVertex(id);
  m_layers[id] = pLayer;
  clearActive();
  onAddToActive(id);

  checkSizes();
  m_pWorkSpace->update();
  emit compliteCalculation();
}

void ConstructorWidget::onChangeActive(KeyType sId) {
  clearActive();
  // if (sId == SIZE_MAX) return;
  m_layers.at(sId)->toActive();
}

void ConstructorWidget::onMakeForward(KeyType sId) {
  emit startCalculation();
  for (const auto active : m_activeSet) m_graph.addEdge(active, sId);
  if (m_activeSet.size() == 1) {
    const auto index = *m_activeSet.cbegin();
    emit changeActiveLayer(m_graph.getForward(index));
  }

  checkSizes();
  m_pWorkSpace->update();
  emit compliteCalculation();
}

void ConstructorWidget::onAddToActive(KeyType sId) {
  m_activeSet.insert(sId);
  updateParamWidget();
}

void ConstructorWidget::onDelFromActive(KeyType sId) {
  m_activeSet.erase(sId);
  updateParamWidget();
}

void ConstructorWidget::updateParamWidget() {
  if (m_activeSet.size() == 1) {
    const auto active = *m_activeSet.cbegin();
    const auto pLayerWidget = m_layers[active];
    emit changeActiveLayer(m_graph.getForward(active));
    emit paramsChanged(pLayerWidget->getParams());
  } else {
    emit clearActiveLayer();
    emit clearChanged();
  }
}

void ConstructorWidget::onActiveAll() {
  for (const auto [id, pLayer] : m_layers) pLayer->toActive();
}

void ConstructorWidget::onCreateWidget(NNLayerWidget* pLayer) {
  QPoint point = QPoint{this->rect().center()};
  onAddLayer(point, pLayer);
}

void ConstructorWidget::onCreateWidget(NNLayerWidget* pLayer, QPoint point) { onAddLayer(point, pLayer); }

void ConstructorWidget::initGUI() {
  auto pLayout = new QGridLayout{this};
  m_pWorkSpace = new ConstructorWorkSpace{m_layers, m_graph, m_activeSet};
  m_pWorkSpace->setAutoFillBackground(false);
  auto temp = new QScrollArea;
  temp->setWidget(m_pWorkSpace);
  m_pWorkSpace->show();
  pLayout->addWidget(temp);
  pLayout->setMargin(0);
}

void ConstructorWidget::createConnections() {
  // clang-format off
  CONNECT_CHECK(connect(&m_graph, SIGNAL(notValid(std::string)), SIGNAL(notValid(std::string))));
  
  CONNECT_CHECK(connect(new QShortcut{QKeySequence{Qt::CTRL + Qt::Key_A}, this},  &QShortcut::activated,
                        [&]() { onActiveAll(); }));
  CONNECT_CHECK(connect(new QShortcut{QKeySequence{Qt::CTRL + Qt::Key_C}, this},  &QShortcut::activated,
                        [&]() { copyToBuffer(); }));
  CONNECT_CHECK(connect(new QShortcut{QKeySequence{Qt::CTRL + Qt::Key_V}, this},  &QShortcut::activated,
                        [&]() { createFromBuffer(); }));
  CONNECT_CHECK(connect(new QShortcut{QKeySequence{Qt::Key_Delete}, this},        &QShortcut::activated,
                        [&]() { onDeleteActive(); }));

  CONNECT_CHECK(connect(this,           SIGNAL(treeWidgetItem(std::vector<QString>)),
                        m_pFabricLayer, SLOT(onCreate(std::vector<QString>))));
  // CONNECT_CHECK(connect(this,           SIGNAL(createCopy(const NNLayerWidget&, const QPoint&)),
  //                       m_pFabricLayer, SLOT(onCreateCopy(const NNLayerWidget&, const QPoint&))));
  
  CONNECT_CHECK(connect(m_pFabricLayer, SIGNAL(createWidget(NNLayerWidget*)),
                        this,           SLOT(onCreateWidget(NNLayerWidget*))));
  CONNECT_CHECK(connect(m_pFabricLayer, SIGNAL(createWidget(NNLayerWidget*, QPoint)),
                        this,           SLOT(onCreateWidget(NNLayerWidget*, QPoint))));
  // clang-format on
}

void ConstructorWidget::makeActive(KeyType sId, bool bActive) {
  if (sId == SIZE_MAX) {
    emit clearActiveLayer();
    emit clearChanged();
    return;
  }
  auto pLayerWidget = m_layers[sId];
  pLayerWidget->makeActive(bActive);
  if (bActive) {
    emit changeActiveLayer(m_graph.getForward(sId));
    emit paramsChanged(pLayerWidget->getParams());
  }
}

void ConstructorWidget::clearActive() {
  for (const auto index : m_activeSet) m_layers[index]->makeActive(false);
  m_activeSet.clear();
  emit clearActiveLayer();
  emit clearChanged();
}

void ConstructorWidget::copyToBuffer() {
  auto& vLayers = m_layersBuffer.first;
  auto& vPos = m_layersBuffer.second;
  vLayers.clear();
  vPos.clear();
  for (const auto index : m_activeSet) {
    vLayers.push_back(*m_layers.at(index));
    vPos.push_back(m_layers.at(index)->pos());
  }
}

void ConstructorWidget::createFromBuffer() {
  auto& vLayers = m_layersBuffer.first;
  auto& vPos = m_layersBuffer.second;
  assert(vLayers.size() == vPos.size());
  const auto size = vLayers.size();
  KeyType id;
  for (auto i = 0; i < size; ++i) {
    vPos[i] += QPoint{10, 10};
    id = m_pFabricLayer->onCreateCopy(vLayers[i], vPos[i]);
    onAddToActive(id);
  }
}

void ConstructorWidget::checkSizes() {
  for (auto [id, pLayer] : m_layers) pLayer->resetInputSize();
  m_graph.checkSizes(m_vInputSize);
}

void ConstructorWidget::resizeEvent(QResizeEvent*) {
  m_pWorkSpace->setMinimumSize(m_pWorkSpace->parentWidget()->size());
  m_pWorkSpace->resize(m_pWorkSpace->getQSize());
}

void ConstructorWidget::showEvent(QShowEvent*) { resizeEvent(nullptr); }

void ConstructorWidget::mousePressEvent(QMouseEvent* pEvent) {
  std::cerr << "Press Constructor " << QGuiApplication::keyboardModifiers() << std::endl;
}

void ConstructorWidget::mouseMoveEvent(QMouseEvent* pEvent) {
  std::cerr << "Move Constructor " << QGuiApplication::keyboardModifiers() << std::endl;
}

void ConstructorWidget::mouseReleaseEvent(QMouseEvent* pEvent) {
  std::cerr << "Release Constructor " << QGuiApplication::keyboardModifiers() << std::endl;
  clearActive();
}

void ConstructorWidget::bfs(KeyType iStart) const {
  std::unordered_set<KeyType> VisitedSet;
  std::queue<KeyType> qToVisit;
  qToVisit.push(iStart);
  VisitedSet.insert(iStart);
  while (! qToVisit.empty()) {
    auto iCurrent = qToVisit.front();
    qToVisit.pop();
    for (auto iNext : m_graph.getForward(iCurrent)) {
      if (VisitedSet.find(iNext) != VisitedSet.cend()) continue;
      VisitedSet.insert(iNext);
      qToVisit.push(iNext);
    }
  }
}

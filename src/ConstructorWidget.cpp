#include "ConstructorWidget.h"
#include <iostream>
#include <queue>
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
    : m_fabricManager{} {
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

auto ConstructorWidget::makePyString() -> std::string { return m_graph.print(); }

auto ConstructorWidget::save() const -> std::string {
  std::stringstream stream;
  stream << m_graph.save() << '\n';
  stream << m_layers.size() << '\n';
  stream << '\n';
  for (const auto [id, layer] : m_layers) { stream << layer->save(); }
  stream << '\n';
  return stream.str();
}

void ConstructorWidget::load(std::stringstream& stream) {
  m_layers.clear();
  m_graph.clear();
  m_graph.load(stream);
  size_t size;
  std::string type, subType;
  KeyType id, maxId{0};
  stream >> size;
  for (auto i = 0u; i < size; ++i) {
    stream >> type >> id >> subType;
    if (id > maxId) maxId = id;
    auto pLayer = m_fabricManager.load(QString::fromStdString(type), QString::fromStdString(subType), id);
    m_layers[id] = pLayer;
    pLayer->load(stream);
    pLayer->setParent(m_pWorkSpace);
    pLayer->makeActive(false);
    pLayer->updateStyle();
    pLayer->show();
    nnConnect(pLayer);
  }
  NNWidget::createId(maxId);

  updateThis();
  clearActive();
}

bool ConstructorWidget::isEmpty() const noexcept { return m_layers.empty(); }

void ConstructorWidget::onSetParams(const std::shared_ptr<NNLayerParams>& spParams) {
  auto active = *m_activeSet.cbegin();
  auto pLayerWidget = m_layers[active];

  pLayerWidget->setParams(spParams);
  pLayerWidget->checkIsValid();
  emit collectErrors(pLayerWidget->getErrors());

  updateThis();
}

void ConstructorWidget::onDeleteActive() {
  if (m_layers.empty()) return;
  while (! m_activeSet.empty()) {
    const auto active = *m_activeSet.cbegin();
    auto pDeletingLayer = m_layers[active];
    pDeletingLayer->deleteThis();
    m_graph.deleteVertex(active);
    m_layers.erase(active);
    m_activeSet.erase(active);
  }
  if (! m_layers.empty()) {
    const auto id = m_layers.cbegin()->first;
    m_layers.at(id)->toActive();
  }

  updateThis();
}

void ConstructorWidget::onDeleteEdge(KeyType id) {
  if (m_layers.empty()) return;
  for (const auto active : m_activeSet) m_graph.deleteEdge(active, id);
  updateThis();
}

void ConstructorWidget::onSetInputSize(const Vector& vInputSize) {
  m_vInputSize = vInputSize;
  updateThis();
}

void ConstructorWidget::nnConnect(NNWidget* pNN) { // clang-format off
  CONNECT_CHECK(connect(pNN, SIGNAL(becomeActive(KeyType)),  SLOT(onChangeActive(KeyType))));
  CONNECT_CHECK(connect(pNN, SIGNAL(makeForward(KeyType)),   SLOT(onMakeForward(KeyType))));
  CONNECT_CHECK(connect(pNN, SIGNAL(addToActive(KeyType)),   SLOT(onAddToActive(KeyType))));
  CONNECT_CHECK(connect(pNN, SIGNAL(delFromActive(KeyType)), SLOT(onDelFromActive(KeyType))));

  CONNECT_CHECK(connect(pNN,          SIGNAL(collectIsMoved(KeyType)),
                        m_pWorkSpace, SLOT(onSendIsMoved(KeyType))));
  CONNECT_CHECK(connect(pNN,          SIGNAL(setIsMoved(bool)),
                        m_pWorkSpace, SLOT(onSetIsMoved(bool))));
  CONNECT_CHECK(connect(pNN,          SIGNAL(grabbed(KeyType, QPoint)),
                        m_pWorkSpace, SLOT(onGrabbed(KeyType, QPoint))));
  CONNECT_CHECK(connect(pNN,          SIGNAL(ungrabbed()),
                        m_pWorkSpace, SLOT(onUngrabbed())));
} // clang-format on

void ConstructorWidget::onAddLayer(const QPoint& crPoint, NNWidget* pLayer) {
  auto id = pLayer->getId();
  m_pWorkSpace->add(crPoint, pLayer);

  nnConnect(pLayer);

  m_graph.addVertex(id);
  m_layers[id] = pLayer;
  clearActive();
  onAddToActive(id);

  updateThis();
}

void ConstructorWidget::onChangeActive(KeyType id) {
  clearActive();
  // if (sId == SIZE_MAX) return;
  m_layers.at(id)->toActive();
}

void ConstructorWidget::onMakeForward(KeyType id) {
  for (const auto active : m_activeSet) m_graph.addEdge(active, id);
  if (m_activeSet.size() == 1) {
    const auto index = *m_activeSet.cbegin();
    emit changeActiveLayer(m_graph.getForward(index));
  }

  updateThis();
}

void ConstructorWidget::onAddToActive(KeyType id) {
  m_activeSet.insert(id);
  auto pLayer = m_layers[id];
  pLayer->updateStyle();
  updateParamWidget();
}

void ConstructorWidget::onDelFromActive(KeyType id) {
  m_activeSet.erase(id);
  auto pLayer = m_layers[id];
  pLayer->makeActive(false);
  pLayer->updateStyle();
  updateParamWidget();
}

void ConstructorWidget::updateParamWidget() {
  if (m_activeSet.size() == 1) {
    const auto active = *m_activeSet.cbegin();
    const auto pLayerWidget = m_layers[active];
    emit changeActiveLayer(m_graph.getForward(active));
    emit paramsChanged(pLayerWidget->getParams());
    emit collectErrors(pLayerWidget->getErrors());
  } else {
    clearSignals();
  }
}

void ConstructorWidget::onActiveAll() {
  for (const auto [id, pLayer] : m_layers) pLayer->toActive();
}

void ConstructorWidget::onCreateWidget(NNWidget* pLayer) {
  QPoint point = QPoint{this->rect().center()};
  onAddLayer(point, pLayer);
}

void ConstructorWidget::onCreateWidget(NNWidget* pLayer, QPoint point) { onAddLayer(point, pLayer); }

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
  CONNECT_CHECK(connect(new QShortcut{QKeySequence{Qt::Key_Delete}, this},        &QShortcut::activated,
                        [&]() { onDeleteActive(); }));

  CONNECT_CHECK(connect(this,             SIGNAL(treeWidgetItem(std::vector<QString>)),
                        &m_fabricManager, SLOT(onCreate(std::vector<QString>))));
  // CONNECT_CHECK(connect(this,           SIGNAL(createCopy(const NNLayerWidget&, const QPoint&)),
  //                       m_pFabricLayer, SLOT(onCreateCopy(const NNLayerWidget&, const QPoint&))));
  
  CONNECT_CHECK(connect(&m_fabricManager, SIGNAL(createWidget(NNWidget*)),
                        this,             SLOT(onCreateWidget(NNWidget*))));
  CONNECT_CHECK(connect(&m_fabricManager, SIGNAL(createWidget(NNWidget*, QPoint)),
                        this,             SLOT(onCreateWidget(NNWidget*, QPoint))));
  // clang-format on
}

void ConstructorWidget::makeActive(KeyType id, bool bActive) {
  if (id == SIZE_MAX) {
    clearSignals();
    return;
  }
  auto pLayerWidget = m_layers[id];
  pLayerWidget->makeActive(bActive);
  if (bActive) {
    emit changeActiveLayer(m_graph.getForward(id));
    emit paramsChanged(pLayerWidget->getParams());
    emit collectErrors(pLayerWidget->getErrors());
  }
}

void ConstructorWidget::clearActive() {
  while (! m_activeSet.empty()) onDelFromActive(*m_activeSet.cbegin());
  m_activeSet.clear();
  clearSignals();
}

void ConstructorWidget::onCutToBuffer() {
  auto& vLayers = m_layersBuffer.first;
  auto& vPos = m_layersBuffer.second;
  vLayers.clear();
  vPos.clear();
  for (const auto index : m_activeSet) {
    vPos.push_back(m_layers.at(index)->pos());
    vLayers.push_back(std::move(m_layers.at(index)));
  }
  onDeleteActive();
}

void ConstructorWidget::onCopyToBuffer() {
  auto& vLayers = m_layersBuffer.first;
  auto& vPos = m_layersBuffer.second;
  vLayers.clear();
  vPos.clear();
  for (const auto index : m_activeSet) {
    const NNWidget* pNN = m_layers.at(index);
    // m_pFabricLayer->bufferCopy(pNN);
    vLayers.push_back(m_fabricManager.chooseFabric(pNN->getType())->bufferCopy(pNN));
    vPos.push_back(pNN->pos());
  }
}

void ConstructorWidget::onCreateFromBuffer() {
  auto& vLayers = m_layersBuffer.first;
  auto& vPos = m_layersBuffer.second;
  assert(vLayers.size() == vPos.size());
  const auto size = vLayers.size();
  KeyType id;
  std::queue<KeyType> activeQueue;
  for (auto i = 0u; i < size; ++i) {
    vPos[i] += QPoint{10, 10};
    id = m_fabricManager.chooseFabric(vLayers[i]->getType())->createCopy(vLayers[i], vPos[i]);
    activeQueue.push(id);
  }
  while (! activeQueue.empty()) {
    auto pLayer = m_layers[activeQueue.front()];
    pLayer->toActive();
    // onAddToActive();
    activeQueue.pop();
  }
}

void ConstructorWidget::checkSizes() {
  emit startCalculation();
  for (auto [id, pLayer] : m_layers) pLayer->resetInputAndOutputSize();
  m_graph.checkSizes(m_vInputSize);
  for (auto [id, pLayer] : m_layers) pLayer->updateStyle();
  emit compliteCalculation();
}

void ConstructorWidget::updateThis() {
  checkSizes();
  m_pWorkSpace->update();
}

void ConstructorWidget::clearSignals() {
  emit clearActiveLayer();
  emit clearChanged();
  emit clearErrors();
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

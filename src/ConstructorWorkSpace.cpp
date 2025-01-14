#include "ConstructorWorkSpace.h"
#include <algorithm>
#include <iostream>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>

ConstructorWorkSpace::ConstructorWorkSpace(const NNLayerContainer& vLayers, const GraphManager& graph,
                                           const HashSet& ActiveSet)
    : m_refvLayers(vLayers)
    , m_refGraph(graph)
    , m_refActiveSet(ActiveSet) {
  initGUI();
}

void ConstructorWorkSpace::add(const QPoint& crPoint, NNWidget* pLayer) {
  pLayer->setParent(this);
  pLayer->move(mapFromParent(crPoint));
  pLayer->show();
}

auto ConstructorWorkSpace::getQSize() const -> QSize { return {width, height}; }

bool ConstructorWorkSpace::isMoved() const { return m_isMoved; }

void ConstructorWorkSpace::setMoved() { m_isMoved = true; }

void ConstructorWorkSpace::onSendIsMoved(KeyType index) { m_refvLayers.at(index)->setMoved(m_isMoved); }

void ConstructorWorkSpace::onSetIsMoved(bool value) { m_isMoved = value; }

void ConstructorWorkSpace::initGUI() {
  setObjectName("ConstructorWorkSpace");
  this->setStyleSheet("QWidget#ConstructorWorkSpace{background-color: yellow}");
}

void ConstructorWorkSpace::paintEvent(QPaintEvent* pEvent) {
  QPainter Painter{this};
  // clang-format off
  Painter.fillRect(
      QRect{{0, 0}, this->size()},
      QBrush{Qt::lightGray});
  // clang-format on
  Painter.setPen(QPen(Qt::black, 4, Qt::SolidLine, Qt::FlatCap));

  for (const auto [id, pLayer] : m_refvLayers) {
    for (const auto iForward : m_refGraph.getForward(id)) {
      const auto pForward = m_refvLayers.at(iForward);
      const auto From = pLayer->pos() + QPoint{pLayer->rect().center().x(), pLayer->rect().bottom()};
      const auto To = pForward->pos() + QPoint{pForward->rect().center().x(), 0};
      Painter.drawLine(From, To);
      Painter.drawEllipse(To, 5, 5);
    }
  }
}

void ConstructorWorkSpace::mousePressEvent(QMouseEvent* pEvent) {
  std::cerr << "Press WorkSpace" << QGuiApplication::keyboardModifiers() << std::endl;

  // QCoreApplication::sendEvent(parent(), pEvent);
}

void ConstructorWorkSpace::mouseMoveEvent(QMouseEvent* pEvent) {
  std::cerr << "Move WorkSpace" << QGuiApplication::keyboardModifiers() << std::endl;

  drag(pEvent->pos());

  m_isMoved = true;
  update();
}

void ConstructorWorkSpace::mouseReleaseEvent(QMouseEvent* pEvent) {
  std::cerr << "Release WorkSpace " << QGuiApplication::keyboardModifiers() << std::endl;
  auto button = pEvent->button();
  if (button == Qt::LeftButton) {
    QCoreApplication::sendEvent(parent(), pEvent);
  } else if (button == Qt::RightButton) {
  }
  // QCoreApplication::sendEvent(parent(), pEvent);
}

void ConstructorWorkSpace::resizeEvent(QResizeEvent*) {
  //  this->resize(width, height);
}

void ConstructorWorkSpace::drag(const QPoint eventPos) {
  if (! isMoved()) {
    dragInit();
    setMoved();
  }
  auto pLayer = dragChoose();
  if (! pLayer) return;
  const int k_margin = 5;
  const auto shift = dragCheckBorders(k_margin, pLayer->pos(), eventPos);
  dragMove(shift);
  dragResize(k_margin);
  m_activeDragAreaPos = shift; // update DragArea
}

void ConstructorWorkSpace::dragInit() {
  m_activeDragAreaPos = QPoint{this->size().width(), this->size().height()};
  m_activeDragAreaSize = QSize{};
  for (const auto i : m_refActiveSet) {
    auto pCurrent = m_refvLayers.at(i);
    m_activeDragAreaPos.rx() = std::min(m_activeDragAreaPos.x(), pCurrent->pos().x());
    m_activeDragAreaPos.ry() = std::min(m_activeDragAreaPos.y(), pCurrent->pos().y());
    m_activeDragAreaSize.rwidth() = std::max(m_activeDragAreaSize.width(),
                                             pCurrent->pos().x() + pCurrent->size().width());
    m_activeDragAreaSize.rheight() = std::max(m_activeDragAreaSize.height(),
                                              pCurrent->pos().y() + pCurrent->size().height());
  }
  m_activeDragAreaSize.rwidth() -= m_activeDragAreaPos.x();
  m_activeDragAreaSize.rheight() -= m_activeDragAreaPos.y();
}

auto ConstructorWorkSpace::dragChoose() const -> NNWidget* {
  if (m_GrabbedIndex != SIZE_MAX) return m_refvLayers.at(m_GrabbedIndex);
  return nullptr;
}

auto ConstructorWorkSpace::dragCheckBorders(const int k_margin, const QPoint& layerPos, const QPoint eventPos)
    -> QPoint {
  const auto currentPos = m_activeDragAreaPos - layerPos;
  // auto eventPosWithCorrections = eventPos - pLayer->getGrabbedPos() + currentPos;
  auto eventPosWithCorrections = eventPos - m_GrabbedPos + currentPos;
  QPoint shift{eventPosWithCorrections};

  if (eventPosWithCorrections.x() - k_margin < 0) shift.rx() = k_margin;
  if (eventPosWithCorrections.y() - k_margin < 0) shift.ry() = k_margin;
  // if (shift.x() == k_margin || shift.y() == k_margin) m_activeDragAreaPos = shift;

  return shift;
}

void ConstructorWorkSpace::dragMove(const QPoint shift) {
  const auto shift_ = shift - m_activeDragAreaPos;
  for (const auto i : m_refActiveSet) {
    auto pCurrent = m_refvLayers.at(i);
    const auto pos = pCurrent->pos();
    pCurrent->move(pos + shift_);
  }
}

void ConstructorWorkSpace::dragResize(const int k_margin) {
  QPoint resize{0, 0};
  const auto mainSize = this->size();
  if (auto tmp = (m_activeDragAreaPos.x() + m_activeDragAreaSize.width()) - mainSize.width() + k_margin;
      tmp > 0)
    resize.rx() = tmp;
  if (auto tmp = (m_activeDragAreaPos.y() + m_activeDragAreaSize.height()) - mainSize.height() + k_margin;
      tmp > 0)
    resize.ry() = tmp;
  if (resize.x() != 0 || resize.y() != 0) {
    QPoint point = resize + QPoint{this->size().width(), this->size().height()};
    width = point.x();
    height = point.y();
    this->resize(point.x(), point.y());
  }
}

void ConstructorWorkSpace::onGrabbed(KeyType index, QPoint pos) {
  m_GrabbedIndex = index;
  m_GrabbedPos = pos;
}

void ConstructorWorkSpace::onUngrabbed() {
  m_GrabbedIndex = SIZE_MAX;
  m_GrabbedPos = {};
}

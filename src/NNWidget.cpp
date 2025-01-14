#include "NNWidget.h"
#include <iostream>
#include <QApplication>
#include <QMouseEvent>
#include <QVBoxLayout>

NNWidget::NNWidget(KeyType id)
    : m_id{id}
    , m_isActive{false}
    , m_isValidParams{true}
    , m_isGlobalError{false}
    , m_isMoved{false}
    , m_strType{"Widget"}{}

NNWidget::NNWidget(KeyType id, QString name)
    : m_id{id}
    , m_isActive{false}
    , m_isValidParams{true}
    , m_isGlobalError{false}
    , m_isMoved{false}
    , m_strType{std::move(name)}{}

NNWidget::NNWidget(const NNWidget& another)
    : QWidget{another.parentWidget()} {
  this->m_id = another.m_id;
  this->m_isActive = another.m_isActive;
  this->m_isValidParams = another.m_isValidParams;
  this->m_isGlobalError = another.m_isGlobalError;
  this->m_isMoved = another.m_isMoved;
  this->m_strType = another.m_strType;
}

auto NNWidget::createId(KeyType id) -> KeyType {
  static KeyType m_classId = 0;
  if (id != SIZE_MAX) m_classId = id;
  return m_classId++;
}

auto NNWidget::makeSizeString(const Vector& vSize) -> QString {
  QString qstrRes = "[";
  if (! vSize.empty()) {
    auto it = vSize.begin();
    qstrRes += QString::number(*it);
    ++it;
    for (const auto ite = vSize.end(); it != ite; ++it) qstrRes += ", " + QString::number(*it);
  }
  qstrRes += "]";
  return qstrRes;
}

auto NNWidget::getId() const -> KeyType { return m_id; }

auto NNWidget::getErrors() const -> const std::string& { return m_strErrors; }

auto NNWidget::getType() const -> const QString& { return m_strType; }

bool NNWidget::isActive() const { return m_isActive; }

bool NNWidget::isMoved() const { return m_isMoved; }

void NNWidget::setMoved(bool value) { m_isMoved = value; }

void NNWidget::changeId(KeyType id) { m_id = id; }

void NNWidget::mousePressEvent(QMouseEvent* pEvent) {
  std::cerr << "Press NNWidget " << QGuiApplication::keyboardModifiers() << std::endl;
  auto modif = QGuiApplication::keyboardModifiers();
  auto button = pEvent->button();
  m_isMoved = false;
  if (Qt::NoModifier == modif) {
    switch (button) {
      case Qt::LeftButton  : pressLeft(pEvent); break;
      case Qt::RightButton : QCoreApplication::sendEvent(parent(), pEvent); break;
      default              :;
    }
  } else if (modif.testFlag(Qt::ControlModifier)) {
    switch (button) {
      case Qt::LeftButton  : break;
      case Qt::RightButton : break;
      default              :;
    }
  }
}

// void NNLayerWidget::mouseMoveEvent(QMouseEvent* pEvent) {
//   std::cerr << "Move NNWidget " << QGuiApplication::keyboardModifiers() << std::endl;
//   QCoreApplication::sendEvent(parent(), pEvent);
// }

void NNWidget::mouseReleaseEvent(QMouseEvent* pEvent) {
  std::cerr << "Release NNWidget " << QGuiApplication::keyboardModifiers() << std::endl;
  emit ungrabbed();
  emit collectIsMoved(m_id);
  auto modif = QGuiApplication::keyboardModifiers();
  auto button = pEvent->button();
  if (isMoved()) {
    if (Qt::NoModifier == modif) {
      switch (button) {
        case Qt::LeftButton  : break;
        case Qt::RightButton : break;
        default              :;
      }
    } else if (modif.testFlag(Qt::ControlModifier)) {
      switch (button) {
        case Qt::LeftButton  : break;
        case Qt::RightButton : break;
        default              :;
      }
    }
  } else {
    if (Qt::NoModifier == modif) {
      switch (button) {
        case Qt::LeftButton  : makeActive(true); break;
        case Qt::RightButton : emit makeForward(m_id); break;
        default              :;
      }
    } else if (modif.testFlag(Qt::ControlModifier)) {
      switch (button) {
        case Qt::LeftButton : releaseCtrlLeft(); break;
        default             :;
      }
    }
  }

  std::cerr << "Release NNWidget isMoved " << isMoved() << std::endl;
  setMoved(false);
  emit setIsMoved(m_isMoved);
  // QCoreApplication::sendEvent(parent(), pEvent);
}

void NNWidget::makeActive(bool bActive) {
  m_isActive = bActive;
  if (m_isActive) emit becomeActive(m_id);
}

void NNWidget::toActive() {
  m_isActive = true;
  emit addToActive(m_id);
}

void NNWidget::deleteThis() { close(); }

void NNWidget::releaseCtrlLeft() { ///@todo
  if (! m_isMoved) {
    m_isActive = ! m_isActive;
    // updateStyle();
    if (m_isActive)
      emit addToActive(m_id);
    else
      emit delFromActive(m_id);
  }
}

void NNWidget::pressLeft(QMouseEvent* pEvent) {
  emit grabbed(m_id, pEvent->pos());
  if (! m_isActive) makeActive(true);
}

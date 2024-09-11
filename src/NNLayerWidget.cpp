#include "NNLayerWidget.h"
#include <iostream>
#include <QApplication>
#include <QMouseEvent>
#include <QVBoxLayout>

QColor make_color(bool bActive, bool bValid) {
  if (bValid)
    return bActive ? QColor{125, 255, 125} : QColor{125, 125, 255};
  else
    return bActive ? QColor{200, 200, 125} : QColor{255, 125, 125};
}

NNLayerWidget::NNLayerWidget(KeyType sId, const std::shared_ptr<NNLayerParams>& spParams)
    : m_sId{sId}
    , m_isGrabbed{false}
    , m_isValidParams{true}
    , m_isMoved{false}
    , m_spParams{spParams}
    , m_pNameLabel{new QLabel{createName()}}
    , m_pOutputLabel{new QLabel{makeSizeString()}} {
  setAttribute(Qt::WA_DeleteOnClose);
  if (m_vInputSizes.empty()) {
    if (m_spParams->getName() != "Bilinear") {
      m_pInputLabel = new QLabel{makeSizeString()};
    } else {
      QString str = makeSizeString() + "\n" + makeSizeString();
      m_pInputLabel = new QLabel{str};
    }
  } else if (m_spParams->getName() != "Bilinear") {
    m_pInputLabel = new QLabel{makeSizeString(m_vInputSizes.front())};
  } else {
    m_pInputLabel = new QLabel{makeSizeString(m_vInputSizes.front()) + "\n"
                               + makeSizeString(m_vInputSizes.back())};
  }

  initGUI();
}

NNLayerWidget::NNLayerWidget(const NNLayerWidget& another)
    : QWidget{} {
  this->m_sId = another.m_sId;
  this->m_GrabbedPos = another.m_GrabbedPos;

  this->m_isActive = another.m_isActive;
  this->m_isGrabbed = another.m_isGrabbed;
  this->m_isValidParams = another.m_isValidParams;
  this->m_isMoved = another.m_isMoved;

  this->m_spParams = another.getParams()->copy();
  this->m_vInputSizes = another.m_vInputSizes;

  this->m_pInputLabel = new QLabel{another.m_pInputLabel};
  this->m_pNameLabel = new QLabel{another.m_pNameLabel};
  this->m_pOutputLabel = new QLabel{another.m_pOutputLabel};
}

auto NNLayerWidget::makeSizeString(const Vector& vSize) -> QString {
  QString qstrRes = "[";
  for (auto sSize : vSize) qstrRes += QString::number(sSize) + ", ";
  qstrRes += "]";
  return qstrRes;
}

auto NNLayerWidget::createId() -> KeyType {
  static KeyType m_classId = 0;
  return m_classId++;
}

auto NNLayerWidget::getId() const -> KeyType { return m_sId; }

auto NNLayerWidget::getParams() const noexcept -> const std::shared_ptr<NNLayerParams>& { return m_spParams; }

auto NNLayerWidget::getInputSize() const -> const std::vector<Vector>& { return m_vInputSizes; }

auto NNLayerWidget::getGrabbedPos() const noexcept -> const QPoint& { return m_GrabbedPos; }

void NNLayerWidget::setParams(const std::shared_ptr<NNLayerParams>& spParams) noexcept {
  m_spParams = spParams;
  m_pNameLabel->setText(createName());
}

void NNLayerWidget::setMoved(bool value) { m_isMoved = value; }

void NNLayerWidget::changeId(KeyType id) { m_sId = id; }

void NNLayerWidget::deleteLayer() { close(); }

bool NNLayerWidget::isGrabbed() const { return m_isGrabbed; }

bool NNLayerWidget::isActive() const { return m_isActive; }

bool NNLayerWidget::isMoved() const { return m_isMoved; }

void NNLayerWidget::pressLeft(QMouseEvent* pEvent) {
  m_isGrabbed = true;
  emit grabbed(m_sId, m_isGrabbed);
  emit waitToDrag();
  m_GrabbedPos = pEvent->pos();
  if (! m_isActive) makeActive(true);
}

void NNLayerWidget::mousePressEvent(QMouseEvent* pEvent) {
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

void NNLayerWidget::mouseReleaseEvent(QMouseEvent* pEvent) {
  std::cerr << "Release NNWidget " << QGuiApplication::keyboardModifiers() << std::endl;
  m_isGrabbed = false;
  emit grabbed(m_sId, m_isGrabbed);
  emit collectIsMoved(m_sId);
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
        case Qt::RightButton : emit makeForward(m_sId); break;
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

void NNLayerWidget::releaseCtrlLeft() {
  if (! m_isGrabbed) {
    m_isActive = ! m_isActive;
    updateStyle();
    if (m_isActive)
      emit addToActive(m_sId);
    else
      emit delFromActive(m_sId);
  }
}

void NNLayerWidget::toActive() {
  m_isActive = true;
  updateStyle();
  emit addToActive(m_sId);
}

void NNLayerWidget::makeActive(bool bActive) {
  m_isActive = bActive;
  if (m_isActive) emit becomeActive(m_sId);

  updateStyle();
}

void NNLayerWidget::resetInputSize() { m_vInputSizes.clear(); }

void NNLayerWidget::addInputSize(const Vector& vInputSize) ///< @todo
{
  if (vInputSize.empty()) {
    m_isValidParams = false;
  } else {
    m_vInputSizes.push_back(vInputSize);
    m_isValidParams = m_spParams->checkInputSize(m_vInputSizes);
  }
  updateStyle();
}

auto NNLayerWidget::calcOutputSize() const -> Vector {
  if (m_vInputSizes.empty()) return {};
  return m_spParams->calcOutputSize(m_vInputSizes);
}

void NNLayerWidget::initGUI() {
  setAutoFillBackground(true);
  makeActive(true);

  auto pLayout = new QVBoxLayout{this};

  m_pInputLabel->setAlignment(Qt::AlignHCenter);
  m_pNameLabel->setAlignment(Qt::AlignHCenter);
  m_pOutputLabel->setAlignment(Qt::AlignHCenter);

  pLayout->addWidget(m_pInputLabel);
  pLayout->addWidget(m_pNameLabel);
  pLayout->addWidget(m_pOutputLabel);

  pLayout->setContentsMargins(5, 5, 5, 5);
  setLayout(pLayout);
}

void NNLayerWidget::updateStyle() {
  auto Color = make_color(m_isActive, m_isValidParams);

  auto Palette = palette();
  Palette.setColor(QPalette::Window, Color);
  Palette.setColor(QPalette::Button, Color);
  setPalette(Palette);

  auto sMaxSize = -1;
  auto sizeInputSize = m_vInputSizes.size();
  switch (sizeInputSize) {
    case 0 :
      m_pInputLabel->setText(makeSizeString());
      m_pOutputLabel->setText(makeSizeString());
      break;
    case 1 :
      m_pInputLabel->setText(makeSizeString(m_vInputSizes.front()));
      m_pOutputLabel->setText(makeSizeString(calcOutputSize()));
      break;
    case 2 :
      QString str = makeSizeString(m_vInputSizes.front()) + makeSizeString(m_vInputSizes.back());
      m_pInputLabel->setText(str);
      m_pOutputLabel->setText(makeSizeString(calcOutputSize()));
  }

  auto sInputStrSize = fontMetrics().horizontalAdvance(m_pInputLabel->text());
  auto sNameStrSize = fontMetrics().horizontalAdvance(m_pNameLabel->text());
  auto sOutputStrSize = fontMetrics().horizontalAdvance(m_pOutputLabel->text());

  sMaxSize = std::max(sMaxSize, sInputStrSize);
  sMaxSize = std::max(sMaxSize, sNameStrSize);
  sMaxSize = std::max(sMaxSize, sOutputStrSize);

  setFixedWidth(sMaxSize + 5 * 2);
}

auto NNLayerWidget::createName() const -> QString {
  return m_spParams->getDisplayName() + " " + QString::number(m_sId);
}

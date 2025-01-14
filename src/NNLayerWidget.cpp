#include "NNLayerWidget.h"
#include <iostream>
#include <QApplication>
#include <QMouseEvent>
#include <QVBoxLayout>



QColor make_color(bool bActive, bool bValid) {
  if (bValid)
    return bActive ? QColor{125, 255, 125} : QColor{125, 125, 255};
  return bActive ? QColor{200, 200, 125} : QColor{255, 125, 125};
}

NNLayerWidget::NNLayerWidget(KeyType id, const std::shared_ptr<NNLayerParams>& spParams)
    : NNWidget{id, "Layer"}
    , m_spParams{spParams}
    , m_pInputLabel{new QLabel}
    , m_pNameLabel{new QLabel}
    , m_pOutputLabel{new QLabel} {
  setAttribute(Qt::WA_DeleteOnClose);
  if (m_vInputSizes.empty()) {
    if (m_spParams->getName() != "Bilinear") {
      m_pInputLabel->setText(makeSizeString());
    } else {
      QString str = makeSizeString() + makeSizeString();
      m_pInputLabel->setText(str);
    }
  } else {
    if (m_spParams->getName() != "Bilinear") {
      m_pInputLabel->setText(makeSizeString(m_vInputSizes.front()));
    } else {
      QString str = makeSizeString(m_vInputSizes.front()) + makeSizeString(m_vInputSizes.back());
      m_pInputLabel->setText(str);
    }
  }

  initGUI();
}

NNLayerWidget::NNLayerWidget(const NNLayerWidget& another)
    : NNWidget{another} {
  this->m_spParams = another.getParams()->copy();
  this->m_vInputSizes = another.m_vInputSizes;
  this->m_pInputLabel = new QLabel{another.m_pInputLabel};
  this->m_pNameLabel = new QLabel{another.m_pNameLabel};
  this->m_pOutputLabel = new QLabel{another.m_pOutputLabel};
}

auto NNLayerWidget::getParams() const noexcept -> const std::shared_ptr<NNLayerParams>& { return m_spParams; }

auto NNLayerWidget::getInputSize() const -> const VectorOfVector& { return m_vInputSizes; }

auto NNLayerWidget::getMainOutputSize() -> Vector {
  if (m_vOutputSizes.empty()) m_vOutputSizes.push_back({});
  return m_vOutputSizes.front();
}

auto NNLayerWidget::getOptionalOutputSize(KeyType index) -> Vector {
  if (m_vOutputSizes.empty()) m_vOutputSizes.push_back({});
  if (m_vOutputSizes.size() <= index) return {};
  return m_vOutputSizes[index];
}

void NNLayerWidget::setParams(const std::shared_ptr<NNLayerParams>& spParams) noexcept {
  m_spParams = spParams;
  m_pNameLabel->setText(createName());
}

void NNLayerWidget::updateStyle() {
  auto Color = make_color(m_isActive, m_isValidParams);

  auto Palette = palette();
  Palette.setColor(QPalette::Window, Color);
  Palette.setColor(QPalette::Button, Color);
  setPalette(Palette);

  auto sMaxSize = -1;
  m_pNameLabel->setText(createName());
  {
    QString str;
    switch (m_vInputSizes.size()) {
      case 0 : str = makeSizeString(); break;
      case 1 : str = makeSizeString(m_vInputSizes.front()); break;
      case 2 : str = makeSizeString(m_vInputSizes.front()) + makeSizeString(m_vInputSizes.back());
    }
    m_pInputLabel->setText(str);
  }

  {
    QString str;
    switch (m_vOutputSizes.size()) {
      case 0 : str = makeSizeString(); break;
      case 1 : str = makeSizeString(m_vOutputSizes.front()); break;
      case 2 : str = makeSizeString(m_vOutputSizes.front()) + makeSizeString(m_vOutputSizes.back()); break;
      case 3 :
        str = makeSizeString(m_vOutputSizes[0]) + makeSizeString(m_vOutputSizes[1])
            + makeSizeString(m_vOutputSizes[2]);
    }
    m_pOutputLabel->setText(str);
  }

  auto sInputStrSize = fontMetrics().horizontalAdvance(m_pInputLabel->text());
  auto sNameStrSize = fontMetrics().horizontalAdvance(m_pNameLabel->text());
  auto sOutputStrSize = fontMetrics().horizontalAdvance(m_pOutputLabel->text());

  sMaxSize = std::max(sMaxSize, sInputStrSize);
  sMaxSize = std::max(sMaxSize, sNameStrSize);
  sMaxSize = std::max(sMaxSize, sOutputStrSize);

  setFixedWidth(sMaxSize + 5 * 2);
}

auto NNLayerWidget::save() const -> std::string {
  std::stringstream stream;
  stream << "Layer" << '\n';
  stream << m_id << '\n';
  stream << m_spParams->save() /* << '\n'*/;
  const auto pos_ = pos();
  stream << pos_.x() << ' ' << pos_.y() << '\n' << '\n';

  return stream.str();
}

void NNLayerWidget::load(std::stringstream& stream) {
  m_spParams->load(stream);
  QPoint pos_;
  stream >> pos_.rx() >> pos_.ry();
  move(pos_);
}

void NNLayerWidget::resetInputSize() { m_vInputSizes.clear(); }

void NNLayerWidget::resetInputAndOutputSize() {
  m_vInputSizes.clear();
  m_vOutputSizes.clear();
}

void NNLayerWidget::checkIsValid() {
  m_isGlobalError = false;
  m_strErrors = m_spParams->checkInputSize(m_vInputSizes, m_isGlobalError);
  m_isValidParams = m_strErrors.empty();
}

void NNLayerWidget::addInputSize(const Vector& vInputSize) ///< @todo
{
  if (vInputSize.empty()) {
    m_isValidParams = false;
  } else {
    m_vInputSizes.push_back(vInputSize);
    checkIsValid();
  }
  updateStyle();
}

auto NNLayerWidget::calcOutputSize() -> const VectorOfVector& {
  if (m_vInputSizes.empty() || m_isGlobalError) {
    m_vOutputSizes.clear();
    m_vOutputSizes.push_back({});
  } else {
    m_vOutputSizes = m_spParams->calcOutputSize(m_vInputSizes);
  }
  return m_vOutputSizes;
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

auto NNLayerWidget::createName() const -> QString {
  return m_spParams->getDisplayName() + " " + QString::number(m_id);
}

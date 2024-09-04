#include "NNLayerWidget.h"


#include <iostream>

#include <QMenu>
#include <QMouseEvent>
#include <QVBoxLayout>

#include <QShortcut>
#include <QApplication>

QColor make_color(bool bActive, bool bValid)
{
  if (bValid)
    return bActive?  QColor{125, 255, 125} : QColor{125, 125, 255};
  else
    return bActive?  QColor{200, 200, 125} : QColor{255, 125, 125};
}

QString make_size_string(const std::vector<std::size_t>& vSize = {})
{
  QString qstrRes = "[";
  for (auto sSize : vSize)
    qstrRes += QString::number(sSize) + ", ";

  qstrRes += "]";
  return qstrRes;
}

//std::shared_ptr<NNLayerParams> NNLayerWidget::createLayer()
//{
////  m_fabricLayer->create();
//  return std::make_shared<LinearLayerParams>();
//}

NNLayerWidget::NNLayerWidget(std::size_t sId, const std::shared_ptr<NNLayerParams>& spParams)
  : m_sId{sId},
    m_bGrabbed{false},
    m_spParams{spParams},
    m_bValidParams{true},
     //////////// doSomthing
    m_pNameLabel{new QLabel{m_spParams->getDisplayName() + " " + QString::number(sId)}},
    m_pOutputLabel{new QLabel{make_size_string({})}}
{
  setAttribute(Qt::WA_DeleteOnClose);
  if(m_vInputSizes.empty()){
    if(m_spParams->getName() != "Bilinear"){
      m_pInputLabel  = new QLabel{make_size_string()};
    }else{
      QString str = make_size_string() + "\n" + make_size_string();
      m_pInputLabel = new QLabel{str};
    }
  }else{
    if(m_spParams->getName() != "Bilinear"){
      m_pInputLabel  = new QLabel{make_size_string(m_vInputSizes.front())};
    }else{
      m_pInputLabel = new QLabel{make_size_string(m_vInputSizes.front())
                      + "\n" + make_size_string(m_vInputSizes.back())};
    }
  }


  initGUI();
}

std::size_t NNLayerWidget::getId() const
{
  return m_sId;
}

void NNLayerWidget::setParams(const std::shared_ptr<NNLayerParams>& spParams) noexcept
{
  m_spParams = spParams;
  m_pNameLabel->setText(m_spParams->getDisplayName());
}

const std::shared_ptr<NNLayerParams>& NNLayerWidget::getParams() const noexcept
{
  return m_spParams;
}

void NNLayerWidget::deleteLayer()
{
  close();
}

bool NNLayerWidget::isGrabbed() const
{
  return m_bGrabbed;
}

bool NNLayerWidget::isActive() const
{
  return m_bActive;
}

void NNLayerWidget::mousePressEvent(QMouseEvent* pEvent)
{
  auto modif = QGuiApplication::keyboardModifiers();
  if(Qt::NoModifier == modif){
    if (pEvent->button() == Qt::LeftButton){
      m_bGrabbed = true;
      m_GrabbedPos = pEvent->pos();
      if(!m_bActive)
        makeActive(true);
    }else if (pEvent->button() == Qt::RightButton){
      emit makeForward(m_sId);
    }
  }else if(modif.testFlag(Qt::ControlModifier)){
    if (pEvent->button() == Qt::LeftButton){
      m_bGrabbed = true;
      m_GrabbedPos = pEvent->pos();
      m_bActive = !m_bActive;
      updateStyle();
      if(m_bActive)
        emit addToActive(m_sId);
      else
        emit delFromActive(m_sId);
    }
  }
}

void NNLayerWidget::toActive()
{
  m_bActive = true;
  updateStyle();
  emit addToActive(m_sId);
}

void NNLayerWidget::mouseReleaseEvent(QMouseEvent* /*pEvent*/)
{
  m_bGrabbed = false;
}

void NNLayerWidget::makeActive(bool bActive)
{
  m_bActive = bActive;
  if (m_bActive)
    emit becomeActive(m_sId);

  updateStyle();
}

const QPoint& NNLayerWidget::getGrabbedPos() const noexcept
{
  return m_GrabbedPos;
}

void NNLayerWidget::addForward(NNLayerWidget* pForward)
{
  auto it = std::find(m_vForwards.begin(), m_vForwards.end(), pForward);
  if (it != m_vForwards.end())
    return ;

  m_vForwards.push_back(pForward);
}

void NNLayerWidget::removeForward(NNLayerWidget* pForward)
{
  auto itForward = std::find(m_vForwards.begin(), m_vForwards.end(), pForward);

  if (itForward != m_vForwards.end())
    m_vForwards.erase(itForward);
}

const std::vector<NNLayerWidget*>& NNLayerWidget::getForward() const noexcept
{
  return m_vForwards;
}

void NNLayerWidget::resetInputSize()
{
  m_vInputSizes.clear();
}

void NNLayerWidget::addInputSize(const std::vector<std::size_t>& vInputSize) ///< @todo
{
  if (vInputSize.empty()){
    m_bValidParams = false;
  }
  else{
    m_vInputSizes.push_back(vInputSize);
    m_bValidParams = m_spParams->checkInputSize(m_vInputSizes);
  }
  updateStyle();
}

std::vector<std::size_t> NNLayerWidget::calcOutputSize() const
{
  if (m_vInputSizes.empty())
    return {};

  return m_spParams->calcOutputSize(m_vInputSizes);
}

void NNLayerWidget::initGUI()
{
  setAutoFillBackground(true);

  setContextMenuPolicy(Qt::CustomContextMenu);

  makeActive(true);

  auto pLayout = new QVBoxLayout{this};

  m_pInputLabel->setAlignment(Qt::AlignHCenter);
  m_pNameLabel->setAlignment(Qt::AlignHCenter);
  m_pOutputLabel->setAlignment(Qt::AlignHCenter);

  pLayout->addWidget(m_pInputLabel);
  pLayout->addWidget(m_pNameLabel);
  pLayout->addWidget(m_pOutputLabel);

  pLayout->setContentsMargins(5,5,5,5);

  setLayout(pLayout);
}

void NNLayerWidget::updateStyle()
{
  auto Color = make_color(m_bActive, m_bValidParams);

  auto Palette = palette();
  Palette.setColor(QPalette::Window, Color);
  Palette.setColor(QPalette::Button, Color);
  setPalette(Palette);

  auto sMaxSize = -1;
  if(m_vInputSizes.empty()){
    m_pInputLabel->setText(make_size_string());
  }else if(m_vInputSizes.size() == 1){
    m_pInputLabel->setText(make_size_string(m_vInputSizes.front()));
  }else if(m_vInputSizes.size() == 2){
    QString str = make_size_string(m_vInputSizes.front()) + make_size_string(m_vInputSizes.back());
    m_pInputLabel->setText(str);
  }

  if (!m_vInputSizes.empty())
    m_pOutputLabel->setText(make_size_string(calcOutputSize()));
  else
    m_pOutputLabel->setText(make_size_string());

  auto sInputStrSize = fontMetrics().horizontalAdvance(m_pInputLabel->text());
  auto sNameStrSize = fontMetrics().horizontalAdvance(m_pNameLabel->text());
  auto sOutputStrSize = fontMetrics().horizontalAdvance(m_pOutputLabel->text());

  sMaxSize = std::max(sMaxSize, sInputStrSize);
  sMaxSize = std::max(sMaxSize, sNameStrSize);
  sMaxSize = std::max(sMaxSize, sOutputStrSize);

  setFixedWidth(sMaxSize + 5 * 2);
}

const std::vector<std::vector<std::size_t>> &NNLayerWidget::getInputSize() const
{
  return m_vInputSizes;
}

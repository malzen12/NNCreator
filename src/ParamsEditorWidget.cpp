#include "ParamsEditorWidget.h"

#include <QHBoxLayout>
#include "ParamWidget.h"
#include <QToolButton>

#include <iostream>

ParamsEditorWidget::ParamsEditorWidget()
  : m_pNameLabel{new QLabel},
    m_pForwardLayout{new QVBoxLayout},
    m_pParamsLayout{new QVBoxLayout},
    m_pConfirm{new QPushButton{"Confirm"}},
    m_pDelete{new QPushButton{"Delete"}}
{
  initGUI();
  createConnections();
}

void ParamsEditorWidget::onForward(NNLayerWidget* pLayer)
{
  initForward(pLayer);
  update();
}

void ParamsEditorWidget::onSetParams(const std::shared_ptr<NNLayerParams>& spParams)
{
  m_spParams = spParams;
  initEditors();
  update();
}

void ParamsEditorWidget::onConfirmParams()
{
  if (!m_spParams->isValid())
    return ;

  m_spParams = collectParams();

  emit paramsChanged(m_spParams);
}

void ParamsEditorWidget::initGUI()
{
  auto pButtonsLayout = new QHBoxLayout;

  pButtonsLayout->addWidget(m_pConfirm);
  pButtonsLayout->addWidget(m_pDelete);

  pButtonsLayout->setContentsMargins(0,0,0,0);

  auto pMainLayout = new QVBoxLayout{this};

  pMainLayout->addWidget(m_pNameLabel);
  pMainLayout->addLayout(m_pForwardLayout);
  pMainLayout->addLayout(m_pParamsLayout);
  pMainLayout->addLayout(pButtonsLayout);
  pMainLayout->addSpacerItem(new QSpacerItem{10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding});

  pMainLayout->setContentsMargins(0,0,0,0);
}

void ParamsEditorWidget::createConnections()
{
  auto bRes = true;

  bRes &= static_cast<bool>(connect(m_pConfirm, SIGNAL(clicked(bool)), SLOT(onConfirmParams())));
  bRes &= static_cast<bool>(connect(m_pDelete, SIGNAL(clicked(bool)), SIGNAL(deleteActive())));


  assert(bRes);
}


void ParamsEditorWidget::initForward(const NNLayerWidget* pLayer)
{

  while(!m_pForwardLayout->isEmpty()){
    auto pItem = m_pForwardLayout->takeAt(0);
    auto layout = pItem->layout()->takeAt(0);
    auto button = layout->widget();
    button->close();

    layout = pItem->layout()->takeAt(0);
    auto label = layout->widget();
    label->close();

    delete layout;
  }

  if(pLayer != nullptr){
    for(const auto& pCurrent : pLayer->getForward()){
      auto temp = new Edge{pCurrent->getId()};
      auto bRes = true;
      bRes &= static_cast<bool>(connect(temp, SIGNAL(deletedEdge(std::size_t)), SIGNAL(deletedEdge(std::size_t))));
      assert(bRes);
      m_pForwardLayout->addLayout(temp->getLayout());
    }
  }else{
    m_pNameLabel->clear();
  }

}

void ParamsEditorWidget::initEditors()
{

  while (auto pItem = m_pParamsLayout->itemAt(0)){
    auto pWdg = pItem->widget();
    if (!pWdg)
      continue;

    pWdg->close();
    m_pParamsLayout->removeWidget(pWdg);
  }

  if(m_spParams != nullptr){
    m_pNameLabel->setText(QString::fromStdString(m_spParams->getName()));

    for(const auto& spParam : m_spParams->getParams())
      m_pParamsLayout->addWidget(new ParamWidget{spParam});
  }else{
    m_pNameLabel->clear();
  }
}

const std::shared_ptr<NNLayerParams>& ParamsEditorWidget::collectParams()
{
  std::vector<NNParam> vParams;
  vParams.reserve(m_pParamsLayout->count());

  for (auto i = 0; i < m_pParamsLayout->count(); ++i){
    auto pWdg = static_cast<ParamWidget*>(m_pParamsLayout->itemAt(i)->widget());
    if (pWdg)
      vParams.push_back(pWdg->collectValue());
  }

  m_spParams->setParams(vParams);
  return m_spParams;
}

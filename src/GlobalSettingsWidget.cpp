#include "GlobalSettingsWidget.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>

#include "ParamWidget.h"

GlobalSettingsWidget::GlobalSettingsWidget()
  :
    m_pTrainSettingsLayout{new QGridLayout},
    m_pMakeTrainXmlButton{new QPushButton{"Make train xml"}},
    m_pNetSettingsLayout{new QGridLayout},
    m_pConfirmSizeButton{new QPushButton{"Confirm"}},
    m_pMakeNetLabel{new QLabel{"Make net"}},
    m_pMakeNetXmlButton{new QPushButton{"XML"}},
    m_pMakeNetPyButton{new QPushButton{"Python"}},
    m_pIsValidButton{new QPushButton{"Check the validity"}}
{
  initGUI();
  createConnections();

}

void GlobalSettingsWidget::onUpdateInputSize()
{
  auto List = collectInputSize().getValue().toList();

  std::vector<std::size_t> vInputSize;

  for (const auto& crVal : List){
    auto sAxis = crVal.toUInt();
    if (sAxis)
      vInputSize.push_back(sAxis);
  }

  emit inputSizeChanged(vInputSize);
}

void GlobalSettingsWidget::onMakeTrainXml()
{
  emit makeTrainXml(collectParams(m_pTrainSettingsLayout));
}

void GlobalSettingsWidget::onMakeNetXml()
{
  emit makeNetXml(collectParams(m_pNetSettingsLayout));
}

void GlobalSettingsWidget::onMakeNetPy()
{
  emit makeNetPy(collectParams(m_pNetSettingsLayout));
}

void GlobalSettingsWidget::initGUI()
{
  m_pTrainSettingsLayout->addWidget(new ParamWidget{NNParam{"Optimizer", "Optimizer", QVariant::Type::String, true}}, 0, 0, 1, 3);
  m_pTrainSettingsLayout->addWidget(new ParamWidget{NNParam{"Optimizer_speed", 0.001, QVariant::Type::Double}}, 1, 0, 1, 3);
  m_pTrainSettingsLayout->addWidget(new ParamWidget{NNParam{"weight_decay", 0, QVariant::Type::Double}}, 2, 0, 1, 3);
  m_pTrainSettingsLayout->addWidget(new ParamWidget{NNParam{"Epoch_cnt", 100}}, 3, 0, 1, 3);
  m_pTrainSettingsLayout->addWidget(new ParamWidget{NNParam{"Scheduler", "Scheduler", QVariant::Type::String, true}}, 4, 0, 1, 3);
  m_pTrainSettingsLayout->addWidget(new ParamWidget{NNParam{"Train Xml path", "Xmls\\TestTrain", QVariant::Type::String}}, 5, 0, 1, 3);
  m_pTrainSettingsLayout->addWidget(m_pMakeTrainXmlButton, 6, 0, 1, 3);

  m_pNetSettingsLayout->addWidget(new ParamWidget{NNParam{"Input size", {}, QVariant::Type::List}}, 0, 0, 1, 2);
  m_pNetSettingsLayout->addWidget(m_pConfirmSizeButton, 0, 2);
  m_pNetSettingsLayout->addWidget(new ParamWidget{NNParam{"NN path", "Xmls\\TestNN", QVariant::Type::String}}, 1, 0, 1, 3);
  m_pNetSettingsLayout->addWidget(m_pMakeNetLabel, 2, 0);
  m_pNetSettingsLayout->addWidget(m_pMakeNetXmlButton, 2, 1);
  m_pNetSettingsLayout->addWidget(m_pMakeNetPyButton, 2, 2);
  m_pNetSettingsLayout->addWidget(m_pIsValidButton, 3, 0, 1, 3);

  auto pMainLayout = new QVBoxLayout{this};
  pMainLayout->addLayout(m_pTrainSettingsLayout);
  pMainLayout->addLayout(m_pNetSettingsLayout);
  pMainLayout->addSpacerItem(new QSpacerItem{10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding});

}

void GlobalSettingsWidget::createConnections()
{
  bool bRes = true;

  bRes &= static_cast<bool>(connect(m_pConfirmSizeButton, SIGNAL(clicked(bool)), SLOT(onUpdateInputSize())));

  bRes &= static_cast<bool>(connect(m_pMakeNetXmlButton, SIGNAL(clicked(bool)), SLOT(onMakeNetXml())));
  bRes &= static_cast<bool>(connect(m_pMakeNetPyButton, SIGNAL(clicked(bool)), SLOT(onMakeNetPy())));
  bRes &= static_cast<bool>(connect(m_pMakeTrainXmlButton, SIGNAL(clicked(bool)), SLOT(onMakeTrainXml())));
  bRes &= static_cast<bool>(connect(m_pIsValidButton, SIGNAL(clicked(bool)), SIGNAL(isValid())));

  assert(bRes);
}

std::vector<NNParam> GlobalSettingsWidget::collectParams(QGridLayout* pLayout)
{
  std::vector<NNParam> vParams;

  for (auto i = 0; i < pLayout->count(); ++i){
    auto pWdg = dynamic_cast<ParamWidget*>(pLayout->itemAt(i)->widget());
    if (pWdg)
      vParams.push_back(pWdg->collectValue());
  }

  return vParams;
}

NNParam GlobalSettingsWidget::collectInputSize()
{
  for (auto i = 0; i < m_pNetSettingsLayout->count(); ++i){
    auto pWdg = dynamic_cast<ParamWidget*>(m_pNetSettingsLayout->itemAt(i)->widget());
    if (pWdg && "Input size" == pWdg->getName())
      return pWdg->collectValue();
  }
  assert(false);
  return NNParam{"Input size", {}, QVariant::Type::List};
}

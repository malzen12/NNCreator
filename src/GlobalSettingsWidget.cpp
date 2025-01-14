#include "GlobalSettingsWidget.h"
#include "Macros.h"
#include "ParamWidget.h"
#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <iostream>
#include <QFileDialog>

GlobalSettingsWidget::GlobalSettingsWidget()
    : m_pFileLayout{new QGridLayout}
    , m_pWorkDirLabel{new QLabel}
    , m_pTrainSettingsLayout{new QGridLayout}
    , m_pMakeTrainXmlButton{new QPushButton{"Make train xml"}}
    , m_pNetSettingsLayout{new QGridLayout}
    , m_pConfirmSizeButton{new QPushButton{"Confirm"}} {
  initGUI();
  createConnections();
}

auto GlobalSettingsWidget::getName() const -> std::string {
  return m_pFileNameParamWidget->collectValue().convertToString();
}

void GlobalSettingsWidget::onUpdateInputSize() {
  const auto List = collectInputSize().getValue().toList();
  Vector vInputSize;
  for (const auto& crVal : List) {
    auto sAxis = crVal.toUInt();
    if (sAxis) vInputSize.push_back(sAxis);
  }
  emit inputSizeChanged(vInputSize);
}

void GlobalSettingsWidget::onMakeTrainXml() { emit makeTrainXml(collectParams(m_pTrainSettingsLayout)); }

void GlobalSettingsWidget::onChoseWorkDir() {
  auto fileName = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this));
  QDir homeDir = QDir::current();
  homeDir.setFilter(QDir::NoFilter | QDir::AllEntries);
  QString dir = homeDir.relativeFilePath(fileName);
  m_pWorkDirLabel->setText(dir);
  emit setWorkDir(dir.toStdString());
}

void GlobalSettingsWidget::initGUI() {
  // clang-format off
  
  auto pWorkDirLayout = new QHBoxLayout{};
  pWorkDirLayout->addWidget(new QLabel{"Work Dir:"});
  pWorkDirLayout->addWidget(m_pWorkDirLabel);
  m_pFileLayout->addLayout(pWorkDirLayout, 0, 0);

  m_pFileNameParamWidget = new ParamWidget{NNParam{"File name", "TestNN", QVariant::Type::String}};
  m_pFileLayout->addWidget(
      m_pFileNameParamWidget, 1, 0);



  m_pTrainSettingsLayout->addWidget(
      new ParamWidget{NNParam{"Optimizer", "Optimizer", QVariant::Type::String, true}},0, 0, 1, 3);
  m_pTrainSettingsLayout->addWidget(
      new ParamWidget{NNParam{"Optimizer_speed", 0.001, QVariant::Type::Double}}, 1, 0, 1, 3);
  m_pTrainSettingsLayout->addWidget(
      new ParamWidget{NNParam{"weight_decay", 0, QVariant::Type::Double}}, 2, 0, 1, 3);
  m_pTrainSettingsLayout->addWidget(
      new ParamWidget{NNParam{"Epoch_cnt", 100}}, 3, 0, 1, 3);
  m_pTrainSettingsLayout->addWidget(
      new ParamWidget{NNParam{"Scheduler", "Scheduler", QVariant::Type::String, true}}, 4, 0, 1, 3);
  m_pTrainSettingsLayout->addWidget(
      new ParamWidget{NNParam{"Train Xml path", "Xmls\\TestTrain", QVariant::Type::String}}, 5, 0, 1, 3);
  m_pTrainSettingsLayout->addWidget(
      m_pMakeTrainXmlButton, 6, 0, 1, 3);

  m_pNetSettingsLayout->addWidget(
      new ParamWidget{NNParam{"Input size", {}, QVariant::Type::List}}, 0, 0, 1, 2);
  m_pNetSettingsLayout->addWidget(
      m_pConfirmSizeButton, 0, 2);

  // clang-format on
  auto pMainLayout = new QVBoxLayout{this};
  pMainLayout->addLayout(m_pFileLayout);
  pMainLayout->addLayout(m_pTrainSettingsLayout);
  pMainLayout->addLayout(m_pNetSettingsLayout);
}

void GlobalSettingsWidget::createConnections() {
  // clang-format off
  CONNECT_CHECK(connect(m_pConfirmSizeButton,   SIGNAL(clicked(bool)), SLOT(onUpdateInputSize())));
  CONNECT_CHECK(connect(m_pMakeTrainXmlButton,  SIGNAL(clicked(bool)), SLOT(onMakeTrainXml())));
  // clang-format on
}

auto GlobalSettingsWidget::collectParams(QGridLayout* pLayout) -> NNParamContainer {
  NNParamContainer vParams;
  for (auto i = 0; i < pLayout->count(); ++i) {
    auto pWidget = static_cast<ParamWidget*>(pLayout->itemAt(i)->widget());
    if (pWidget) vParams.push_back(pWidget->collectValue());
  }
  return vParams;
}

auto GlobalSettingsWidget::collectInputSize() -> NNParam {
  for (auto i = 0; i < m_pNetSettingsLayout->count(); ++i) {
    auto pWidget = static_cast<ParamWidget*>(m_pNetSettingsLayout->itemAt(i)->widget());
    if (pWidget && "Input size" == pWidget->getName()) return pWidget->collectValue();
  }
  assert(false);
  return NNParam{"Input size", {}, QVariant::Type::List};
}

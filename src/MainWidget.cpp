#include "MainWidget.h"
#include "Macros.h"
#include <fstream>
#include <sstream>
#include <QHBoxLayout>

MainWidget::MainWidget()
    : m_pGlobalSettingsWidget{new GlobalSettingsWidget}
    , m_pConstructorWidget{new ConstructorWidget}
    , m_pParamsEditorWidget{new ParamsEditorWidget}
    , m_pCreateLayerWidget{new CreateLayerWidget} {
  initGUI();
  createConnections();
}

void MainWidget::onWriteTrainXml(const NNParamContainer& vParams) const {
  const std::string strName = "Train Xml path";
  std::stringstream stream;
  auto itPath = std::find_if(vParams.cbegin(), vParams.cend(),
                             [&strName](const NNParam& crParam) { return strName == crParam.getName(); });

  for (auto it = vParams.cbegin(), ite = vParams.cend(); it != ite; ++it)
    if (itPath != it) stream << it->makeXmlString();

  writeXml(itPath->getValue().toString().toStdString(), stream.str());
}

void MainWidget::onWriteNetXml(const NNParamContainer& vParams) const {
  const std::string strName = "NN path";
  auto itPath = std::find_if(vParams.cbegin(), vParams.cend(),
                             [&strName](const NNParam& crParam) { return strName == crParam.getName(); });

  if (! m_pConstructorWidget->isEmpty())
    writeXml(itPath->getValue().toString().toStdString(), m_pConstructorWidget->makeXmlString());
  else
    writeXml(itPath->getValue().toString().toStdString(), "");
}

void MainWidget::onWriteNetPy(const NNParamContainer& vParams) const {
  const std::string strName = "NN path";
  auto itPath = std::find_if(vParams.cbegin(), vParams.cend(),
                             [&strName](const NNParam& crParam) { return strName == crParam.getName(); });
  if (! m_pConstructorWidget->isEmpty())
    writePy(itPath->getValue().toString().toStdString(), m_pConstructorWidget->makePyString());
  else
    writePy(itPath->getValue().toString().toStdString(), "");
}

void MainWidget::onCheckIsValid() const {}

void MainWidget::initGUI() {
  auto pLayoutV = new QVBoxLayout{this};
  auto pLayoutH = new QHBoxLayout;
  auto pLayoutLeftV = new QVBoxLayout;

  pLayoutLeftV->addWidget(m_pGlobalSettingsWidget);
  pLayoutLeftV->addWidget(m_pCreateLayerWidget);

  pLayoutH->addLayout(pLayoutLeftV, 10);
  pLayoutH->addWidget(m_pConstructorWidget, 80);
  pLayoutH->addWidget(m_pParamsEditorWidget, 10);

  m_pGlobalWarningWidget = new GlobalWarningWidget;
  pLayoutV->addLayout(pLayoutH, 90);
  pLayoutV->addWidget(m_pGlobalWarningWidget, 10);

  resize(800, 800);
}

void MainWidget::createConnections() { // clang-format off
  CONNECT_CHECK(connect(m_pConstructorWidget,   SIGNAL(paramsChanged(std::shared_ptr<NNLayerParams>)),
                        m_pParamsEditorWidget,  SLOT(onSetParams(std::shared_ptr<NNLayerParams>))));
  CONNECT_CHECK(connect(m_pConstructorWidget,   SIGNAL(changeActiveLayer(HashSet)),
                        m_pParamsEditorWidget,  SLOT(onForward(HashSet))));
  CONNECT_CHECK(connect(m_pConstructorWidget,   SIGNAL(clearChanged()),
                        m_pParamsEditorWidget,  SLOT(onClearEditors())));
  CONNECT_CHECK(connect(m_pConstructorWidget,   SIGNAL(clearActiveLayer()),
                        m_pParamsEditorWidget,  SLOT(onClearForward())));


  CONNECT_CHECK(connect(m_pParamsEditorWidget,  SIGNAL(paramsChanged(std::shared_ptr<NNLayerParams>)),
                        m_pConstructorWidget,   SLOT(onSetParams(std::shared_ptr<NNLayerParams>))));
  CONNECT_CHECK(connect(m_pParamsEditorWidget,  SIGNAL(deleteActive()),
                        m_pConstructorWidget,   SLOT(onDeleteActive())));
  CONNECT_CHECK(connect(m_pParamsEditorWidget,  SIGNAL(deletedEdge(KeyType)),
                        m_pConstructorWidget,   SLOT(onDeleteEdge(KeyType))));

  CONNECT_CHECK(connect(m_pGlobalSettingsWidget,  SIGNAL(makeTrainXml(NNParamContainer)),
                                                  SLOT(onWriteTrainXml(NNParamContainer))));
  CONNECT_CHECK(connect(m_pGlobalSettingsWidget,  SIGNAL(makeNetXml(NNParamContainer)),
                                                  SLOT(onWriteNetXml(NNParamContainer))));
  CONNECT_CHECK(connect(m_pGlobalSettingsWidget,  SIGNAL(makeNetPy(NNParamContainer)),
                                                  SLOT(onWriteNetPy(NNParamContainer))));
  CONNECT_CHECK(connect(m_pGlobalSettingsWidget,  SIGNAL(isValid()),
                                                  SLOT(onCheckIsValid())));

  CONNECT_CHECK(connect(m_pGlobalSettingsWidget,  SIGNAL(inputSizeChanged(Vector)),
                        m_pConstructorWidget,     SLOT(onSetInputSize(Vector))));

  CONNECT_CHECK(connect(m_pConstructorWidget,   SIGNAL(notValid(std::string)),
                        m_pGlobalWarningWidget, SLOT(collecteWarning(std::string))));
  CONNECT_CHECK(connect(m_pConstructorWidget,   SIGNAL(startCalculation()),
                        m_pGlobalWarningWidget, SLOT(clearWarnings())));
  CONNECT_CHECK(connect(m_pConstructorWidget,   SIGNAL(compliteCalculation()),
                        m_pGlobalWarningWidget, SLOT(printWarnings())));

  CONNECT_CHECK(connect(m_pCreateLayerWidget, SIGNAL(treeWidgetItem(std::vector<QString>)),
                        m_pConstructorWidget, SIGNAL(treeWidgetItem(std::vector<QString>))));
} // clang-format on

void MainWidget::writeXml(const std::string& strXmlPath, const std::string& strXmlBody) const {
  //    std::fstream FStream{strXmlPath, std::fstream::out};
  std::ofstream FStream{strXmlPath + ".xml"};
  assert(FStream.is_open());

  FStream << "<?Stream version=\"1.0\"?>" << '\n';
  FStream << "<data>" << '\n';
  FStream << strXmlBody;
  FStream << "</data>" << std::endl;
}

void MainWidget::writePy(const std::string& strPyPath, const std::string& strPyBody) const {
  std::ofstream FStream{strPyPath + ".py"};
  assert(FStream.is_open());

  FStream << // clang-format off
R"(import pandas as pd
import numpy as np

import torch
import torch.nn as nn
import torch.nn.functional as F)" // clang-format on
          << '\n';
  FStream << strPyBody;
}

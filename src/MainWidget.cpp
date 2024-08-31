#include "MainWidget.h"

#include <fstream>
#include <sstream>

#include <QHBoxLayout>


#include <QShortcut>

MainWidget::MainWidget()
  : m_pGlobalSettingsWidget{new GlobalSettingsWidget},
    m_pConstructorWidget{new ConstructorWidget},
    m_pParamsEditorWidget{new ParamsEditorWidget}
{
  initGUI();
  createConnections();


}

void MainWidget::onWriteTrainXml(const std::vector<NNParam>& vParams) const
{
  const std::string strName = "Train Xml path";

  std::stringstream Stream;
  auto itPath = std::find_if(vParams.cbegin(), vParams.cend(), [&strName](const NNParam& crParam)
  {
    return strName == crParam.getName();
  });

  for(auto it = vParams.cbegin(), ite = vParams.cend(); it != ite; ++it ){
    if(itPath != it)
      Stream << it->makeXmlString();
  }

  writeXml(itPath->getValue().toString().toStdString(), Stream.str());
}

void MainWidget::onWriteNetXml(const std::vector<NNParam>& vParams) const
{
  const std::string strName = "NN path";

  auto itPath = std::find_if(vParams.cbegin(), vParams.cend(), [&strName](const NNParam& crParam)
  {
    return strName == crParam.getName();
  });

  if(!m_pConstructorWidget->isEmpty())
    writeXml(itPath->getValue().toString().toStdString(), m_pConstructorWidget->makeXmlString());
  else
    writeXml(itPath->getValue().toString().toStdString(), "");
}

void MainWidget::onWriteNetPy(const std::vector<NNParam>& vParams) const
{
  const std::string strName = "NN path";

  auto itPath = std::find_if(vParams.cbegin(), vParams.cend(), [&strName](const NNParam& crParam)
  {
    return strName == crParam.getName();
  });
  if(!m_pConstructorWidget->isEmpty())
    writePy(itPath->getValue().toString().toStdString(), m_pConstructorWidget->makePyString());
  else
    writePy(itPath->getValue().toString().toStdString(), "");
}

void MainWidget::onCheckIsValid() const
{

}


void MainWidget::initGUI()
{
  auto pLayoutV = new QVBoxLayout{this};
  auto pLayoutH = new QHBoxLayout{pLayoutV->widget()};

  pLayoutH->addWidget(m_pGlobalSettingsWidget, 10);
  pLayoutH->addWidget(m_pConstructorWidget, 80);
  pLayoutH->addWidget(m_pParamsEditorWidget, 10);

  m_pGlobalWarningWidget = new GlobalWarningWidget;
  pLayoutV->addLayout(pLayoutH, 90);
  pLayoutV->addWidget(m_pGlobalWarningWidget, 10);
//  pLayoutV->addWidget(m_pGlobalSettingsWidget, 10);

  resize(800, 800);
}

void MainWidget::createConnections()
{
  auto bRes = true;

  bRes &= static_cast<bool>(connect(m_pConstructorWidget, SIGNAL(paramsChanged(std::shared_ptr<NNLayerParams>)), m_pParamsEditorWidget, SLOT(onSetParams(std::shared_ptr<NNLayerParams>))));
  bRes &= static_cast<bool>(connect(m_pConstructorWidget, SIGNAL(changeActiveLayer(NNLayerWidget*)), m_pParamsEditorWidget, SLOT(onForward(NNLayerWidget*))));

  bRes &= static_cast<bool>(connect(m_pParamsEditorWidget, SIGNAL(paramsChanged(std::shared_ptr<NNLayerParams>)), m_pConstructorWidget, SLOT(onSetParams(std::shared_ptr<NNLayerParams>))));
  bRes &= static_cast<bool>(connect(m_pParamsEditorWidget, SIGNAL(deleteActive()), m_pConstructorWidget, SLOT(onDeleteActive())));
  bRes &= static_cast<bool>(connect(m_pParamsEditorWidget, SIGNAL(deletedEdge(std::size_t)), m_pConstructorWidget, SLOT(onDeleteEdge(std::size_t))));

  bRes &= static_cast<bool>(connect(m_pGlobalSettingsWidget, SIGNAL(makeTrainXml(std::vector<NNParam>)), SLOT(onWriteTrainXml(std::vector<NNParam>))));
  bRes &= static_cast<bool>(connect(m_pGlobalSettingsWidget, SIGNAL(makeNetXml(std::vector<NNParam>)), SLOT(onWriteNetXml(std::vector<NNParam>))));
  bRes &= static_cast<bool>(connect(m_pGlobalSettingsWidget, SIGNAL(makeNetPy(std::vector<NNParam>)), SLOT(onWriteNetPy(std::vector<NNParam>))));
  bRes &= static_cast<bool>(connect(m_pGlobalSettingsWidget, SIGNAL(isValid()), SLOT(onCheckIsValid())));

  bRes &= static_cast<bool>(connect(m_pGlobalSettingsWidget, SIGNAL(inputSizeChanged(std::vector<std::size_t>)), m_pConstructorWidget, SLOT(onSetInputSize(std::vector<std::size_t>))));

  bRes &= static_cast<bool>(connect(m_pConstructorWidget, SIGNAL(notValid(std::string)), m_pGlobalWarningWidget, SLOT(collecteWarning(std::string))));
  bRes &= static_cast<bool>(connect(m_pConstructorWidget, SIGNAL(startCalculation()), m_pGlobalWarningWidget, SLOT(clearWarnings())));
  bRes &= static_cast<bool>(connect(m_pConstructorWidget, SIGNAL(compliteCalculation()), m_pGlobalWarningWidget, SLOT(printWarnings())));

  assert(bRes);
}

void MainWidget::writeXml(const std::string& strXmlPath, const std::string& strXmlBody) const
{
  //    std::fstream FStream{strXmlPath, std::fstream::out};
  std::ofstream FStream{strXmlPath + ".xml"};
  assert(FStream.is_open());

  FStream << "<?Stream version=\"1.0\"?>" << '\n';
  FStream << "<data>" << std::endl;

  FStream << strXmlBody;

  FStream << "</data>" << std::endl;
}

void MainWidget::writePy(const std::string& strPyPath, const std::string& strPyBody) const
{
  std::ofstream FStream{strPyPath + ".py"};
  assert(FStream.is_open());

  FStream <<
R"(import pandas as pd
import numpy as np

import torch
import torch.nn as nn
import torch.nn.functional as F)"
          << '\n';

  FStream << strPyBody;
}

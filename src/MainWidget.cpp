#include "MainWidget.h"
#include "Macros.h"
#include <fstream>
#include <sstream>
#include <QHBoxLayout>
#include <QMenuBar>

MainWidget::MainWidget()
    : m_pGlobalSettingsWidget{new GlobalSettingsWidget}
    , m_pConstructorWidget{new ConstructorWidget}
    , m_pParamsEditorWidget{new ParamsEditorWidget}
    , m_pCreateLayerWidget{new CreateNNWidget} {
  initGUI();
  createConnections();
}

void MainWidget::onWriteTrainXml(const NNParamContainer& vParams) {
  const std::string strName = "Train Xml path";
  std::stringstream stream;
  auto itPath = std::find_if(vParams.cbegin(), vParams.cend(),
                             [&strName](const NNParam& crParam) { return strName == crParam.getName(); });

  for (auto it = vParams.cbegin(), ite = vParams.cend(); it != ite; ++it)
    if (itPath != it) stream << it->makeXmlString();

  writeXml(stream.str());
}

void MainWidget::onWriteNetXml() {
  if (! m_pConstructorWidget->isEmpty())
    writeXml(m_pConstructorWidget->makeXmlString());
  else
    writeXml("");
}

void MainWidget::onWriteNetPy() {
  if (! m_pConstructorWidget->isEmpty())
    writePy(m_pConstructorWidget->makePyString());
  else
    writePy("");
}

void MainWidget::onSave() {
  setFileName();
  const auto filePath = QDir::toNativeSeparators(QString::fromStdString(m_strPath + "/" + m_strName + ".nn"));
  std::ofstream FStream{filePath.toStdString()};
  assert(FStream.is_open());
  FStream << m_pConstructorWidget->save();
}

void MainWidget::onLoad() {
  const auto filePath = QFileDialog::getOpenFileName();
  if (filePath.isEmpty()) return;
  QFile file{filePath};
  file.open(QIODevice::ReadOnly);
  std::stringstream stream{file.readAll().toStdString()};
  m_pConstructorWidget->load(stream);
}

void MainWidget::onSetWorkDir(const std::string& str) {
  auto absolutePath = QDir{QString::fromStdString(str)};
  absolutePath.makeAbsolute();
  m_strPath = absolutePath.path().toStdString();
}

void MainWidget::initGUI() {
  auto menu = menuBar();
  auto fileMenu = menu->addMenu(tr("&File"));
  QAction* newAct = new QAction(tr("&New"), this);
  newAct->setShortcuts(QKeySequence::New);
  newAct->setStatusTip(tr("Create a new file"));
  fileMenu->addAction(newAct);

  QAction* saveAct = new QAction(tr("&Save"), this);
  saveAct->setShortcuts(QKeySequence::Save);
  saveAct->setStatusTip(tr("Save the document to disk"));
  connect(saveAct, SIGNAL(triggered(bool)), this, SLOT(onSave()));
  fileMenu->addAction(saveAct);

  QAction* openAct = new QAction(tr("&Open"), this);
  openAct->setShortcuts(QKeySequence::Open);
  openAct->setStatusTip(tr("Open the document from disk"));
  connect(openAct, SIGNAL(triggered(bool)), this, SLOT(onLoad()));
  fileMenu->addAction(openAct);

  auto exportMenu = fileMenu->addMenu("Export As");
  QAction* exportAsXmlAct = new QAction(tr("&Xml"), this);
  exportAsXmlAct->setStatusTip(tr("Export as Xml"));
  CONNECT_CHECK(connect(exportAsXmlAct, SIGNAL(triggered(bool)), this, SLOT(onWriteNetXml())));
  exportMenu->addAction(exportAsXmlAct);
  QAction* exportAsPyAct = new QAction(tr("&Python"), this);
  exportAsPyAct->setStatusTip(tr("Export as Python"));
  CONNECT_CHECK(connect(exportAsPyAct, SIGNAL(triggered(bool)), this, SLOT(onWriteNetPy())));
  exportMenu->addAction(exportAsPyAct);

  QAction* choseWorkDir = new QAction(tr("&Chose work dir"), this);
  choseWorkDir->setStatusTip(tr("Chose work dir"));
  CONNECT_CHECK(
      connect(choseWorkDir, SIGNAL(triggered(bool)), m_pGlobalSettingsWidget, SLOT(onChoseWorkDir())));
  fileMenu->addAction(choseWorkDir);

  auto editMenu = menu->addMenu(tr("&Edit"));
  QAction* cutAct = new QAction(tr("Cu&t"), this);
  cutAct->setShortcuts(QKeySequence::Cut);
  cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                          "clipboard"));
  CONNECT_CHECK(connect(cutAct, SIGNAL(triggered(bool)), m_pConstructorWidget, SLOT(onCutToBuffer())));
  editMenu->addAction(cutAct);

  QAction* copyAct = new QAction(tr("&Copy"), this);
  copyAct->setShortcuts(QKeySequence::Copy);
  copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                           "clipboard"));
  // connect(copyAct, &QAction::triggered, m_pConstructorWidget, &ConstructorWidget::copyToBuffer);
  CONNECT_CHECK(connect(copyAct, SIGNAL(triggered(bool)), m_pConstructorWidget, SLOT(onCopyToBuffer())));
  editMenu->addAction(copyAct);

  QAction* pasteAct = new QAction(tr("&Paste"), this);
  pasteAct->setShortcuts(QKeySequence::Paste);
  pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                            "selection"));
  // connect(pasteAct, &QAction::triggered, m_pConstructorWidget, &ConstructorWidget::createFromBuffer;
  CONNECT_CHECK(connect(pasteAct, SIGNAL(triggered(bool)), m_pConstructorWidget, SLOT(onCreateFromBuffer())));
  editMenu->addAction(pasteAct);

  menuBar()->addSeparator();

  // QToolBar* fileToolBar = addToolBar(tr("File"));
  auto wid = new QWidget{};
  auto pLayoutV = new QVBoxLayout{};
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

  wid->setLayout(pLayoutV);
  setCentralWidget(wid);
}

void MainWidget::createConnections() { // clang-format off
  CONNECT_CHECK(connect(m_pConstructorWidget,   SIGNAL(paramsChanged(std::shared_ptr<NNLayerParams>)),
                        m_pParamsEditorWidget,  SLOT(onSetParams(std::shared_ptr<NNLayerParams>))));
  CONNECT_CHECK(connect(m_pConstructorWidget,   SIGNAL(changeActiveLayer(HashSet)),
                        m_pParamsEditorWidget,  SLOT(onForward(HashSet))));
  CONNECT_CHECK(connect(m_pConstructorWidget,   SIGNAL(collectErrors(std::string)),
                        m_pParamsEditorWidget,  SLOT(onSetErrors(std::string))));
  CONNECT_CHECK(connect(m_pConstructorWidget,   SIGNAL(clearChanged()),
                        m_pParamsEditorWidget,  SLOT(onClearEditors())));
  CONNECT_CHECK(connect(m_pConstructorWidget,   SIGNAL(clearActiveLayer()),
                        m_pParamsEditorWidget,  SLOT(onClearForward())));
  CONNECT_CHECK(connect(m_pConstructorWidget,   SIGNAL(clearErrors()),
                        m_pParamsEditorWidget,  SLOT(onClearErrors())));



  CONNECT_CHECK(connect(m_pParamsEditorWidget,  SIGNAL(paramsChanged(std::shared_ptr<NNLayerParams>)),
                        m_pConstructorWidget,   SLOT(onSetParams(std::shared_ptr<NNLayerParams>))));
  CONNECT_CHECK(connect(m_pParamsEditorWidget,  SIGNAL(deleteActive()),
                        m_pConstructorWidget,   SLOT(onDeleteActive())));
  CONNECT_CHECK(connect(m_pParamsEditorWidget,  SIGNAL(deletedEdge(KeyType)),
                        m_pConstructorWidget,   SLOT(onDeleteEdge(KeyType))));

  CONNECT_CHECK(connect(m_pGlobalSettingsWidget,  SIGNAL(makeTrainXml(NNParamContainer)),
                                                  SLOT(onWriteTrainXml(NNParamContainer))));
  CONNECT_CHECK(connect(m_pGlobalSettingsWidget,  SIGNAL(makeNetXml()),
                        this,                     SLOT(onWriteNetXml())));
  CONNECT_CHECK(connect(m_pGlobalSettingsWidget,  SIGNAL(makeNetPy()),
                        this,                     SLOT(onWriteNetPy())));
  CONNECT_CHECK(connect(m_pGlobalSettingsWidget,  SIGNAL(setWorkDir(std::string)),
                        this,                     SLOT(onSetWorkDir(std::string))));

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

void MainWidget::writeXml(const std::string& strXmlBody) {
  setFileName();
  const auto filePath = QDir::toNativeSeparators(QString::fromStdString(m_strPath + "/" + m_strName + ".xml"));
  std::ofstream FStream{filePath.toStdString()};
  assert(FStream.is_open());

  FStream << "<?Stream version=\"1.0\"?>" << '\n';
  FStream << "<data>" << '\n';
  FStream << strXmlBody;
  FStream << "</data>" << std::endl;
}

void MainWidget::writePy(const std::string& strPyBody) {
  setFileName();
  const auto filePath = QDir::toNativeSeparators(QString::fromStdString(m_strPath + "/" + m_strName + ".py"));
  std::ofstream FStream{filePath.toStdString()};
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

void MainWidget::setFileName() { m_strName = m_pGlobalSettingsWidget->getName(); }

#pragma once

#include "ConstructorWidget.h"
#include "CreateNNWidget.h"
#include "GlobalSettingsWidget.h"
#include "GlobalWarningWidget.h"
#include "ParamsEditorWidget.h"
#include <QMainWindow>

class MainWidget : public QMainWindow {
  Q_OBJECT
  using KeyType = std::size_t;
  using NNParamContainer = std::vector<NNParam>;

public:
  MainWidget();

private slots:
  void onWriteTrainXml(const NNParamContainer& vParams);
  void onWriteNetXml();
  void onWriteNetPy();

  void onSave();
  void onLoad();

  void onSetWorkDir(const std::string& str);

private:
  void initGUI();
  void createConnections();

  void writeXml(const std::string& strXmlBody);
  void writePy(const std::string& strPyBody);

  void setFileName();

private:
  GlobalSettingsWidget* m_pGlobalSettingsWidget;
  ConstructorWidget* m_pConstructorWidget;
  ParamsEditorWidget* m_pParamsEditorWidget;
  CreateNNWidget* m_pCreateLayerWidget;
  GlobalWarningWidget* m_pGlobalWarningWidget;

  std::string m_strName{"untitled"};
  std::string m_strPath{"."};
};

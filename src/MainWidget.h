#pragma once

#include "GlobalSettingsWidget.h"
#include "ConstructorWidget.h"
#include "ParamsEditorWidget.h"

class MainWidget: public QWidget
{
  Q_OBJECT

public:
  MainWidget();

private slots:
  void onWriteNetXml(const std::vector<NNParam>& vParams) const;
  void onWriteNetPy(const std::vector<NNParam>& vParams) const;
  void onWriteTrainXml(const std::vector<NNParam>& vParams) const;

private:
  void initGUI();
  void createConnections();

  void writeXml(const std::string& strXmlPath, const std::string& strXmlBody) const;
  void writePy(const std::string &strPyPath, const std::string &strPyBody) const;

  GlobalSettingsWidget* m_pGlobalSettingsWidget;
  ConstructorWidget* m_pConstructorWidget;
  ParamsEditorWidget* m_pSettingsEdit;
};


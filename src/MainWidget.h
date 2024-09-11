#pragma once

#include "ConstructorWidget.h"
#include "CreateLayerWidget.h"
#include "GlobalSettingsWidget.h"
#include "GlobalWarningWidget.h"
#include "ParamsEditorWidget.h"

class MainWidget : public QWidget {
  Q_OBJECT
  using KeyType = std::size_t;
  using NNParamContainer = std::vector<NNParam>;

public:
  MainWidget();

private slots:
  void onWriteTrainXml(const NNParamContainer& vParams) const;
  void onWriteNetXml(const NNParamContainer& vParams) const;
  void onWriteNetPy(const NNParamContainer& vParams) const;
  void onCheckIsValid() const;

private:
  void initGUI();
  void createConnections();

  void writeXml(const std::string& strXmlPath, const std::string& strXmlBody) const;
  void writePy(const std::string& strPyPath, const std::string& strPyBody) const;

  GlobalSettingsWidget* m_pGlobalSettingsWidget;
  ConstructorWidget* m_pConstructorWidget;
  ParamsEditorWidget* m_pParamsEditorWidget;
  CreateLayerWidget* m_pCreateLayerWidget;
  GlobalWarningWidget* m_pGlobalWarningWidget;
};

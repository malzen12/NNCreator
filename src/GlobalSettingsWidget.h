#pragma once

#include "NNParam.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

#include "ParamWidget.h"
#include "qapplication.h"
#include <iostream>
#include <QFileDialog>

class GlobalSettingsWidget : public QWidget {
  Q_OBJECT
  using KeyType = std::size_t;
  using Vector = std::vector<KeyType>;
  using NNParamContainer = std::vector<NNParam>;

public:
  GlobalSettingsWidget();

  auto getName() const -> std::string;

signals:
  void inputSizeChanged(const Vector&);
  void makeTrainXml(const NNParamContainer&);
  void makeNetXml();
  void makeNetPy();
  void isValid();
  void notValid(const std::string&);

  void setFileName(std::string);
  void setWorkDir(std::string);

private slots:
  void onUpdateInputSize();
  void onMakeTrainXml();

  void onChoseWorkDir();

private:
  void initGUI();
  void createConnections();

  static auto collectParams(QGridLayout* pLayout) -> NNParamContainer;
  auto collectInputSize() -> NNParam;

private:
  QGridLayout* m_pFileLayout;
  QLabel* m_pWorkDirLabel;
  ParamWidget* m_pFileNameParamWidget;
  QPushButton* m_pChoseWorkDirButton;

  QGridLayout* m_pTrainSettingsLayout;
  QPushButton* m_pMakeTrainXmlButton;

  QGridLayout* m_pNetSettingsLayout;
  QPushButton* m_pConfirmSizeButton;

  ////////////////////////////////
};

#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>

#include "LayerParams/NNParam.h"


class GlobalSettingsWidget: public QWidget
{
  Q_OBJECT

public:
  GlobalSettingsWidget();

signals:
  void inputSizeChanged(const std::vector<std::size_t>& );
  void makeNetXml(const std::vector<NNParam>& );
  void makeNetPy(const std::vector<NNParam>& );
  void makeTrainXml(const std::vector<NNParam>& );
  void isValid();
  void notValid(const std::string&);

private slots:
  void onUpdateInputSize();
  void onMakeNetXml();
  void onMakeNetPy();
  void onMakeTrainXml();
private:
  void initGUI();
  void createConnections();

  std::vector<NNParam> collectParams(QGridLayout* pLayout);
  NNParam collectInputSize();

  QGridLayout* m_pTrainSettingsLayout;
  QPushButton* m_pMakeTrainXmlButton;

  QGridLayout* m_pNetSettingsLayout;
  QPushButton* m_pConfirmSizeButton;
  QLabel* m_pMakeNetLabel;
  QPushButton* m_pMakeNetXmlButton;
  QPushButton* m_pMakeNetPyButton;
  QPushButton* m_pIsValidButton;
};


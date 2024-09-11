#pragma once

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

#include "LayerParams/NNParam.h"

class GlobalSettingsWidget : public QWidget {
  Q_OBJECT
  using KeyType          = std::size_t;
  using Vector           = std::vector<KeyType>;
  using NNParamContainer = std::vector<NNParam>;

public:
  GlobalSettingsWidget();

signals:
  void inputSizeChanged(const Vector&);
  void makeNetXml(const NNParamContainer&);
  void makeNetPy(const NNParamContainer&);
  void makeTrainXml(const NNParamContainer&);
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

  auto collectParams(QGridLayout* pLayout) -> NNParamContainer;
  auto collectInputSize() -> NNParam;

private:
  QGridLayout* m_pTrainSettingsLayout;
  QPushButton* m_pMakeTrainXmlButton;

  QGridLayout* m_pNetSettingsLayout;
  QPushButton* m_pConfirmSizeButton;
  QLabel*      m_pMakeNetLabel;
  QPushButton* m_pMakeNetXmlButton;
  QPushButton* m_pMakeNetPyButton;
  QPushButton* m_pIsValidButton;
};

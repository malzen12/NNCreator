#pragma once

#include "GlobalSettingsWidget.h"
#include "ConstructorWidget.h"
#include "ParamsEditorWidget.h"



#include <QShortcut>

class GlobalWarningWidget: public QWidget
{
  Q_OBJECT
public:
  GlobalWarningWidget()
    : m_pWarningLabel{new QLabel},
      m_pScrollArea{new QScrollArea}
  {
    auto pLayout = new QGridLayout{this};

    m_pWarningLabel->setText("Warning area\n\n\n\n\n\n\nend!");
    m_pWarningLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_pScrollArea->setWidget(m_pWarningLabel);
    pLayout->addWidget(m_pScrollArea);
    pLayout->setMargin(0);




  };
private slots:
  void clearWarnings(){
    m_pWarningLabel->clear();
    width = 0;
    height = 0;
    qstrWarning.clear();
  }

  void collecteWarning(const std::string& warning){
    QString temp{QString::fromStdString(warning)};
    if(!qstrWarning.isEmpty())
      qstrWarning += "\n";
    qstrWarning += temp;
    width = std::max(width, fontMetrics().horizontalAdvance(temp));
    height += fontMetrics().height();

  }

  void printWarnings(){
    if(qstrWarning.isEmpty()){
      m_pWarningLabel->setText("OK");
      m_pWarningLabel->setStyleSheet("QLabel {background : white; color : green; }");
      width = fontMetrics().horizontalAdvance("OK");
      height = fontMetrics().height();
    }else{
      m_pWarningLabel->setText(qstrWarning);
      m_pWarningLabel->setStyleSheet("QLabel {background : white; color : red; }");
      m_pWarningLabel->resize(width, height);
    }
  }

  void resizeEvent(QResizeEvent*) final{
    m_pWarningLabel->setMinimumSize(m_pWarningLabel->parentWidget()->size());
    m_pWarningLabel->resize(width, height);
  }
  void showEvent(QShowEvent*) final
  {
    resizeEvent(nullptr);
  }


private:
  QLabel* m_pWarningLabel;
  QScrollArea* m_pScrollArea;
  QString qstrWarning;
  int width{0};
  int height{0};
};


class MainWidget: public QWidget
{
  Q_OBJECT

public:
  MainWidget();

private slots:
  void onWriteNetXml(const std::vector<NNParam>& vParams) const;
  void onWriteNetPy(const std::vector<NNParam>& vParams) const;
  void onWriteTrainXml(const std::vector<NNParam>& vParams) const;
  void onCheckIsValid() const;
private:
  void initGUI();
  void createConnections();

  void writeXml(const std::string& strXmlPath, const std::string& strXmlBody) const;
  void writePy(const std::string &strPyPath, const std::string &strPyBody) const;

  GlobalSettingsWidget* m_pGlobalSettingsWidget;
  ConstructorWidget* m_pConstructorWidget;
  ParamsEditorWidget* m_pParamsEditorWidget;
  GlobalWarningWidget* m_pGlobalWarningWidget;
};


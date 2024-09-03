#pragma once

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QGridLayout>

class GlobalWarningWidget: public QWidget
{
  Q_OBJECT
public:
  GlobalWarningWidget();;
private slots:
  void clearWarnings();
  void collecteWarning(const std::string& warning);
  void printWarnings();

  void resizeEvent(QResizeEvent*) final;
  void showEvent(QShowEvent*) final;


private:
  QLabel* m_pWarningLabel;
  QScrollArea* m_pScrollArea;
  QString qstrWarning;
  int width{0};
  int height{0};
};



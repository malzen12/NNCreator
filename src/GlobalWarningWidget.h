#pragma once

#include <QLabel>
#include <QScrollArea>
#include <QWidget>

class GlobalWarningWidget : public QWidget {
  Q_OBJECT
public:
  GlobalWarningWidget();
private:
  void resizeEvent(QResizeEvent *) final;
  void showEvent(QShowEvent *) final;
private slots:
  void clearWarnings();
  void collecteWarning(const std::string &warning);
  void printWarnings();

private:
  QLabel *m_pWarningLabel;
  QScrollArea *m_pScrollArea;
  QString qstrWarning;
  int width{0};
  int height{0};
};

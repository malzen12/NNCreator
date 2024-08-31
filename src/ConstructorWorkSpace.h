#pragma once

#include <QWidget>
#include <QMenu>

#include "NNLayerWidget.h"

class ConstructorWorkSpace: public QWidget{
  Q_OBJECT
public:
  ConstructorWorkSpace(const std::unordered_map<std::size_t, NNLayerWidget*>& vLayers, QMenu* pMenu);

  void add(const QPoint& crPoint, NNLayerWidget* pLayer);
  QSize getQSize(){
    return {width, height};
  }
  void setWidth(int value);
  void setHeight(int value);
private:
  void initGUI();
  void mousePressEvent(QMouseEvent* pEvent) final;
  void paintEvent(QPaintEvent* pEvent) final;
//  void mouseMoveEvent(QMouseEvent* pEvent) final;
  void resizeEvent(QResizeEvent*) final;

private:
  const std::unordered_map<std::size_t, NNLayerWidget*>& m_refvLayers;
  QMenu* m_pMenu;
  int width{0};
  int height{0};
};

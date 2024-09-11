#pragma once

#include "ConstructorWidgetHelper.h"
#include "NNLayerWidget.h"
#include <unordered_set>
#include <QWidget>

class ConstructorWorkSpace : public QWidget {
  Q_OBJECT
  using KeyType          = std::size_t;
  using HashSet          = std::unordered_set<KeyType>;
  using NNLayerContainer = std::unordered_map<KeyType, NNLayerWidget*>;

public:
  ConstructorWorkSpace(const NNLayerContainer& vLayers, const GraphManager& m_refGraph,
                       const HashSet& ActiveSet);

  void add(const QPoint& crPoint, NNLayerWidget* pLayer);

  auto getQSize() const -> QSize;

  bool isMoved();
  void setMoved();
public slots:
  void onSendIsMoved(KeyType index);
  void onSetIsMoved(bool value);

private:
  void initGUI();
  void paintEvent(QPaintEvent* pEvent) final;
  void mousePressEvent(QMouseEvent* pEvent) final;
  void mouseMoveEvent(QMouseEvent* pEvent) final;
  void mouseReleaseEvent(QMouseEvent* pEvent) final;
  void resizeEvent(QResizeEvent*) final;

  void drag(const QPoint eventPos);
  auto dragChoose() const -> NNLayerWidget*;

  auto dragCheckBorders(const int k_margin, NNLayerWidget* pLayer, const QPoint eventPos) -> QPoint;
  void dragMove(const QPoint shift);
  void dragResize(const int k_margin);
private slots:
  void onSetGrabbed(KeyType index, bool value);
  void dragInit();

private:
  const NNLayerContainer& m_refvLayers;
  const GraphManager& m_refGraph;
  const HashSet& m_refActiveSet;

  bool m_isMoved;
  KeyType m_GrabbedIndex;
  QPoint m_activeDragAreaPos;
  QSize m_activeDragAreaSize;

  int width{0};
  int height{0};
};

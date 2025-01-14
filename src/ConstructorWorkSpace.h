#pragma once

#include "ConstructorWidgetHelper.h"
#include "NNLayerWidget.h"
#include <unordered_set>
#include <QWidget>

class ConstructorWorkSpace : public QWidget {
  Q_OBJECT
  using KeyType = std::size_t;
  using HashSet = std::unordered_set<KeyType>;
  using NNLayerContainer = std::unordered_map<KeyType, NNWidget*>;

public:
  ConstructorWorkSpace(const NNLayerContainer& vLayers, const GraphManager& m_refGraph,
                       const HashSet& ActiveSet);

  void add(const QPoint& crPoint, NNWidget* pLayer);

  auto getQSize() const -> QSize;

  bool isMoved() const;
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
  void dragInit();
  auto dragChoose() const -> NNWidget*;
  auto dragCheckBorders(const int k_margin, const QPoint& layerPos, const QPoint eventPos) -> QPoint;
  void dragMove(const QPoint shift);
  void dragResize(const int k_margin);
private slots:
  void onGrabbed(KeyType index, QPoint pos);
  void onUngrabbed();

private:
  const NNLayerContainer& m_refvLayers;
  const GraphManager& m_refGraph;
  const HashSet& m_refActiveSet;

  bool m_isMoved;
  KeyType m_GrabbedIndex{SIZE_MAX};
  QPoint m_GrabbedPos;
  QPoint m_activeDragAreaPos;
  QSize m_activeDragAreaSize;

  int width{0};
  int height{0};
};

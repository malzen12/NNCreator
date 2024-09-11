#pragma once

#include "ConstructorWidgetHelper.h"
#include "ConstructorWorkSpace.h"
#include "FabricLayer.h"
#include "NNLayerWidget.h"
#include <QGridLayout>
#include <QScrollArea>
#include <QWidget>

class ConstructorWidget : public QWidget {
  Q_OBJECT

  using bfs_proc = std::function<void(NNLayerWidget*, NNLayerWidget*)>;
  using KeyType = std::size_t;
  using Set = std::set<KeyType>;
  using Vector = std::vector<KeyType>;
  using HashSet = std::unordered_set<KeyType>;
  using HashMapOfHashSet = std::unordered_map<KeyType, HashSet>;
  using HashMap = std::unordered_map<KeyType, KeyType>;
  using Queue = std::queue<KeyType>;
  using NNLayerContainer = std::unordered_map<KeyType, NNLayerWidget*>;
  using NNLayerHashSet = std::unordered_set<NNLayerWidget*>;

public:
  ConstructorWidget();

  auto makeXmlString() const -> std::string;
  auto makePyString() -> std::string;
  bool isEmpty() const noexcept;

  void updateParamWidget();

signals:
  void paramsChanged(const std::shared_ptr<NNLayerParams>&);
  void changeActiveLayer(HashSet);
  void clearActiveLayer();
  void clearChanged();
  void notValid(const std::string&);
  void startCalculation();
  void compliteCalculation();
  void treeWidgetItem(const std::vector<QString>&);
  void createCopy(const NNLayerWidget&, const QPoint&);

public slots:
  void onSetParams(const std::shared_ptr<NNLayerParams>& spParams);
  void onDeleteActive();
  void onDeleteEdge(KeyType sId);

  void onSetInputSize(const Vector& vInputSize);

private slots:
  void onAddLayer(const QPoint& crPoint, NNLayerWidget* pLayer);
  void onChangeActive(KeyType sId);
  void onMakeForward(KeyType sId);
  void onAddToActive(KeyType sId);
  void onDelFromActive(KeyType sId);
  void onActiveAll();

  void onCreateWidget(NNLayerWidget* pLayer);
  void onCreateWidget(NNLayerWidget* pLayer, QPoint point);

private:
  void initGUI();
  void createConnections();

  void makeActive(KeyType sId, bool bActive);
  void clearActive();

  void copyToBuffer();
  void createFromBuffer();

  void checkSizes();
  void resizeEvent(QResizeEvent*) final;
  void showEvent(QShowEvent*) final;
  void mousePressEvent(QMouseEvent* pEvent) final;
  void mouseMoveEvent(QMouseEvent* pEvent) final;
  void mouseReleaseEvent(QMouseEvent* pEvent) final;

  void bfs(KeyType iStart) const;

private:
  NNLayerContainer m_layers;
  HashSet m_activeSet;
  Vector m_vInputSize;
  using VectorLayerAndPos = std::pair<std::vector<NNLayerWidget>, std::vector<QPoint>>;
  VectorLayerAndPos m_layersBuffer;

  ConstructorWorkSpace* m_pWorkSpace;

  GraphManager m_graph{m_layers};
  FabricLayer* m_pFabricLayer;
};

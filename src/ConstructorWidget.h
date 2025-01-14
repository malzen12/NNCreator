#pragma once

#include "ConstructorWidgetHelper.h"
#include "ConstructorWorkSpace.h"
#include "FabricManager.h"
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
  using NNContainer = std::unordered_map<KeyType, NNWidget*>;
  using NNHashSet = std::unordered_set<NNWidget*>;
  using VectorNNAndPos = std::pair<std::vector<NNWidget*>, std::vector<QPoint>>;

public:
  ConstructorWidget();

  auto makeXmlString() const -> std::string;
  auto makePyString() -> std::string;

  auto save() const -> std::string;
  void load(std::stringstream& stream);

  bool isEmpty() const noexcept;

signals:
  void paramsChanged(const std::shared_ptr<NNLayerParams>&);
  void changeActiveLayer(HashSet);
  void collectErrors(const std::string&);
  void clearActiveLayer();
  void clearChanged();
  void clearErrors();
  void notValid(const std::string&);
  void startCalculation();
  void compliteCalculation();
  void treeWidgetItem(const std::vector<QString>&);
  void createCopy(const NNLayerWidget&, const QPoint&);

public slots:
  void onSetParams(const std::shared_ptr<NNLayerParams>& spParams);
  void onDeleteActive();
  void onDeleteEdge(KeyType id);

  void onSetInputSize(const Vector& vInputSize);

private slots:
  void onAddLayer(const QPoint& crPoint, NNWidget* pLayer);
  void onChangeActive(KeyType id);
  void onMakeForward(KeyType id);
  void onAddToActive(KeyType id);
  void onDelFromActive(KeyType id);
  void onActiveAll();

  void onCreateWidget(NNWidget* pLayer);
  void onCreateWidget(NNWidget* pLayer, QPoint point);

  void onCutToBuffer();
  void onCopyToBuffer();
  void onCreateFromBuffer();

private:
  void initGUI();
  void createConnections();
  void updateParamWidget();

  void makeActive(KeyType id, bool bActive);
  void clearActive();
  void checkSizes();
  void updateThis();

  void clearSignals();
  void nnConnect(NNWidget* pLayer);

  void resizeEvent(QResizeEvent*) final;
  void showEvent(QShowEvent*) final;
  void mousePressEvent(QMouseEvent* pEvent) final;
  void mouseMoveEvent(QMouseEvent* pEvent) final;
  void mouseReleaseEvent(QMouseEvent* pEvent) final;

  void bfs(KeyType iStart) const;

private:
  Vector m_vInputSize;
  NNContainer m_layers;

  HashSet m_activeSet;
  VectorNNAndPos m_layersBuffer;

  ConstructorWorkSpace* m_pWorkSpace;

  GraphManager m_graph{m_layers};
  FabricManager m_fabricManager;
};

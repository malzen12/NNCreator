#pragma once

#include <QWidget>
#include <QScrollArea>
#include <QMenu>
#include <QGridLayout>



#include "NNLayerWidget.h"
#include "ConstructorWorkSpace.h"


#include "ConstructorWidgetHelper.h"



class ConstructorWidget: public QWidget
{
  Q_OBJECT

  using bfs_proc = std::function<void(NNLayerWidget*, NNLayerWidget*)>;

public:
  ConstructorWidget();

  std::string makeXmlString() const;
  std::string makePyString();
  bool isEmpty() const noexcept;

signals:
  void paramsChanged(const std::shared_ptr<NNLayerParams>&);
  void notValid(const std::string&);
  void startCalculation();
  void compliteCalculation();
  void changeActiveLayer(NNLayerWidget*);

public slots:
  void onSetParams(const std::shared_ptr<NNLayerParams>& spParams);
  void onDeleteActive();
  void onDeleteEdge(std::size_t sId);

  void onSetInputSize(const std::vector<std::size_t>& vInputSize);


private slots:
  void onAddLayer(const QPoint& crPoint, const std::shared_ptr<NNLayerParams>& spParams);
  void onProcActions(QAction* /*pAction*/);
  void onChangeActive(std::size_t sId);
  void onMakeForward(std::size_t sId);
  void onAddToActive(std::size_t sId);
  void onDelFromActive(std::size_t sId);
  void onActiveAll();

private:
  void initGUI();
  void createConnections();

  void makeActive(std::size_t sId, bool bActive);

  void checkSizes();
  void resizeEvent(QResizeEvent*) final;
  void showEvent(QShowEvent*) final;
  void mouseReleaseEvent(QMouseEvent*) final;
  void mouseMoveEvent(QMouseEvent* pEvent) final;

  void bfs(NNLayerWidget* pStart, bfs_proc fCurrentProc, bfs_proc fForwardsProc) const;

private:

  ConstructorWorkSpace* m_pWorkSpace;

  QMenu* m_pMenu;
  std::unordered_map<std::size_t, NNLayerWidget*> m_vLayers;
  GraphManager graph{m_vLayers};

  std::unordered_set<std::size_t> m_ActiveSet;

  std::vector<std::size_t> m_vInputSize;


};


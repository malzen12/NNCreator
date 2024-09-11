#pragma once

#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>

#include <unordered_set>

#include "NNLayerParams.h"

class Edge : public QHBoxLayout {
  Q_OBJECT
  using KeyType = std::size_t;

public:
  Edge(KeyType sId);

  void createConnections();

signals:
  void deletedEdge(KeyType);

private:
  QToolButton* button;
  QLabel*      label;
  KeyType      sId;
};

class ParamsEditorWidget : public QWidget {
  Q_OBJECT
  using KeyType = std::size_t;
  using HashSet = std::unordered_set<KeyType>;

public:
  ParamsEditorWidget();
signals:
  void paramsChanged(const std::shared_ptr<NNLayerParams>& crParams);
  void deleteActive();
  void deletedEdge(KeyType);
public slots:
  void onSetParams(const std::shared_ptr<NNLayerParams>& spParams);
  void onForward(const HashSet& forward);

private:
  void initGUI();
  void createConnections();

  void initForward(const HashSet& forward);
  void initEditors();

  auto collectParams() -> const std::shared_ptr<NNLayerParams>&;
private slots:
  void onConfirmParams();
  void onClearForward();
  void onClearEditors();

private:
  QLabel* m_pNameLabel;

  QVBoxLayout* m_pForwardLayout;
  QVBoxLayout* m_pParamsLayout;

  QPushButton* m_pConfirm;
  QPushButton* m_pDelete;

  std::shared_ptr<NNLayerParams> m_spParams;
};

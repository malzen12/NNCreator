#pragma once

#include "NNLayerParams.h"
#include <unordered_set>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>

class Edge : public QHBoxLayout {
  Q_OBJECT
  using KeyType = std::size_t;

public:
  explicit Edge(KeyType id);

  void createConnections();

signals:
  void deletedEdge(KeyType);

private:
  QToolButton* m_pButton;
  QLabel* m_pLabel;
  KeyType m_id;
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
  void onSetErrors(const std::string& strErrors);

  void onConfirmParams();
  void onClearForward();
  void onClearEditors();
  void onClearErrors();

private:
  void initGUI();
  void createConnections();

  void initForward(const HashSet& forward);
  void initEditors();

  auto collectParams() -> const std::shared_ptr<NNLayerParams>&;
private slots:

private:
  QLabel* m_pNameLabel;

  QVBoxLayout* m_pForwardLayout;
  QVBoxLayout* m_pParamsLayout;

  QPushButton* m_pConfirm;
  QPushButton* m_pDelete;

  QLabel* m_pErrors;

  std::shared_ptr<NNLayerParams> m_spParams;
};

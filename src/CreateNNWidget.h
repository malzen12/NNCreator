#pragma once

#include <QTreeWidget>
#include <QWidget>

#include <QFile>

class CreateNNWidget : public QTreeWidget {
  Q_OBJECT
public:
  CreateNNWidget();

signals:
  void treeWidgetItem(const std::vector<QString>&);
private slots:
  void createItem(QTreeWidgetItem* item, int column);

private:
  void initWidget();
  bool isOnFirst(int position);
  bool isOnSameLevel(int position, int predPosition);
  bool isOnNextLevel(int position, int predPosition);

  void ifOnFirst(QAbstractItemModel* model, QModelIndex& index, int& predPosition);
  void ifOnSameLevel(QAbstractItemModel* model, QModelIndex& index);
  void ifOnNextLevel(QAbstractItemModel* model, QModelIndex& index, int& predPosition);
  void ifOnAboveLevel(QAbstractItemModel* model, QModelIndex& index, int position, int& predPosition);
  void initGui();
  void createConnections();

private:
};


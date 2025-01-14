#include "CreateNNWidget.h"
#include "Macros.h"
#include <iostream>
#include <QBoxLayout>
#include <QFile>
#include <QLabel>
#include <QScrollArea>
#include <QStandardItemModel>

CreateNNWidget::CreateNNWidget() {
  initWidget();
  initGui();
  createConnections();
}

void CreateNNWidget::createItem(QTreeWidgetItem* item, int column) {
  if (item->childCount() == 0) {
    std::vector<QString> tags{};
    auto parent = item;
    while (parent) {
      tags.push_back(parent->data(column, Qt::DisplayRole).toString());
      parent = parent->parent();
    }
    std::reverse(tags.begin(), tags.end());
    for (const auto& str : tags) std::cout << str.toStdString() << ' ';
    emit treeWidgetItem(tags);
    std::cout << std::endl;
  }
}

void CreateNNWidget::initWidget() {
  QFile file("CreateLayerWidget.txt");
  file.open(QIODevice::ReadOnly);
  const auto list = QString{file.readAll()}.split("\n");
  auto model = this->model();
  auto index = model->index(model->rowCount(), 0);
  int position{};
  int predPosition{};
  QString strStriped{};

  for (const auto& str : list) {
    position = str.count('\t');
    strStriped = str.simplified();
    if (isOnFirst(position))
      ifOnFirst(model, index, predPosition);
    else if (isOnSameLevel(position, predPosition))
      ifOnSameLevel(model, index);
    else if (isOnNextLevel(position, predPosition))
      ifOnNextLevel(model, index, predPosition);
    else
      ifOnAboveLevel(model, index, position, predPosition);
    model->setData(index, strStriped);
  }
}

bool CreateNNWidget::isOnFirst(int position) { return position == 0; }

bool CreateNNWidget::isOnSameLevel(int position, int predPosition) { return position == predPosition; }

bool CreateNNWidget::isOnNextLevel(int position, int predPosition) { return position > predPosition; }

void CreateNNWidget::ifOnFirst(QAbstractItemModel* model, QModelIndex& index, int& predPosition) {
  const auto row = model->rowCount();
  model->insertRow(row);
  index = model->index(row, 0);
  predPosition = 0;
}

void CreateNNWidget::ifOnSameLevel(QAbstractItemModel* model, QModelIndex& index) {
  const auto parent = model->parent(index);
  const auto row = model->rowCount(parent);
  model->insertRow(row, parent);
  index = model->index(row, 0, parent);
}

void CreateNNWidget::ifOnNextLevel(QAbstractItemModel* model, QModelIndex& index, int& predPosition) {
  const auto row = model->rowCount(index);
  model->insertRow(row, index);
  index = model->index(0, 0, index);
  ++predPosition;
}

void CreateNNWidget::ifOnAboveLevel(QAbstractItemModel* model, QModelIndex& index, int position,
                                       int& predPosition) {
  while (! isOnSameLevel(position, predPosition)) {
    --predPosition;
    index = model->parent(index);
  }
  const auto parent = model->parent(index);
  const auto row = model->rowCount(parent);
  model->insertRow(row, parent);
  index = model->index(row, 0, parent);
}

void CreateNNWidget::initGui() { setHeaderHidden(true); }

void CreateNNWidget::createConnections() {
  // clang-format off
  CONNECT_CHECK(connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
                              SLOT(createItem(QTreeWidgetItem*, int))));
  // clang-format on
}

/*
Non-linear Activations (weighted sum, nonlinearity)

Non-linear Activations (other)

Distance Functions

Loss Functions

Utilities

Quantized Functions

*/

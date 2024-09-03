#include "CreateLayerWidget.h"

#include <QLabel>
#include <QScrollArea>
#include <QBoxLayout>


#include <QStandardItemModel>
#include <iostream>
#include <QFile>

CreateLayerWidget::CreateLayerWidget() {
  initWidget();
  initGui();
  createConnections();
}

void CreateLayerWidget::createItem(QTreeWidgetItem *item, int column) {
  auto modif = QGuiApplication::keyboardModifiers();
  std::vector<QString> tags{};
  if (item->childCount() == 0) {
    auto parent = item;
    while (parent != nullptr) {
      tags.push_back(parent->data(column, Qt::DisplayRole).toString());
      parent = parent->parent();
    }
  }
  std::reverse(tags.begin(), tags.end());
  for(const auto& str : tags){
    std::cout << str.toStdString() << ' ';
  }
  std::cout << std::endl;
}


void CreateLayerWidget::initWidget(){
  QFile file("CreateLayerWidget.txt");
  file.open(QIODevice::ReadOnly);
  auto strFile = QString{file.readAll()};
  auto list = strFile.split("\n");
  auto model = this->model();
  auto index = model->index(model->rowCount(), 0);
  int position{};
  int predPosition{};
  QString strStriped{};

  for (const auto &str : list) {
    position = str.count('\t');
    strStriped = str.simplified();
    if (position == 0) {
      model->insertRow(model->rowCount());
      index = model->index(model->rowCount() - 1, 0);
      predPosition = 0;
    } else if (position == predPosition) {
      model->insertRow(model->rowCount(model->parent(index)),
                       model->parent(index));
      index = model->index(model->rowCount(model->parent(index)) - 1, 0,
                           model->parent(index));
    } else if (position > predPosition) {
      model->insertRow(model->rowCount(index), index);
      index = model->index(0, 0, index);
      ++predPosition;
    } else {
      while (position != predPosition) {
        --predPosition;
        index = model->parent(index);
      }
      model->insertRow(model->rowCount(model->parent(index)),
                       model->parent(index));
      index = model->index(model->rowCount(model->parent(index)) - 1, 0,
                           model->parent(index));
    }
    model->setData(index, strStriped);
  }
}

void CreateLayerWidget::initGui() { setHeaderHidden(true); }

void CreateLayerWidget::createConnections() {
  connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
          SLOT(createItem(QTreeWidgetItem *, int)));
}
/*
Non-linear Activations (weighted sum, nonlinearity)

Non-linear Activations (other)

Distance Functions

Loss Functions

Utilities

Quantized Functions

*/


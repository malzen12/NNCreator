#pragma once


#include <QWidget>
#include <QTreeWidget>

#include "qapplication.h"

#include <iostream>

class CreateLayerWidget : public QTreeWidget
{
  Q_OBJECT
public:
  CreateLayerWidget();

signals:
  void treeWidgetItem(const std::vector<QString>&);
private slots:
  void createItem(QTreeWidgetItem* item, int column);;
private:
  void initWidget();
  void initGui();
  void createConnections();
private:

};


class GroupOfLayers : public QWidget
{
  Q_OBJECT

private:
  std::string m_strName;
//  std::vector<>
};



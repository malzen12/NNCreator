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
private slots:
  void createItem(QTreeWidgetItem* item, int column);;
private:
  void initWidget();
  void initGui();
  void createConnections();
private:
  int listIndex{-1};
};


class GroupOfLayers : public QWidget
{
  Q_OBJECT

private:
  std::string m_strName;
//  std::vector<>
};



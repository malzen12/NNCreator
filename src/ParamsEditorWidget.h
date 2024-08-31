#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

#include "NNLayerWidget.h"
#include <QToolButton>
#include <iostream>

class Edge : public QHBoxLayout
{
  Q_OBJECT
public:
  Edge(std::size_t sId)
    : /*layout{new QHBoxLayout},*/
      button{new QToolButton},
      label{new QLabel{QString::number(sId)}},
      sId{sId}
  {
//    layout->addWidget(button);
//    layout->addWidget(label);
    this->addWidget(button);
    this->addWidget(label);
    createConnections();
  }
  void createConnections(){
    auto bRes = true;
    auto func = [&](){
      emit deletedEdge(sId);
      button->close();
      label->close();
//      delete layout;
      delete this;

    };
    bRes &= static_cast<bool>(connect(button, &QToolButton::clicked, func));
    assert(bRes);
  };
  QHBoxLayout* getLayout(){
//    return layout;
    return this;
  }
signals:
  void deletedEdge(std::size_t);
private:
//  QHBoxLayout* layout;
  QToolButton* button;
  QLabel* label;
  std::size_t sId;
};

class ParamsEditorWidget: public QWidget
{
  Q_OBJECT

public:
  ParamsEditorWidget();

public slots:
  void onSetParams(const std::shared_ptr<NNLayerParams>& spParams);

  void onForward(NNLayerWidget* pLayer);
signals:
  void paramsChanged(const std::shared_ptr<NNLayerParams>& crParams);
  void deleteActive();
  void deletedEdge(std::size_t);

private slots:
  void onConfirmParams();

private:
  void initGUI();
  void createConnections();

  void initForward(const NNLayerWidget* pLayer);
  void initEditors();
  const std::shared_ptr<NNLayerParams>& collectParams();


  QLabel* m_pNameLabel;

  QVBoxLayout* m_pForwardLayout;

  QVBoxLayout* m_pParamsLayout;

  QPushButton* m_pConfirm;
  QPushButton* m_pDelete;

  std::shared_ptr<NNLayerParams> m_spParams;
};

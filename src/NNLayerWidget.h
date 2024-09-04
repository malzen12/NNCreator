#pragma once

#include <QLabel>
#include <iostream>


#include "LayerParams/BilinearLayerParams.h"
#include "LayerParams/NNLayerParams.h"
#include "LayerParams/Conv1dLayerParams.h"
#include "LayerParams/Conv2dLayerParams.h"
#include "LinearLayerParams.h"
#include "qapplication.h"


class NNLayerWidget: public QWidget
{
  Q_OBJECT

public:
  explicit NNLayerWidget(std::size_t sId, const std::shared_ptr<NNLayerParams>& spParams);

  std::size_t getId() const;
  static std::size_t createId(){
    static std::size_t m_classId = 0;
    return m_classId++;
  }

  void setParams(const std::shared_ptr<NNLayerParams>& spParams) noexcept;
  const std::shared_ptr<NNLayerParams>& getParams() const noexcept;
  const std::vector<std::vector<std::size_t>>& getInputSize() const;

  void deleteLayer();

  bool isGrabbed() const;
  bool isActive() const;

  void mousePressEvent(QMouseEvent* pEvent) final;
  void mouseReleaseEvent(QMouseEvent* pEvent) final;

  void makeActive(bool bActive);

  const QPoint& getGrabbedPos() const noexcept;

  void addForward(NNLayerWidget* pForward);
  void removeForward(NNLayerWidget* pForward);
  const std::vector<NNLayerWidget*>& getForward() const noexcept;

  void resetInputSize();
  void addInputSize(const std::vector<std::size_t>& vInputSize);
  std::vector<std::size_t> calcOutputSize() const;

  void toActive();
signals:
  void becomeActive(std::size_t);
  void makeForward(std::size_t);
  void addToActive(std::size_t);
  void delFromActive(std::size_t);

private:
  void initGUI();
  void updateStyle();


  std::size_t m_sId;
  bool m_bGrabbed;
  QPoint m_GrabbedPos;

  bool m_bActive;

  std::shared_ptr<NNLayerParams> m_spParams;

  std::vector<std::vector<std::size_t>> m_vInputSizes;
  bool m_bValidParams;

  std::vector<NNLayerWidget*> m_vForwards;

  QLabel* m_pInputLabel;
  QLabel* m_pNameLabel;
  QLabel* m_pOutputLabel;

};

class FabricLayer : public QObject
{
  Q_OBJECT
public:
  FabricLayer(){
    m_FactorMap["Linear"] = &FabricLayer::createLinear;
    m_FactorMap["Convolution"] = &FabricLayer::createConvolution;
  }
public:
signals:
  void changeWidget(NNLayerWidget*);
  void createAndConnectWidget(NNLayerWidget*);
  void createWidget(NNLayerWidget*);
public slots:
  void create(const std::vector<QString>& tags){
    auto pWidget = createImpl(tags);
    if (pWidget == nullptr)
      return;
    auto modif = QGuiApplication::keyboardModifiers();
    switch(modif){
      case Qt::ShiftModifier:
        emit changeWidget(pWidget);
        break;
      case Qt::AltModifier:
        emit createAndConnectWidget(pWidget);
        break;
      case Qt::NoModifier:
      default:
        emit createWidget(pWidget);
    }
  };
private:
  NNLayerWidget* createImpl(const std::vector<QString>& tags){
    std::size_t index = 0;
    while(index < tags.size() && tags[index] != m_strType){
      ++index;
    }
    ++index;
    if(index == tags.size()){
      std::cerr << "You don't have " << m_strType.toStdString() << " in txt file" << std::endl;
      return nullptr;
    }
    auto type = tags[index];
    QString layer{};
    for(++index; index < tags.size(); ++index){
      layer += tags[index];
    }

//    if(type == "Linear"){
//      createLinear(layer);

//    }


    if(m_FactorMap.find(type) != m_FactorMap.cend()){
      return m_FactorMap[type](layer);
    }else{
      std::cerr << "You don't have such type of " << type.toStdString() << " in " << m_strType.toStdString()  << std::endl;
      return nullptr;
    }

  };
  static NNLayerWidget* createLinear(const QString& layer){
    if (layer == "Linear")
      return new NNLayerWidget{NNLayerWidget::createId(), std::make_shared<LinearLayerParams>()};
    else if(layer == "Bilinear")
      return new NNLayerWidget{NNLayerWidget::createId(), std::make_shared<BilinearLayerParams>()};
    else{
      std::cerr << "NE" << std::endl;
      return nullptr;
    }
  }

  static NNLayerWidget* createConvolution(const QString& layer){
    if (layer == "Conv1d")
      return new NNLayerWidget{NNLayerWidget::createId(), std::make_shared<Conv1dLayerParams>()};
    else if(layer == "Conv2d")
      return new NNLayerWidget{NNLayerWidget::createId(), std::make_shared<Conv2dLayerParams>()};
    else{
      std::cerr << "NE" << std::endl;
      return nullptr;
    }
  }

private:
  std::unordered_map<QString, decltype(&FabricLayer::createLinear)> m_FactorMap;

  QString m_strType{"Layer"};
};


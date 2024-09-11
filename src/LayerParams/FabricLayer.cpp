#include "FabricLayer.h"
#include "BilinearLayerParams.h"
#include "Conv1dLayerParams.h"
#include "Conv2dLayerParams.h"
#include "LinearLayerParams.h"
#include <iostream>

FabricLayer::FabricLayer() {
  m_FactorMap["Linear"] = &FabricLayer::createLinear;
  m_FactorMap["Convolution"] = &FabricLayer::createConvolution;
}

void FabricLayer::onCreate(const std::vector<QString>& tags) {
  auto pWidget = createImpl(tags);
  if (! pWidget) return;
  auto modif = QGuiApplication::keyboardModifiers();
  switch (modif) {
    case Qt::ShiftModifier : emit changeWidget(pWidget); break;
    case Qt::AltModifier   : emit createAndConnectWidget(pWidget); break;
    case Qt::NoModifier    :
    default                : emit createWidget(pWidget);
  }
}

auto FabricLayer::onCreateCopy(const NNLayerWidget& layer, const QPoint& point) -> FabricLayer::KeyType {
  const auto id = NNLayerWidget::createId();
  auto const pLayerCopy = new NNLayerWidget{id, layer.getParams()->copy()};
  emit createWidget(pLayerCopy, point);
  return id;
}

auto FabricLayer::createImpl(const std::vector<QString>& tags) -> NNLayerWidget* {
  const auto [type, layer] = Impl(tags);
  if (type.isEmpty() || layer.isEmpty()) return nullptr;
  if (m_FactorMap.find(type) != m_FactorMap.cend()) return m_FactorMap[type](layer);
  std::cerr << "You don't have such type of " << type.toStdString() << " in " << m_strType.toStdString()
            << std::endl;
  return nullptr;
}

auto FabricLayer::Impl(const std::vector<QString>& tags) const -> std::pair<QString, QString> {
  KeyType index = 0;
  while (index < tags.size() && tags[index] != m_strType) ++index;
  ++index;
  if (index == tags.size()) {
    std::cerr << "You don't have " << m_strType.toStdString() << " in txt file" << std::endl;
    return {{}, {}};
  };
  QString type = tags[index];
  QString layer{};
  for (++index; index < tags.size(); ++index) layer += tags[index];
  return {type, layer};
}

auto FabricLayer::createLinear(const QString& layer) -> NNLayerWidget* {
  static std::unordered_map<QString, int> liner{
      {"Linear",   0},
      {"Bilinear", 1}
  };
  if (liner.find(layer) == liner.cend()) {
    std::cerr << "NE" << std::endl;
    return nullptr;
  }
  switch (liner.at(layer)) {
    case 0 :
      return new NNLayerWidget{NNLayerWidget::createId(), std::make_shared<LinearLayerParams>()};
      break;
    case 1 :
      return new NNLayerWidget{NNLayerWidget::createId(), std::make_shared<BilinearLayerParams>()};
      break;
    default : std::cerr << "NE" << std::endl; return nullptr;
  }
}

auto FabricLayer::createConvolution(const QString& layer) -> NNLayerWidget* {
  static std::unordered_map<QString, int> convolution{
      {"Conv1d", 0},
      {"Conv2d", 1}
  };
  if (convolution.find(layer) == convolution.cend()) {
    std::cerr << "NE" << std::endl;
    return nullptr;
  }

  switch (convolution.at(layer)) {
    case 0 :
      return new NNLayerWidget{NNLayerWidget::createId(), std::make_shared<Conv1dLayerParams>()};
      break;
    case 1 :
      return new NNLayerWidget{NNLayerWidget::createId(), std::make_shared<Conv2dLayerParams>()};
      break;
    default : std::cerr << "NE" << std::endl; return nullptr;
  }
}

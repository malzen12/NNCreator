#include "FabricLayer.h"
#include "BilinearLayerParams.h"
#include "Conv1dLayerParams.h"
#include "Conv2dLayerParams.h"
#include "Conv3dLayerParams.h"
#include "LinearLayerParams.h"
#include "NNLayerWidget.h"
#include <iostream>

FabricLayer::FabricLayer()
    : IFabric("Layer") {
  initLinear();
  initConvolution();
}

auto FabricLayer::createCopy(const NNWidget* layer, const QPoint& point) -> KeyType {
  const auto id = NNWidget::createId();
  auto const pLayerCopy = new NNLayerWidget{id, layer->getParams()->copy()};
  emit createWidget(pLayerCopy, point);
  return id;
}

auto FabricLayer::bufferCopy(const NNWidget* layer) const -> NNWidget* {
  return new NNLayerWidget{layer->getId(), layer->getParams()->copy()};
}

void FabricLayer::initLinear() {
  m_FabricMap["Linear"] = [](KeyType id) -> NNWidget* {
    return new NNLayerWidget{id, std::make_shared<LinearLayerParams>()};
  };
  m_FabricMap["Bilinear"] = [](KeyType id) -> NNWidget* {
    return new NNLayerWidget{id, std::make_shared<BilinearLayerParams>()};
  };
}

void FabricLayer::initConvolution() {
  m_FabricMap["Conv1d"] = [](KeyType id) -> NNWidget* {
    return new NNLayerWidget{id, std::make_shared<Conv1dLayerParams>()};
  };
  m_FabricMap["Conv2d"] = [](KeyType id) -> NNWidget* {
    return new NNLayerWidget{id, std::make_shared<Conv2dLayerParams>()};
  };
  m_FabricMap["Conv3d"] = [](KeyType id) -> NNWidget* {
    return new NNLayerWidget{id, std::make_shared<Conv3dLayerParams>()};
  };
}







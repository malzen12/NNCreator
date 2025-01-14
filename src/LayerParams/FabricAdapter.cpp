#include "FabricAdapter.h"

FabricAdapter::FabricAdapter()
    : IFabric("Adapter") {}


auto FabricAdapter::createCopy(const NNWidget* layer, const QPoint& point) -> KeyType {
//  const auto id = NNWidget::createId();
//  auto const pLayerCopy = new NNLayerWidget{id, layer->getParams()->copy()};
//  emit createWidget(pLayerCopy, point);
//  return id;
  return {};
}

auto FabricAdapter::bufferCopy(const NNWidget* layer) const -> NNWidget* {
//  return new NNLayerWidget{layer->getId(), layer->getParams()->copy()};
  return {};
}

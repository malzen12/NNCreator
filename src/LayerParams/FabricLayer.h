#pragma once

#include "IFabric.h"

class FabricLayer : public IFabric {
public:
  FabricLayer();

  auto createCopy(const NNWidget* pLayer, const QPoint& point) -> KeyType final;
  auto bufferCopy(const NNWidget* pLayer) const -> NNWidget* final;
private:
  void initLinear();
  void initConvolution();
};


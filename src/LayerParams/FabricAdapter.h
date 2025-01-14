#pragma once

#include "IFabric.h"

class FabricAdapter : public IFabric {
public:
  FabricAdapter();

  auto createCopy(const NNWidget* pLayer, const QPoint& point) -> KeyType final;
  auto bufferCopy(const NNWidget* pLayer) const -> NNWidget* final;

private:
  void initSomething();
};

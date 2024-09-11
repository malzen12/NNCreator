#pragma once

#include "NNLayerParams.h"

class BilinearLayerParams : public NNLayerParams {
public:
  BilinearLayerParams();
  BilinearLayerParams(const std::string& strName, const NNParamContainer& vParams);

  auto copy() const -> std::shared_ptr<NNLayerParams>;
  bool checkInputSize(const InputSizeType& vInputSizes) const final;
  auto calcOutputSize(const InputSizeType& vInputSizes) const -> Vector final;
};

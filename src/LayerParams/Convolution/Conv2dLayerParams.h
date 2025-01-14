#pragma once

#include "NNLayerParams.h"

class Conv2dLayerParams : public NNLayerParams {
public:
  Conv2dLayerParams();
  Conv2dLayerParams(const std::string &strName, const std::vector<NNParam> &vParams);

  auto copy() const -> std::shared_ptr<NNLayerParams>;
  auto checkInputSize(const InputSizeType& vInputSizes, bool& isGlobalError) const -> std::string final;
  auto calcOutputSize(const InputSizeType& vInputSizes) const -> VectorOfVector final;
};

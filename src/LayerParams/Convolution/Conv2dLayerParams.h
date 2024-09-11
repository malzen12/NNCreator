#pragma once

#include "NNLayerParams.h"

class Conv2dLayerParams : public NNLayerParams {
public:
  Conv2dLayerParams();
  Conv2dLayerParams(const std::string &strName, const std::vector<NNParam> &vParams);

  auto copy() const -> std::shared_ptr<NNLayerParams>;
  bool checkInputSize(const InputSizeType& vInputSizes) const final;
  std::vector<std::size_t> calcOutputSize(const InputSizeType &vInputSizes) const final;
};

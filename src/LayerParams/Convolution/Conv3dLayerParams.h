#pragma once

#include "NNLayerParams.h"

class Conv3dLayerParams : public NNLayerParams {
public:
  Conv3dLayerParams();
  Conv3dLayerParams(const std::string& strName, const std::vector<NNParam>& vParams);

  auto copy() const -> std::shared_ptr<NNLayerParams>;
  auto checkInputSize(const InputSizeType& vInputSizes, bool& isGlobalError) const -> std::string final;
  auto calcOutputSize(const InputSizeType& vInputSizes) const -> VectorOfVector final;
};

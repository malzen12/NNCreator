#pragma once

#include "NNLayerParams.h"

class Conv1dLayerParams : public NNLayerParams {
public:
  Conv1dLayerParams();
  Conv1dLayerParams(const std::string& strName, const NNParamContainer& vParams);

  auto copy() const -> std::shared_ptr<NNLayerParams>;
  auto checkInputSize(const InputSizeType& vInputSizes, bool& isGlobalError) const -> std::string final;
  auto calcOutputSize(const InputSizeType& vInputSizes) const -> VectorOfVector final;
};

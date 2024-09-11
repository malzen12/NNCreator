#pragma once

#include "NNLayerParams.h"

class Conv1dLayerParams : public NNLayerParams {
public:
  Conv1dLayerParams();
  Conv1dLayerParams(const std::string& strName, const NNParamContainer& vParams);

  auto copy() const -> std::shared_ptr<NNLayerParams>;
  bool checkInputSize(const InputSizeType& vInputSizes) const final;
  auto calcOutputSize(const InputSizeType& vInputSizes) const -> Vector final;
};

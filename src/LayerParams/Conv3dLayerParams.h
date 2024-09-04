#pragma once

#include "NNLayerParams.h"

class Conv3dLayerParams : public NNLayerParams
{
public:
  Conv3dLayerParams();
  Conv3dLayerParams(const std::string& strName, const std::vector<NNParam>& vParams);

  bool checkInputSize(const InputSizeType& vInputSizes) const final;
  std::vector<std::size_t> calcOutputSize(const InputSizeType& vInputSizes) const final;
};

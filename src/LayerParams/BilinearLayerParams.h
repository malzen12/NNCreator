#pragma once

#include "NNLayerParams.h"

class BilinearLayerParams : public NNLayerParams
{
public:
  BilinearLayerParams(const std::string& strName, const std::vector<NNParam>& vParams);

  bool checkInputSize(const InputSizeType& vInputSizes) const final;
  std::vector<std::size_t> calcOutputSize(const InputSizeType& vInputSizes) const final;
};


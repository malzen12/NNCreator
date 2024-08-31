#pragma once

#include "NNLayerParams.h"

class LinearLayerParams : public NNLayerParams
{
public:
  LinearLayerParams(const std::string& strName, const std::vector<NNParam>& vParams);

  bool checkInputSize(const std::vector<std::vector<std::size_t>>& vInputSizes) const final;
  std::vector<std::size_t> calcOutputSize(const std::vector<std::vector<std::size_t>>& vInputSizes) const final;
};


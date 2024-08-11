#pragma once

#include "NNLayerParams.h"

class FlattenLayerParams : public NNLayerParams
{
public:
  FlattenLayerParams(const std::string& strName, const std::vector<NNParam>& vParams);

  bool checkInputSize(const std::vector<std::size_t>& vInputSize) const final;
  std::vector<std::size_t> calcOutputSize(const std::vector<std::size_t>& vInputSize) const final;
};


#pragma once

#include "NNLayerParams.h"

class RecurrentLayerParams : public NNLayerParams
{
public:
  RecurrentLayerParams(const std::string& strName, const std::vector<NNParam>& vParams);

  QString getDisplayName() const noexcept final;

  bool checkInputSize(const InputSizeType& vInputSizes) const final;
  std::vector<std::size_t> calcOutputSize(const InputSizeType& vInputSizes) const final;
};


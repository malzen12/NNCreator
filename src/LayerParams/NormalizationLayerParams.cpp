#include "NormalizationLayerParams.h"

NormalizationLayerParams::NormalizationLayerParams(const std::string& strName, const std::vector<NNParam>& vParams)
  : NNLayerParams(strName, vParams)
{
  assert(m_vParams.size() >= 2);
}

QString NormalizationLayerParams::getDisplayName() const noexcept
{
  return m_vParams[0].getValue().toString();
}

bool NormalizationLayerParams::checkInputSize(const InputSizeType& vInputSizes) const
{
  auto& vInputSize = vInputSizes.front();
  return !vInputSize.empty();
}

std::vector<std::size_t> NormalizationLayerParams::calcOutputSize(const InputSizeType& vInputSizes) const
{
  auto& vInputSize = vInputSizes.front();
  return vInputSize;
}


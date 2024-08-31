#include "ActivationLayerParams.h"

ActivationLayerParams::ActivationLayerParams(const std::string& strName, const std::vector<NNParam>& vParams)
  : NNLayerParams(strName, vParams)
{
  assert(m_vParams.size() >= 1);
}

QString ActivationLayerParams::getDisplayName() const noexcept
{
  return m_vParams.front().getValue().toString();
}

bool ActivationLayerParams::checkInputSize(const InputSizeType& vInputSizes) const
{
  return !vInputSizes.empty();
}

std::vector<std::size_t> ActivationLayerParams::calcOutputSize(const InputSizeType& vInputSizes) const
{
  auto& vInputSize = vInputSizes.front();
  return vInputSize;
}

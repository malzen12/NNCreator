#include "LinearLayerParams.h"

LinearLayerParams::LinearLayerParams(const std::string& strName, const std::vector<NNParam>& vParams)
  : NNLayerParams(strName, vParams)
{
  assert(m_vParams.size() >= 3);
}

bool LinearLayerParams::checkInputSize(const InputSizeType& vInputSizes) const
{
  auto& vInputSize = vInputSizes.front();
  return m_vParams[0].getValue().toUInt() == vInputSize.back();
}

std::vector<std::size_t> LinearLayerParams::calcOutputSize(const InputSizeType& vInputSizes) const
{
  auto& vInputSize = vInputSizes.front();
  auto vRes = vInputSize;
  vRes.back() = m_vParams[1].getValue().toUInt();
  return vRes;
}


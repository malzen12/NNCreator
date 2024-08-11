#include "PoolLayerParams.h"

PoolLayerParams::PoolLayerParams(const std::string& strName, const std::vector<NNParam>& vParams)
  : NNLayerParams(strName, vParams)
{
  assert(m_vParams.size() >= 4);
}

bool PoolLayerParams::checkInputSize(const std::vector<std::size_t>& vInputSize) const
{
  auto sKernel = m_vParams[0].getValue().toUInt();

  return  vInputSize.back() > sKernel;
}

std::vector<std::size_t> PoolLayerParams::calcOutputSize(const std::vector<std::size_t>& vInputSize) const
{
  assert(vInputSize.size() > 1);

  auto sKernel = m_vParams[0].getValue().toUInt();
  auto sStride = m_vParams[1].getValue().toUInt();
  auto sPadding = m_vParams[2].getValue().toUInt();
  auto sDilitation = m_vParams[3].getValue().toUInt();

  auto vRes = vInputSize;
  vRes.back() = (vInputSize.back() + 2 * sPadding - sDilitation * (sKernel - 1) - 1) / sStride + 1;

  return vRes;
}

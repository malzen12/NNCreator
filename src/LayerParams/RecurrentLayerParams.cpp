#include "RecurrentLayerParams.h"

RecurrentLayerParams::RecurrentLayerParams(const std::string& strName, const std::vector<NNParam>& vParams)
  : NNLayerParams(strName, vParams)
{
  assert(m_vParams.size() >= 6);
}

QString RecurrentLayerParams::getDisplayName() const noexcept
{
  return m_vParams[0].getValue().toString();
}

bool RecurrentLayerParams::checkInputSize(const std::vector<std::size_t>& vInputSize) const
{
  auto sInputSize = m_vParams[1].getValue().toUInt();

  return vInputSize.back() == sInputSize;
}

std::vector<std::size_t> RecurrentLayerParams::calcOutputSize(const std::vector<std::size_t>& vInputSize) const
{
  assert(vInputSize.size() >= 1);

  auto sHiddenSize = m_vParams[2].getValue().toUInt();
  auto bBidirectional = m_vParams[5].getValue().toBool();

  auto vRes = vInputSize;

  vRes.back()= sHiddenSize * (bBidirectional? 2 : 1);

  return vRes;
}

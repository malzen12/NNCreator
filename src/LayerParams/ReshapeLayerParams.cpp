#include "ReshapeLayerParams.h"

ReshapeLayerParams::ReshapeLayerParams(const std::string& strName, const std::vector<NNParam>& vParams)
  : NNLayerParams(strName, vParams)
{
  assert(m_vParams.size() >= 1);
}

bool ReshapeLayerParams::checkInputSize(const InputSizeType& vInputSizes) const
{
  return !m_vParams[0].getValue().toList().empty();
}

std::vector<std::size_t> ReshapeLayerParams::calcOutputSize(const InputSizeType& vInputSizes) const
{
  auto List = m_vParams[0].getValue().toList();
  std::vector<std::size_t> vRes;

  for (const auto& crValue : List){
    vRes.push_back(crValue.toUInt());
  }

  return vRes;
}

#include "EmbeddingLayerParams.h"

EmbeddingLayerParams::EmbeddingLayerParams(const std::string& strName, const std::vector<NNParam>& vParams)
  : NNLayerParams(strName, vParams)
{
  assert(m_vParams.size() >= 2);
}

bool EmbeddingLayerParams::checkInputSize(const InputSizeType& vInputSizes) const
{
  auto& vInputSize = vInputSizes.front();
  return !vInputSize.empty();
}

std::vector<std::size_t> EmbeddingLayerParams::calcOutputSize(const InputSizeType& vInputSizes) const
{
  auto& vInputSize = vInputSizes.front();
  auto vRes = vInputSize;
  vRes.push_back(m_vParams[1].getValue().toUInt());

  return vRes;
}


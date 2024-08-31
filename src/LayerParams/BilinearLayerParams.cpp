#include "BilinearLayerParams.h"

BilinearLayerParams::BilinearLayerParams(const std::string& strName, const std::vector<NNParam>& vParams)
  : NNLayerParams(strName, vParams)
{
  assert(m_vParams.size() >= 3);
}

bool BilinearLayerParams::checkInputSize(const InputSizeType& vInputSizes) const
{
  auto& vInputSize1 = vInputSizes.front();
  auto& vInputSize2 = vInputSizes.back();
  return vInputSizes.size() == 2
      && (
         (m_vParams[0].getValue().toUInt() == vInputSize1.back()
          && m_vParams[1].getValue().toUInt() == vInputSize2.back())
      || (m_vParams[1].getValue().toUInt() == vInputSize1.back()
          && m_vParams[0].getValue().toUInt() == vInputSize2.back())
      );
}

std::vector<std::size_t> BilinearLayerParams::calcOutputSize(const InputSizeType& vInputSizes) const
{
  auto& vInputSize = vInputSizes.front();
  auto vRes = vInputSize;
  vRes.back() = m_vParams[2].getValue().toUInt();
  return vRes;
}


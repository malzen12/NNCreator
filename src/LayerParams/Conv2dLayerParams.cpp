#include "Conv2dLayerParams.h"

///< @todo to aux
inline std::size_t calc_output_axis(std::size_t sInput, std::size_t sKernel, std::size_t sStride, std::size_t sPadding, std::size_t sDilitation)
{
  return static_cast<double>(sInput + 2 * sPadding - sDilitation * (sKernel - 1) - 1) / sStride + 1;
}

Conv2dLayerParams::Conv2dLayerParams(const std::string& strName, const std::vector<NNParam>& vParams)
  : NNLayerParams(strName, vParams)
{
  assert(m_vParams.size() >= 6);
}

bool Conv2dLayerParams::checkInputSize(const std::vector<std::size_t>& vInputSize) const
{
  auto lKernel = m_vParams[2].getValue().toList();
  auto sInDepth = m_vParams[0].getValue().toUInt();

  return vInputSize.size() > 2
      && vInputSize[vInputSize.size() - 1] > lKernel[1].toUInt()
      && vInputSize[vInputSize.size() - 2] > lKernel[0].toUInt()
      && vInputSize[vInputSize.size() - 3] == sInDepth;
}

std::vector<std::size_t> Conv2dLayerParams::calcOutputSize(const std::vector<std::size_t>& vInputSize) const
{
  assert(vInputSize.size() > 1);

  auto sOutDepth = m_vParams[1].getValue().toUInt();

  auto lKernel = m_vParams[2].getValue().toList();
  auto lStride = m_vParams[3].getValue().toList();
  auto lPadding = m_vParams[4].getValue().toList();
  auto lDilitation = m_vParams[5].getValue().toList();

  auto vRes = vInputSize;
  vRes.back() = calc_output_axis(vInputSize.back(),
                                 lKernel[1].toUInt(), lStride[1].toUInt(),
                                 lPadding[1].toUInt(), lDilitation[1].toUInt());
  vRes[vRes.size() - 2] = calc_output_axis(vInputSize[vInputSize.size() - 2],
                                           lKernel[0].toUInt(), lStride[0].toUInt(),
                                           lPadding[0].toUInt(), lDilitation[0].toUInt());
  vRes[vRes.size() - 3] = sOutDepth;

  return vRes;
}

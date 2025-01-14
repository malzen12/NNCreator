#include "Conv2dLayerParams.h"
#include "InitializerFunc.h"
#include "PaddingMode.h"
#include <iostream>


Conv2dLayerParams::Conv2dLayerParams() {
  m_strName = "Conv2d";
  m_vParams = {
      NNParam{"Input I[N,?,1,0]", "in_channels", 0},
      NNParam{"Output O[N,?,1,0]", "out_channels", 0},
      NNParam{"Kernel Size", "kernel_size", QList<QVariant>{1, 1}, QVariant::Type::List},
      NNParam{"Stride", "stride", QList<QVariant>{1, 1}, QVariant::Type::List},
      NNParam{"Padding", "padding", QList<QVariant>{0, 0}, QVariant::Type::List},
      NNParam{"Dilation", "dilation", QList<QVariant>{1, 1}, QVariant::Type::List},
      NNParam{"Groups", "groups", 1},
      NNParam{"Bias", "bias", true, QVariant::Type::Bool},
      NNParam{"Padding Mode", PaddingMode::getClassName(), 0, QVariant::Type::UInt, true},
      NNParam{"Initializer Func", InitializerFunc::getClassName(), 0, QVariant::Type::UInt, true}
  };
}

Conv2dLayerParams::Conv2dLayerParams(const std::string& strName, const std::vector<NNParam>& vParams)
    : NNLayerParams(strName, vParams) {}

auto Conv2dLayerParams::copy() const -> std::shared_ptr<NNLayerParams> {
  return std::make_shared<Conv2dLayerParams>(m_strName, copyImpl());
}

auto Conv2dLayerParams::checkInputSize(const InputSizeType& vInputSizes, bool& isGlobalError) const
    -> std::string {
  const auto sInput = m_vParams[0].getValue().toUInt();
  const auto sOutput = m_vParams[1].getValue().toUInt();
  const auto lKernel = m_vParams[2].getValue().toList();
  const auto lStride = m_vParams[3].getValue().toList();
  const auto lPadding = m_vParams[4].getValue().toList();
  const auto lDilation = m_vParams[5].getValue().toList();
  const auto sGroups = m_vParams[6].getValue().toUInt();

  std::stringstream errorStream;
  if (! (vInputSizes.size() == 1)) {
    errorStream << m_strName << " must have 1 inputs" << '\n';
    isGlobalError = true;
    return errorStream.str();
  }

  const auto& vInputSize = vInputSizes.front();
  if (! (vInputSize.size() == 3 || vInputSize.size() == 4)) {
    errorStream << "Expected 3D (unbatched) or 4D (batched) input but got input of size " << vInputSize.back()
                << '\n';
    isGlobalError = true;
    return errorStream.str();
  }

  const auto calcInputSize = [](KeyType sInput, KeyType sPadding) { return sInput + sPadding * 2; };
  const auto calcKernelSize = [](KeyType sKernel, KeyType sDilation) { return (sKernel - 1) * sDilation + 1; };

  if (int inputSize0 = calcInputSize(vInputSize[vInputSize.size() - 2], lPadding[0].toUInt()),
      kernelSize0 = calcKernelSize(lKernel[0].toUInt(), lDilation[0].toUInt()),
      inputSize1 = calcInputSize(vInputSize.back(), lStride[1].toUInt()),
      kernelSize1 = calcKernelSize(lKernel[1].toUInt(), lDilation[1].toUInt());
      ! (inputSize0 >= kernelSize0 && inputSize1 >= kernelSize1)) {
    errorStream << "Calculated padded input size per channel: "
                << "[" << inputSize0 << ", " << inputSize1 << ']' << " (input + pading * 2)."
                << " Kernel size: "
                << "[" << kernelSize0 << ", " << kernelSize1 << ']'
                << " ((kernel - 1) * dilation + 1). Kernel size can't be greater than actual input size"
                << '\n';
  }

  if (! (vInputSize[vInputSize.size() - 3] == sInput))
    errorStream << "Expected Input: " << vInputSize[vInputSize.size() - 3] << '\n';

  for (const auto& sKernel : lKernel)
    if (! (sKernel.toUInt() > 0)) errorStream << "Kernel should be greater than zero " << '\n';
  for (const auto& sStride : lStride)
    if (! (sStride.toUInt() > 0)) errorStream << "Stride should be greater than zero " << '\n';
  for (const auto& sDilation : lDilation)
    if (! (sDilation.toUInt() > 0)) errorStream << "Dilation should be greater than zero " << '\n';

  if (! (sInput % sGroups == 0))
    errorStream << "Input must be divisible by groups " << sInput << " " << sGroups << '\n';
  if (! (sOutput % sGroups == 0))
    errorStream << "Output must be divisible by groups" << sOutput << " " << sGroups << '\n';
  return errorStream.str();
}

auto Conv2dLayerParams::calcOutputSize(const InputSizeType& vInputSizes) const -> VectorOfVector {
  auto &vInputSize = vInputSizes.front();
  assert(vInputSize.size() > 1);

  const auto sOutput = m_vParams[1].getValue().toUInt();
  const auto lKernel = m_vParams[2].getValue().toList();
  const auto lStride = m_vParams[3].getValue().toList();
  const auto lPadding = m_vParams[4].getValue().toList();
  const auto lDilation = m_vParams[5].getValue().toList();

  auto vRes = vInputSize;
  const auto calcOutput = [](KeyType sInput, KeyType sKernel, KeyType sStride, KeyType sPadding,
                             KeyType sDilation) {
    return (sInput + 2 * sPadding - sDilation * (sKernel - 1) - 1) / sStride + 1;
  };

  vRes.back() = calcOutput(vInputSize.back(), lKernel[1].toUInt(), lStride[1].toUInt(), lPadding[1].toUInt(),
                           lDilation[1].toUInt());
  vRes[vRes.size() - 2] = calcOutput(vInputSize[vInputSize.size() - 2], lKernel[0].toUInt(),
                                     lStride[0].toUInt(), lPadding[0].toUInt(), lDilation[0].toUInt());
  vRes[vRes.size() - 3] = sOutput;

  return {vRes};
}

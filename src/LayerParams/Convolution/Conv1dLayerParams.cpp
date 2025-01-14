#include "Conv1dLayerParams.h"
#include "InitializerFunc.h"
#include "PaddingMode.h"
#include <iostream>

Conv1dLayerParams::Conv1dLayerParams() {
  m_strName = "Conv1d";
  m_vParams = {
      NNParam{"Input I[N,?,0]", "in_channels", 0},
      NNParam{"Output O[N,?,0]", "out_channels", 0},
      NNParam{"Kernel Size", "kernel_size", 1},
      NNParam{"Stride", "stride", 1},
      NNParam{"Padding", "padding", 0},
      NNParam{"Dilation", "dilation", 1},
      NNParam{"Groups", "groups", 1},
      NNParam{"Bias", "bias", true, QVariant::Type::Bool},
      NNParam{"Padding Mode", PaddingMode::getClassName(), 0, QVariant::Type::UInt, true},
      NNParam{"Initializer Func", InitializerFunc::getClassName(), 0, QVariant::Type::UInt, true}
  };
}

Conv1dLayerParams::Conv1dLayerParams(const std::string& strName, const NNParamContainer& vParams)
    : NNLayerParams(strName, vParams) {}

auto Conv1dLayerParams::copy() const -> std::shared_ptr<NNLayerParams> {
  return std::make_shared<Conv1dLayerParams>(m_strName, copyImpl());
}

auto Conv1dLayerParams::checkInputSize(const InputSizeType& vInputSizes, bool& isGlobalError) const
    -> std::string {
  const auto sInput = m_vParams[0].getValue().toUInt();
  const auto sOutput = m_vParams[1].getValue().toUInt();
  const auto sKernel = m_vParams[2].getValue().toUInt();
  const auto sStride = m_vParams[3].getValue().toUInt();
  const auto sPadding = m_vParams[4].getValue().toUInt();
  const auto sDilation = m_vParams[5].getValue().toUInt();
  const auto sGroups = m_vParams[6].getValue().toUInt();

  std::stringstream errorStream;
  if (! (vInputSizes.size() == 1)) {
    errorStream << m_strName << " must have 1 input" << '\n';
    isGlobalError = true;
    return errorStream.str();
  }

  const auto& vInputSize = vInputSizes.front();
  if (! (vInputSize.size() == 2 || vInputSize.size() == 3)) {
    errorStream << "Expected 2D (unbatched) or 3D (batched) input "
                << "but got input of size " << vInputSize.back() << '\n';
    isGlobalError = true;
    return errorStream.str();
  }
  if (int inputSize = vInputSize.back() + sPadding * 2, kernelSize = (sKernel - 1) * sDilation + 1;
      ! (inputSize >= kernelSize)) {
    errorStream << "Calculated padded input size per channel: " << '[' << inputSize << ']'
                << " (input + pading * 2). Kernel size: " << '[' << kernelSize << ']'
                << " ((kernel - 1) * dilation + 1). Kernel size can't be greater than actual input size"
                << '\n';
  }
  if (! (vInputSize[vInputSize.size() - 2] == sInput))
    errorStream << "Expected Input: " << vInputSize[vInputSize.size() - 2] << '\n';
  if (! (sKernel > 0)) errorStream << "Kernel should be greater than zero " << '\n';
  if (! (sStride > 0)) errorStream << "Stride should be greater than zero " << '\n';
  if (! (sDilation > 0)) errorStream << "Dilation should be greater than zero " << '\n';

  if (! (sInput % sGroups == 0))
    errorStream << "Input must be divisible by groups " << sInput << " " << sGroups << '\n';
  if (! (sOutput % sGroups == 0))
    errorStream << "Output must be divisible by groups " << sOutput << " " << sGroups << '\n';
  return errorStream.str();
}

auto Conv1dLayerParams::calcOutputSize(const InputSizeType& vInputSizes) const -> VectorOfVector {
  const auto sOutDepth = m_vParams[1].getValue().toUInt();
  const auto sKernel = m_vParams[2].getValue().toUInt();
  const auto sStride = m_vParams[3].getValue().toUInt();
  const auto sPadding = m_vParams[4].getValue().toUInt();
  const auto sDilation = m_vParams[5].getValue().toUInt();

  const auto& vInputSize = vInputSizes.front();
  assert(vInputSize.size() > 1 && vInputSize.size() <= 3);


  auto vRes = vInputSize;
  vRes.back() = (vRes.back() + 2 * sPadding - sDilation * (sKernel - 1) - 1) / sStride + 1;
  vRes[vRes.size() - 2] = sOutDepth;

  return {vRes};
}

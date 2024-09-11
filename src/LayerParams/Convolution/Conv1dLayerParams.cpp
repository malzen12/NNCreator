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
      NNParam{"Padding Mode", "padding_mode", PaddingMode{static_cast<e_padding_mode>(0)}.toString(),
              QVariant::Type::String, true},
      NNParam{"Initializer Func", InitializerFunc::getClassName(),
              InitializerFunc{static_cast<initializer_func>(0)}.toString(), QVariant::Type::String, true}
  };
}

Conv1dLayerParams::Conv1dLayerParams(const std::string& strName, const NNParamContainer& vParams)
    : NNLayerParams(strName, vParams) {}

auto Conv1dLayerParams::copy() const -> std::shared_ptr<NNLayerParams> {
  return std::make_shared<Conv1dLayerParams>(m_strName, copyImpl());
}

bool Conv1dLayerParams::checkInputSize(const InputSizeType &vInputSizes) const {
  const auto sInput = m_vParams[0].getValue().toUInt();
  const auto sOutput = m_vParams[1].getValue().toUInt();
  const auto sKernel = m_vParams[2].getValue().toUInt();
  const auto sStride = m_vParams[3].getValue().toUInt();
  const auto sPadding = m_vParams[4].getValue().toUInt();
  const auto sDilation = m_vParams[5].getValue().toUInt();
  const auto sGroups = m_vParams[6].getValue().toUInt();

  auto &vInputSize = vInputSizes.front();
  auto bInput = true;
  if (!(vInputSizes.size() == 1)) {
    bInput = false;
    std::cerr << m_strName << " must have 1 inputs" << std::endl;
  }
  if (!(vInputSize.size() == 2 || vInputSize.size() == 3)) {
    bInput = false;
    std::cerr << "Expected 2D (unbatched) or 3D (batched) input "
              << "but got input of size " << vInputSize.back() << std::endl;
  }
  if (int input_size = vInputSize.back() + sPadding * 2,
      kernel_size = (sKernel - 1) * sDilation + 1;
      !(input_size >= kernel_size)) {
    bInput = false;
    std::cerr << "Calculated padded input size per channel: " << input_size
              << "(input_back + pading * 2). Kernel size: " << kernel_size
              << "((kernel - 1) * dilation + 1). Kernel size can't be greater "
                 "than actual input size";
  }
  if (!(vInputSize[vInputSize.size() - 2] == sInput)) {
    bInput = false;
    std::cerr << "Expected Input: " << vInputSize[vInputSize.size() - 2] << std::endl;
  }
  if (!(sInput % sGroups == 0)) {
    bInput = false;
    std::cerr << "Input must be divisible by groups " << sInput << " "
              << sGroups << std::endl;
  }
  if (!(sOutput % sGroups == 0)) {
    bInput = false;
    std::cerr << "Output must be divisible by groups" << sOutput << " "
              << sGroups << std::endl;
  }
  return bInput;
}

auto Conv1dLayerParams::calcOutputSize(const InputSizeType& vInputSizes) const -> Vector {
  const auto sOutDepth = m_vParams[1].getValue().toUInt();
  const auto sKernel = m_vParams[2].getValue().toUInt();
  const auto sStride = m_vParams[3].getValue().toUInt();
  const auto sPadding = m_vParams[4].getValue().toUInt();
  const auto sDilitation = m_vParams[5].getValue().toUInt();

  const auto& vInputSize = vInputSizes.front();
  assert(vInputSize.size() > 1 && vInputSize.size() <= 3);


  auto vRes = vInputSize;
  vRes.back() = (vRes.back() + 2 * sPadding - sDilitation * (sKernel - 1) - 1) / sStride + 1;
  vRes[vRes.size() - 2] = sOutDepth;

  return vRes;
}

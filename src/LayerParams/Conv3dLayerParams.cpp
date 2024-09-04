#include "Conv3dLayerParams.h"
#include "InitializerFunc.h"
#include "PaddingMode.h"
#include <iostream>

inline std::size_t calc_output_axis(std::size_t sInput, std::size_t sKernel, std::size_t sStride, std::size_t sPadding, std::size_t sDilitation)
{
  return static_cast<double>(sInput + 2 * sPadding - sDilitation * (sKernel - 1) - 1) / sStride + 1;
}

Conv3dLayerParams::Conv3dLayerParams()
{
  m_strName = "Conv3d";
  m_vParams = {
      NNParam{"Input I[N,?,2,1,0]", "in_channels", 0},
      NNParam{"Output O[N,?,2,1,0]", "out_channels", 0},
      NNParam{"Kernel Size", "kernel_size", QList<QVariant>{1, 1},
              QVariant::Type::List},
      NNParam{"Stride", "stride", QList<QVariant>{1, 1, 1},
              QVariant::Type::List},
      NNParam{"Padding", "padding", QList<QVariant>{0, 0, 0},
              QVariant::Type::List},
      NNParam{"Dilation", "dilation", QList<QVariant>{1, 1, 1},
              QVariant::Type::List},
      NNParam{"Groups", "groups", 1},
      NNParam{"Bias", "bias", true, QVariant::Type::Bool},
      NNParam{"Padding Mode", "padding_mode",
              PaddingMode{static_cast<e_padding_mode>(0)}.toString(),
              QVariant::Type::String, true},
      NNParam{"Initializer Func", InitializerFunc::getClassName(),
              InitializerFunc{static_cast<initializer_func>(0)}.toString(),
              QVariant::Type::String, true}};
}


bool Conv3dLayerParams::checkInputSize(const InputSizeType& vInputSizes) const ///@todo
{
  auto sInput = m_vParams[0].getValue().toUInt();
  auto sOutput = m_vParams[1].getValue().toUInt();
  auto lKernel = m_vParams[2].getValue().toList();
  auto sGroups = m_vParams[6].getValue().toUInt();
  auto sInDepth = m_vParams[0].getValue().toUInt();

  auto& vInputSize = vInputSizes.front();
  auto bInput = true;
//  if (!(vInputSizes.size() == 1)) {
//    bInput = false;
//    std::cerr << m_strName << " must have 1 inputs" << std::endl;
//  }
//  if (!(vInputSize.size() == 3 || vInputSize.size() == 4)) {
//    bInput = false;
//    std::cerr << "Expected 3D (unbatched) or 4D (batched) input "
//              << "but got input of size " << vInputSize.back() << std::endl;
//  }

//  //&& vInputSize[vInputSize.size() - 1] > lKernel[1].toUInt()
//  //&& vInputSize[vInputSize.size() - 2] > lKernel[0].toUInt()

//  if (!(vInputSize[vInputSize.size() - 3] == sInput)) {
//    bInput = false;
//    std::cerr << "Expected Input: " << vInputSize[vInputSize.size() - 3]
//              << std::endl;
//  }
//  if (!(sInput % sGroups == 0)) {
//    bInput = false;
//    std::cerr << "Input must be divisible by groups " << sInput << " "
//              << sGroups << std::endl;
//  }
//  if (!(sOutput % sGroups == 0)) {
//    bInput = false;
//    std::cerr << "Output must be divisible by groups" << sOutput << " "
//              << sGroups << std::endl;
//  }
  return bInput;
}

std::vector<std::size_t> Conv3dLayerParams::calcOutputSize(const InputSizeType& vInputSizes) const ///@todo
{
  auto& vInputSize = vInputSizes.front();
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

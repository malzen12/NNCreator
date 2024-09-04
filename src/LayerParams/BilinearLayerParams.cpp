#include "BilinearLayerParams.h"
#include <iostream>

BilinearLayerParams::BilinearLayerParams() {
  m_strName = "Bilinear";
  m_vParams = {
    NNParam{"Input 1 I[*,?]", "in1_features", 0},
    NNParam{"Input 2 I[*,?]", "in2_features", 0},
    NNParam{"Output O[*,?]", "out_features", 0},
    NNParam{"Bias", "bias", true, QVariant::Type::Bool},
    NNParam{"Initializer Func", InitializerFunc::getClassName(),
            InitializerFunc{static_cast<initializer_func>(0)}.toString(),
            QVariant::Type::String, true}};
}

BilinearLayerParams::BilinearLayerParams(const std::string& strName, const std::vector<NNParam>& vParams)
  : NNLayerParams(strName, vParams)
{
  assert(m_vParams.size() >= 3);
}

bool BilinearLayerParams::checkInputSize(const InputSizeType& vInputSizes) const {
  auto& vInputSize1 = vInputSizes.front();
  auto& vInputSize2 = vInputSizes.back();

  auto bInput = true;
  if (!(vInputSizes.size() == 2)) {
    bInput = false;
    std::cerr << m_strName << " must have 2 inputs" << std::endl;
  }

  if (!((m_vParams[0].getValue().toUInt() == vInputSize1.back() &&
         m_vParams[1].getValue().toUInt() == vInputSize2.back()) ||
        (m_vParams[1].getValue().toUInt() == vInputSize1.back() &&
         m_vParams[0].getValue().toUInt() == vInputSize2.back()))) {
    bInput = false;
    std::cerr << "Expected Input 1: " << vInputSize1.back()
              << " Input 2: " << vInputSize2.back()
              << " or Input 2: " << vInputSize2.back()
              << " Input 1:" << vInputSize1.back() << std::endl;
  }
  return bInput;
}

std::vector<std::size_t> BilinearLayerParams::calcOutputSize(const InputSizeType& vInputSizes) const
{
  auto& vInputSize = vInputSizes.front();
  auto vRes = vInputSize;
  vRes.back() = m_vParams[2].getValue().toUInt();
  return vRes;
}


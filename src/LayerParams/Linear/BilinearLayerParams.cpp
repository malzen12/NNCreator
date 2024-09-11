#include "BilinearLayerParams.h"
#include "InitializerFunc.h"
#include <iostream>

BilinearLayerParams::BilinearLayerParams() {
  m_strName = "Bilinear";
  m_vParams = {
      NNParam{"Input 1 I[*,?]", "in1_features", 0},
      NNParam{"Input 2 I[*,?]", "in2_features", 0},
      NNParam{"Output O[*,?]", "out_features", 0},
      NNParam{"Bias", "bias", true, QVariant::Type::Bool},
      NNParam{"Initializer Func", InitializerFunc::getClassName(),
              InitializerFunc{static_cast<initializer_func>(0)}.toString(), QVariant::Type::String, true}
  };
}

BilinearLayerParams::BilinearLayerParams(const std::string& strName, const NNParamContainer& vParams)
    : NNLayerParams(strName, vParams) {}

auto BilinearLayerParams::copy() const -> std::shared_ptr<NNLayerParams> {
  return std::make_shared<BilinearLayerParams>(m_strName, copyImpl());
}

bool BilinearLayerParams::checkInputSize(const InputSizeType &vInputSizes) const {
  const auto input1 = m_vParams[0].getValue().toUInt();
  const auto input2 = m_vParams[1].getValue().toUInt();

  const auto& vInputSize1 = vInputSizes.front();
  const auto& vInputSize2 = vInputSizes.back();

  auto isValid = true;
  if (!(vInputSizes.size() == 2)) {
    isValid = false;
    std::cerr << m_strName << " must have 2 inputs" << std::endl;
  }
  if (const auto size1 = vInputSize1.size(), size2 = vInputSize2.size(); ! (size1 == size2)) {
    isValid = false;
    std::cerr << "Inputs size are not equal: " << size1 << ", " << size2 << std::endl;
  } else {
    bool isEqual = true;
    for (auto i = 0; i < size1 - 1; ++i) {
      if (! (vInputSize1[i] == vInputSize2[i])) {
        isEqual = false;
        break;
      }
    }
    if (! isEqual) {
      std::cerr << "Inputs are not equal" << std::endl;
      std::cerr << "First :" << std::endl;
      for (auto i = 0; i < size1 - 1; ++i) std::cerr << ' ' << vInputSize1[i];
      std::cerr << std::endl;
      std::cerr << "Second:" << std::endl;
      for (auto i = 0; i < size1 - 1; ++i) std::cerr << ' ' << vInputSize2[i];
      std::cerr << std::endl;
    }
  }

  if (! ((input1 == vInputSize1.back() && input2 == vInputSize2.back())
         || (input2 == vInputSize1.back() && input1 == vInputSize2.back()))) {
    isValid = false;
    std::cerr << "Expected Input 1: " << vInputSize1.back()
              << " Input 2: " << vInputSize2.back()
              << " or Input 2: " << vInputSize2.back()
              << " Input 1:" << vInputSize1.back() << std::endl;
  }
  return isValid;
}

auto BilinearLayerParams::calcOutputSize(const InputSizeType& vInputSizes) const -> Vector {
  const auto output = m_vParams[2].getValue().toUInt();
  auto& vInputSize = vInputSizes.front();
  auto vRes = vInputSize;
  vRes.back() = output;
  return vRes;
}

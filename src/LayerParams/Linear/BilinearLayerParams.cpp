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
      NNParam{"Initializer Func", InitializerFunc::getClassName(), 0, QVariant::Type::UInt, true}
  };
}

BilinearLayerParams::BilinearLayerParams(const std::string& strName, const NNParamContainer& vParams)
    : NNLayerParams(strName, vParams) {}

auto BilinearLayerParams::copy() const -> std::shared_ptr<NNLayerParams> {
  return std::make_shared<BilinearLayerParams>(m_strName, copyImpl());
}

auto BilinearLayerParams::checkInputSize(const InputSizeType& vInputSizes, bool& isGlobalError) const
    -> std::string {
  const auto input1 = m_vParams[0].getValue().toUInt();
  const auto input2 = m_vParams[1].getValue().toUInt();

  std::stringstream errorStream;
  if (!(vInputSizes.size() == 2)) {
    errorStream << m_strName << " must have 2 inputs" << '\n';
    isGlobalError = true;
    return errorStream.str();
  }

  const auto& vInputSize1 = vInputSizes.front();
  const auto& vInputSize2 = vInputSizes.back();
  if (const auto size1 = vInputSize1.size(), size2 = vInputSize2.size(); ! (size1 == size2)) {
    errorStream << "Inputs size are not equal: " << size1 << ", " << size2 << '\n';
  } else {
    if (! (size1 > 0)) return errorStream.str();
    bool isEqual = true;
    for (auto i = 0u; i < size1 - 1; ++i) {
      if (! (vInputSize1[i] == vInputSize2[i])) {
        isEqual = false;
        break;
      }
    }
    if (! isEqual) {
      errorStream << "Inputs are not equal" << '\n';
      errorStream << "First :" << '\n';
      for (auto i = 0u; i < size1 - 1; ++i) errorStream << ' ' << vInputSize1[i];
      errorStream << '\n';
      errorStream << "Second:" << '\n';
      for (auto i = 0u; i < size1 - 1; ++i) errorStream << ' ' << vInputSize2[i];
      errorStream << '\n';
    }
  }

  if (! ((input1 == vInputSize1.back() && input2 == vInputSize2.back())
         || (input2 == vInputSize1.back() && input1 == vInputSize2.back()))) {
    errorStream << "Expected Input 1: " << vInputSize1.back() << " Input 2: " << vInputSize2.back()
                << " or Input 2: " << vInputSize2.back() << " Input 1:" << vInputSize1.back() << '\n';
  }
  return errorStream.str();
}

auto BilinearLayerParams::calcOutputSize(const InputSizeType& vInputSizes) const -> VectorOfVector {
  const auto output = m_vParams[2].getValue().toUInt();
  auto& vInputSize = vInputSizes.front();
  auto vRes = vInputSize;
  vRes.back() = output;
  return {vRes};
}

#include "LinearLayerParams.h"
#include "InitializerFunc.h"
#include <iostream>

LinearLayerParams::LinearLayerParams() {
  m_strName = "Linear";
  m_vParams = {
      NNParam{"Input I[*,?]", "in_features", 0},
      NNParam{"Output O[*,?]", "out_features", 0},
      NNParam{"Bias", "bias", true, QVariant::Type::Bool},
      NNParam{"Initializer Func", InitializerFunc::getClassName(),
              InitializerFunc{static_cast<initializer_func>(0)}.toString(), QVariant::Type::String, true}
  };
}

LinearLayerParams::LinearLayerParams(const std::string& strName, const NNParamContainer& vParams)
    : NNLayerParams(strName, vParams) {}

auto LinearLayerParams::copy() const -> std::shared_ptr<NNLayerParams> {
  return std::make_shared<LinearLayerParams>(m_strName, copyImpl());
}

bool LinearLayerParams::checkInputSize(const InputSizeType& vInputSizes) const {
  const auto input = m_vParams[0].getValue().toUInt();
  const auto& vInputSize = vInputSizes.front();

  auto isValid = true;
  if (! (vInputSizes.size() == 1)) {
    isValid = false;
    std::cerr << m_strName << " must have 1 input" << std::endl;
  }
  if (! (vInputSize.back() == input)) {
    isValid = false;
    std::cerr << "Expected Input: " << vInputSize.back() << std::endl;
  }
  return isValid;
}

auto LinearLayerParams::calcOutputSize(const InputSizeType& vInputSizes) const -> Vector {
  const auto output = m_vParams[1].getValue().toUInt();
  const auto& vInputSize = vInputSizes.front();
  auto vRes = vInputSize;

  vRes.back() = output;
  return vRes;
}

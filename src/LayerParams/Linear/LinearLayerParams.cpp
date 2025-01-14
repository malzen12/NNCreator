#include "LinearLayerParams.h"
#include "InitializerFunc.h"
#include <iostream>

LinearLayerParams::LinearLayerParams() {
  m_strName = "Linear";
  m_vParams = {
      NNParam{"Input I[*,?]", "in_features", 0},
      NNParam{"Output O[*,?]", "out_features", 0},
      NNParam{"Bias", "bias", true, QVariant::Type::Bool},
      NNParam{"Initializer Func", InitializerFunc::getClassName(), 0, QVariant::Type::UInt, true}
  };
}

LinearLayerParams::LinearLayerParams(const std::string& strName, const NNParamContainer& vParams)
    : NNLayerParams(strName, vParams) {}

auto LinearLayerParams::copy() const -> std::shared_ptr<NNLayerParams> {
  return std::make_shared<LinearLayerParams>(m_strName, copyImpl());
}

auto LinearLayerParams::checkInputSize(const InputSizeType& vInputSizes, bool& isGlobalError) const
    -> std::string {
  const auto input = m_vParams[0].getValue().toUInt();

  std::stringstream errorStream;
  if (! (vInputSizes.size() == 1)) {
    errorStream << m_strName << " must have 1 input" << '\n';
    isGlobalError = true;
    return errorStream.str();
  }

  const auto& vInputSize = vInputSizes.front();
  if (! (vInputSize.back() == input)) errorStream << "Expected Input: " << vInputSize.back() << '\n';
  return errorStream.str();
}

auto LinearLayerParams::calcOutputSize(const InputSizeType& vInputSizes) const -> VectorOfVector {
  const auto output = m_vParams[1].getValue().toUInt();
  const auto& vInputSize = vInputSizes.front();
  auto vRes = vInputSize;

  vRes.back() = output;
  return {vRes};
}

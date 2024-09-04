#include "LinearLayerParams.h"
#include <iostream>

LinearLayerParams::LinearLayerParams(){
  m_strName = "Linear";
  m_vParams = {
    NNParam{"Input I[*,?]", "in_features", 0},
    NNParam{"Output O[*,?]", "out_features", 0},
    NNParam{"Bias", "bias", true, QVariant::Type::Bool},
    NNParam{"Initializer Func", InitializerFunc::getClassName(),
            InitializerFunc{static_cast<initializer_func>(0)}.toString(),
            QVariant::Type::String, true}};
}

LinearLayerParams::LinearLayerParams(const std::string& strName, const std::vector<NNParam>& vParams)
  : NNLayerParams(strName, vParams)
{
  assert(m_vParams.size() >= 3);
}

bool LinearLayerParams::checkInputSize(const InputSizeType& vInputSizes) const
{
  auto sInput = m_vParams[0].getValue().toUInt();
  auto& vInputSize = vInputSizes.front();
  auto bInput = true;

  if (!(vInputSizes.size() == 1)) {
    bInput = false;
    std::cerr << m_strName << " must have 1 inputs" << std::endl;
  }
  if(!(vInputSize.back() == sInput)){
    bInput = false;
    std::cerr << "Expected Input: " << vInputSize.back() << std::endl;
  }
  return bInput;
}

std::vector<std::size_t> LinearLayerParams::calcOutputSize(const InputSizeType& vInputSizes) const
{
  auto& vInputSize = vInputSizes.front();
  auto vRes = vInputSize;
  vRes.back() = m_vParams[1].getValue().toUInt();
  return vRes;
}


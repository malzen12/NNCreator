#pragma once

#include "InitializerFunc.h"
#include "NNLayerParams.h"

class BilinearLayerParams : public NNLayerParams
{
public:
  BilinearLayerParams(const std::string& strName, const std::vector<NNParam>& vParams);
  BilinearLayerParams() {
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

  bool checkInputSize(const InputSizeType& vInputSizes) const final;
  std::vector<std::size_t> calcOutputSize(const InputSizeType& vInputSizes) const final;
};


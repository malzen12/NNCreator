#pragma once

#include "InitializerFunc.h"
#include "NNLayerParams.h"

class LinearLayerParams : public NNLayerParams
{
public:
  LinearLayerParams(const std::string& strName, const std::vector<NNParam>& vParams);
  LinearLayerParams(){
    m_strName = "Linear";
    m_vParams = {
        NNParam{"Input I[*,?]", "in_features", 0},
        NNParam{"Output O[*,?]", "out_features", 0},
        NNParam{"Bias", "bias", true, QVariant::Type::Bool},
        NNParam{"Initializer Func", InitializerFunc::getClassName(),
                InitializerFunc{static_cast<initializer_func>(0)}.toString(),
                QVariant::Type::String, true}};
  }

  bool checkInputSize(const std::vector<std::vector<std::size_t>>& vInputSizes) const final;
  std::vector<std::size_t> calcOutputSize(const std::vector<std::vector<std::size_t>>& vInputSizes) const final;
private:


};


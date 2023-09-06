#pragma once

#include "NNLayerParams.h"

class NormalizationLayerParams : public NNLayerParams
{
public:
    explicit NormalizationLayerParams(const std::string& strName, const std::vector<NNParam>& vParams);

    bool checkInputSize(const std::vector<std::size_t>& vInputSize) const;
    std::vector<std::size_t> calcOutputSize(const std::vector<std::size_t>& vInputSize) const;

    std::string makeXmlString() const;
};


#pragma once

#include "NNLayerParams.h"

class PoolLayerParams : public NNLayerParams
{
public:
    explicit PoolLayerParams(const std::string& strName, const std::vector<NNParam>& vParams);

    bool checkInputSize(const std::vector<std::size_t>& vInputSize) const;
    std::vector<std::size_t> calcOutputSize(const std::vector<std::size_t>& vInputSize) const;

    std::string makeXmlString() const;
};


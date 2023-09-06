#pragma once

#include "NNLayerParams.h"

class EmbeddingLayerParams : public NNLayerParams
{
public:
    explicit EmbeddingLayerParams(const std::string& strName, const std::vector<NNParam>& vParams);

    bool checkInputSize(const std::vector<std::size_t>& vInputSize) const;
    std::vector<std::size_t> calcOutputSize(const std::vector<std::size_t>& vInputSize) const;

    std::string makeXmlString() const;
};

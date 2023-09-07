#pragma once

#include "NNLayerParams.h"

class ActivationLayerParams : public NNLayerParams
{
public:
    explicit ActivationLayerParams(const std::string& strName, const std::vector<NNParam>& vParams);

    QString getDisplayName() const noexcept final;

    bool checkInputSize(const std::vector<std::size_t>& vInputSize) const final;
    std::vector<std::size_t> calcOutputSize(const std::vector<std::size_t>& vInputSize) const final;
};


#pragma once

#include <string>
#include <vector>

#include "NNParam.h"

class NNLayerParams
{
public:
    NNLayerParams() = default;
    explicit NNLayerParams(const std::string& strName, const std::vector<NNParam>& vParams);

    static NNLayerParams makeLinear();
    static NNLayerParams makeConv1d();
    static NNLayerParams makePool();
    static NNLayerParams makeEmbedding();
    static NNLayerParams makeReshape();
    static NNLayerParams makeNormalization();
    static NNLayerParams makeActivation();

    void setName(const std::string& strName) noexcept;
    const std::string& getName() const noexcept;

    const std::vector<NNParam>& getParams() const noexcept;
    void setParams(const std::vector<NNParam>& vParams) noexcept;

    bool isValid() const noexcept;

private:
    std::string m_strName;
    std::vector<NNParam> m_vParams;
};


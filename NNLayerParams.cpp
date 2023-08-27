#include "NNLayerParams.h"

NNLayerParams::NNLayerParams(const std::string& strName, const std::vector<NNParam>& vParams)
    : m_strName{strName},
      m_vParams{vParams}
{

}

NNLayerParams NNLayerParams::makeLinear()
{
    auto strName = "Linear";

    std::vector<NNParam> vParams = {NNParam{"input size", 0}, NNParam{"input size", 0}};

    return NNLayerParams{strName, vParams};
}

NNLayerParams NNLayerParams::makeConv1d()
{
    auto strName = "Linear";
    std::vector<NNParam> vParams = {NNParam{"input size", 0}, NNParam{"input size", 0}};

    return NNLayerParams{strName, vParams};
}

NNLayerParams NNLayerParams::makePool()
{
    auto strName = "Linear";
    std::vector<NNParam> vParams = {NNParam{"input size", 0}, NNParam{"input size", 0}};

    return NNLayerParams{strName, vParams};
}

NNLayerParams NNLayerParams::makeEmbedding()
{
    auto strName = "Linear";
    std::vector<NNParam> vParams = {NNParam{"input size", 0}, NNParam{"input size", 0}};

    return NNLayerParams{strName, vParams};
}

NNLayerParams NNLayerParams::makeReshape()
{
    auto strName = "Linear";
    std::vector<NNParam> vParams = {NNParam{"input size", 0}, NNParam{"input size", 0}};

    return NNLayerParams{strName, vParams};
}

NNLayerParams NNLayerParams::makeNormalization()
{
    auto strName = "Linear";
    std::vector<NNParam> vParams = {NNParam{"input size", 0}, NNParam{"input size", 0}};

    return NNLayerParams{strName, vParams};
}

NNLayerParams NNLayerParams::makeActivation()
{
    auto strName = "Activation function";

    std::vector<NNParam> vParams = {NNParam{"activation type", "relu", true}};///< @todo activation func

    return NNLayerParams{strName, vParams};
}

void NNLayerParams::setName(const std::string& strName) noexcept
{
    m_strName = strName;
}

const std::string& NNLayerParams::getName() const noexcept
{
    return m_strName;
}

const std::vector<NNParam>& NNLayerParams::getParams() const noexcept
{
    return m_vParams;
}

void NNLayerParams::setParams(const std::vector<NNParam>& vParams) noexcept
{
    m_vParams = vParams;
}

bool NNLayerParams::isValid() const noexcept
{
    return !m_vParams.empty();
}

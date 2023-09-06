#include "ActivationLayerParams.h"

ActivationLayerParams::ActivationLayerParams(const std::string& strName, const std::vector<NNParam>& vParams)
    : NNLayerParams(strName, vParams)
{
    assert(m_vParams.size() >= 1);
}

bool ActivationLayerParams::checkInputSize(const std::vector<std::size_t>& vInputSize) const
{
    return !vInputSize.empty();
}

std::vector<std::size_t> ActivationLayerParams::calcOutputSize(const std::vector<std::size_t>& vInputSize) const
{
    return vInputSize;
}

std::string ActivationLayerParams::makeXmlString() const
{
    return "";
}

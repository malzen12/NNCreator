#include "NormalizationLayerParams.h"

NormalizationLayerParams::NormalizationLayerParams(const std::string& strName, const std::vector<NNParam>& vParams)
    : NNLayerParams(strName, vParams)
{
    assert(m_vParams.size() >= 1);
}

bool NormalizationLayerParams::checkInputSize(const std::vector<std::size_t>& vInputSize) const
{
    return !vInputSize.empty();
}

std::vector<std::size_t> NormalizationLayerParams::calcOutputSize(const std::vector<std::size_t>& vInputSize) const
{
    return vInputSize;
}

std::string NormalizationLayerParams::makeXmlString() const
{
    return "";
}

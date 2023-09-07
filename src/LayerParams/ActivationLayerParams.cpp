#include "ActivationLayerParams.h"

ActivationLayerParams::ActivationLayerParams(const std::string& strName, const std::vector<NNParam>& vParams)
    : NNLayerParams(strName, vParams)
{
    assert(m_vParams.size() >= 1);
}

QString ActivationLayerParams::getDisplayName() const noexcept
{
    return m_vParams[0].getValue().toString();
}

bool ActivationLayerParams::checkInputSize(const std::vector<std::size_t>& vInputSize) const
{
    return !vInputSize.empty();
}

std::vector<std::size_t> ActivationLayerParams::calcOutputSize(const std::vector<std::size_t>& vInputSize) const
{
    return vInputSize;
}

#include "LinearLayerParams.h"

LinearLayerParams::LinearLayerParams(const std::string& strName, const std::vector<NNParam>& vParams)
    : NNLayerParams(strName, vParams)
{
    assert(m_vParams.size() >= 3);
}

bool LinearLayerParams::checkInputSize(const std::vector<std::size_t>& vInputSize) const
{
    return m_vParams[0].getValue().toUInt() == vInputSize.back();
}

std::vector<std::size_t> LinearLayerParams::calcOutputSize(const std::vector<std::size_t>& vInputSize) const
{
    auto vRes = vInputSize;
    vRes.back() = m_vParams[1].getValue().toUInt();
    return vRes;
}


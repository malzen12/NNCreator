#include "ConcatinateLayerParams.h"

ConcatinateLayerParams::ConcatinateLayerParams(const std::string& strName, const std::vector<NNParam>& vParams)
    : NNLayerParams(strName, vParams)
{
    assert(m_vParams.size() >= 1);
}

bool ConcatinateLayerParams::checkInputSize(const std::vector<std::size_t>& vInputSize) const
{
    auto sAxis = m_vParams[0].getValue().toUInt();
    return sAxis < vInputSize.size();
}

std::vector<std::size_t> ConcatinateLayerParams::calcOutputSize(const std::vector<std::size_t>& vInputSize) const
{
    return vInputSize;
}

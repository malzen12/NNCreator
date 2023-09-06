#include "Conv1dLayerParams.h"

Conv1dLayerParams::Conv1dLayerParams(const std::string& strName, const std::vector<NNParam>& vParams)
    : NNLayerParams(strName, vParams)
{
    assert(m_vParams.size() >= 6);
}

bool Conv1dLayerParams::checkInputSize(const std::vector<std::size_t>& vInputSize) const
{
    auto sKernel = m_vParams[2].getValue().toUInt();
    auto sInDepth = m_vParams[0].getValue().toUInt();

    return  vInputSize.size() > 1 &&
            vInputSize.back() > sKernel &&
            vInputSize[vInputSize.size() - 2] == sInDepth;
}

std::vector<std::size_t> Conv1dLayerParams::calcOutputSize(const std::vector<std::size_t>& vInputSize) const
{
    assert(vInputSize.size() > 1);

    auto sOutDepth = m_vParams[1].getValue().toUInt();

    auto sKernel = m_vParams[2].getValue().toUInt();
    auto sStride = m_vParams[3].getValue().toUInt();
    auto sPadding = m_vParams[4].getValue().toUInt();
    auto sDilitation = m_vParams[5].getValue().toUInt();

    auto vRes = vInputSize;
    vRes.back() = (vInputSize.back() + 2 * sPadding - sDilitation * (sKernel - 1) - 1) / sStride + 1;
    vRes[vRes.size() - 2] = sOutDepth;

    return vRes;
}

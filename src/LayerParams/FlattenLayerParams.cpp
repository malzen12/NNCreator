#include "FlattenLayerParams.h"

FlattenLayerParams::FlattenLayerParams(const std::string& strName, const std::vector<NNParam>& vParams)
    : NNLayerParams(strName, vParams)
{
    assert(m_vParams.size() >= 1);
}

bool FlattenLayerParams::checkInputSize(const std::vector<std::size_t>& vInputSize) const
{
    auto lAxisToFlat = m_vParams[0].getValue().toList();

    auto sTo = lAxisToFlat[1].toUInt();

    return  vInputSize.size() > sTo;
}

std::vector<std::size_t> FlattenLayerParams::calcOutputSize(const std::vector<std::size_t>& vInputSize) const
{
    assert(vInputSize.size() > 1);

    auto lAxisToFlat = m_vParams[0].getValue().toList();

    auto sFrom = lAxisToFlat[0].toUInt();
    auto sTo = lAxisToFlat[1].toUInt() + 1;

    std::vector<std::size_t> vRes;
    vRes.insert(vRes.end(), vInputSize.begin(), vInputSize.begin() + sFrom);

    auto sMultiply = std::accumulate(vInputSize.begin() + sFrom, vInputSize.begin() + sTo, std::size_t{1}, [](std::size_t sMultiply, std::size_t sNext)
    {
        return sMultiply * sNext;
    });

    vRes.push_back(sMultiply);

    vRes.insert(vRes.end(), vInputSize.begin() + sTo, vInputSize.end());

    return vRes;
}

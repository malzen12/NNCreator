#include "NNLayerSettings.h"

void NNLayerSettings::setName(const std::string& strName) noexcept
{
    m_strName = strName;
}

const std::string& NNLayerSettings::getName() const noexcept
{
    return m_strName;
}

void NNLayerSettings::setNCnt(int iNCnt) noexcept
{
    m_iNCnt = iNCnt;
}

int NNLayerSettings::getNCnt() const noexcept
{
    return m_iNCnt;
}

void NNLayerSettings::setAFunc(const ActivationFunc& crAFunc) noexcept
{
    m_AFunc = crAFunc;
}

const ActivationFunc& NNLayerSettings::getAFunc() const noexcept
{
    return m_AFunc;
}

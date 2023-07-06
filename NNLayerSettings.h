#pragma once

#include <string>

#include "ActivationFunc.h"

class NNLayerSettings
{
public:
    void setName(const std::string& strName) noexcept;
    const std::string& getName() const noexcept;

    void setNCnt(int iNCnt) noexcept;
    int getNCnt() const noexcept;

    void setAFunc(const ActivationFunc& crAFunc) noexcept;
    const ActivationFunc& getAFunc() const noexcept;

private:
    std::string m_strName;
    int m_iNCnt = 0;
    ActivationFunc m_AFunc = ActivationFunc::fromString("Relu");
};


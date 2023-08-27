#include "NNParam.h"

NNParam::NNParam(const std::string& strName, const QVariant& crValue, bool bEnum)
    : m_strName{strName},
      m_Value(crValue),
      m_bEnum{bEnum}
{

}

const std::string& NNParam::getName() const noexcept
{
    return m_strName;
}

QVariant NNParam::getValue() const noexcept
{
    return m_Value;
}

bool NNParam::isEnum() const noexcept
{
    return m_bEnum;
}

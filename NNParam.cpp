#include "NNParam.h"

NNParam::NNParam(const std::string& strName, const QVariant& crValue, QVariant::Type Type, bool bEnum)
    : m_strName{strName},
      m_Value(crValue),
      m_Type(Type),
      m_bEnum{bEnum}
{

}

const std::string& NNParam::getName() const noexcept
{
    return m_strName;
}

const QVariant& NNParam::getValue() const noexcept
{
    return m_Value;
}

QVariant::Type NNParam::getType() const noexcept
{
    return m_Type;
}

bool NNParam::isEnum() const noexcept
{
    return m_bEnum;
}

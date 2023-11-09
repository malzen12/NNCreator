#include "NNParam.h"

#include <sstream>

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

void NNParam::setValue(const QVariant& crValue)
{
    m_Value = crValue;
}

std::string NNParam::makeXmlString() const
{
    std::stringstream Stream;
    Stream << "<" << m_strName << ">";

    if (m_Type == QVariant::Type::String)
        Stream << m_Value.toString().toStdString();
    else if (m_Type == QVariant::Type::UInt)
        Stream << m_Value.toUInt();
    else if (m_Type == QVariant::Type::Bool)
        Stream << m_Value.toBool();
    else if (m_Type == QVariant::Type::List)
    {
        auto lVal = m_Value.toList();
        for (const auto& crVal : lVal)
            Stream << "<v>" << crVal.toUInt() << "</v>";

    }
    else
        Stream << m_Value.toString().toStdString();

    Stream << "</" << m_strName << ">" << std::endl;
    return Stream.str();
}

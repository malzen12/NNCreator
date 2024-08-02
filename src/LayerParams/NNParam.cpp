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

  switch(m_Type){
    case QVariant::Type::UInt:
      Stream << m_Value.toUInt();
      break;
    case QVariant::Type::Bool:
      Stream << m_Value.toBool();
      break;
    case QVariant::Type::List:{
        auto lVal = m_Value.toList();
        for (const auto& crVal : lVal)
          Stream << "<v>" << crVal.toUInt() << "</v>";
        break;
    }
    case QVariant::Type::String:
    default:
      Stream << m_Value.toString().toStdString();
  }
  Stream << "</" << m_strName << ">" << std::endl;
  return Stream.str();
}

std::string NNParam::makePyString() const
{
  std::stringstream Stream;

  switch(m_Type){
    case QVariant::Type::UInt:
      Stream << m_Value.toUInt();
      break;
    case QVariant::Type::Bool:
      if(m_Value.toBool())
        Stream << "True";
      else
        Stream << "False";
      break;
    case QVariant::Type::List:{
        auto lVal = m_Value.toList();
        Stream << '(';
        if(!lVal.empty()){
          auto it = lVal.cbegin();
          Stream << it->toUInt();
          ++it;
          for(auto ite = lVal.cend(); it != ite; ++it){
            Stream <<", " << it->toUInt();
          }
        }
        Stream << ')';
        break;
    }
    case QVariant::Type::String:
    default:
      Stream << m_Value.toString().toStdString();
  }
  return Stream.str();
}

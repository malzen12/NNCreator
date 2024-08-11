#include "NNParam.h"

#include <sstream>

NNParam::NNParam(std::string strName, std::string strHiddenName,
                 QVariant crValue, QVariant::Type Type, bool bEnum)
  : m_strName{std::move(strName)},
    m_strHiddenName{std::move(strHiddenName)},
    m_Value(std::move(crValue)),
    m_Type(Type),
    m_bEnum{bEnum}
{}

NNParam::NNParam(std::string strName,
                 QVariant crValue, QVariant::Type Type, bool bEnum)
  : NNParam{strName, strName, crValue, Type, bEnum}
{}

const std::string& NNParam::getName() const noexcept
{
  return m_strName;
}

const std::string& NNParam::getHiddenName() const noexcept
{
  return m_strHiddenName;
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

const std::string NNParam::makeXmlString() const
{
  std::stringstream Stream;
  Stream << "<" << m_strHiddenName << ">";

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
  Stream << "</" << m_strHiddenName << ">" << std::endl;
  return Stream.str();
}

const std::string NNParam::makePyString() const
{
  std::stringstream Stream;
  if(m_bEnum){
    if(m_Type == QVariant::Type::String)
          Stream << "'" <<convertToQString().toStdString() << "'";
    return Stream.str();
  }

  switch(m_Type){
    case QVariant::Type::List:
      Stream << '('
             << convertToQString().toStdString()
             << ')';
      break;
    case QVariant::Type::UInt:
    case QVariant::Type::Bool:
    case QVariant::Type::String:
    default:
      Stream << convertToQString().toStdString();
  }
  return Stream.str();
}

const QString NNParam::convertToQString(const QString& qstrDelimiter) const
{
  if(m_bEnum)
    if(m_Type == QVariant::Type::String)
      return m_Value.toString();

  switch(m_Type){
    case QVariant::Type::UInt:
      return QString::number(m_Value.toUInt());
    case QVariant::Type::Bool:
      return m_Value.toBool()? "True": "False";
    case QVariant::Type::List:
      return listToQString(qstrDelimiter);
    case QVariant::Type::String:
    default:
      return m_Value.toString();
  }
}

const QString NNParam::listToQString(const QString& qstrDelimiter) const
{
  auto crList = m_Value.toList();
  if (crList.empty())
    return "";

  auto it = crList.cbegin();
  auto qstrRes = QString::number(it->toUInt());++it;
  for (auto ite = crList.cend(); it != ite; ++it)
    qstrRes += qstrDelimiter + QString::number(it->toUInt());

  return qstrRes;
}


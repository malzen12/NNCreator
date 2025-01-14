#include "NNParam.h"

#include <sstream>

NNParam::NNParam(std::string strName, std::string strHiddenName, QVariant crValue, QVariant::Type Type,
                 bool bEnum)
    : m_strName{std::move(strName)}
    , m_strHiddenName{std::move(strHiddenName)}
    , m_isEnum{bEnum}
    , m_Type(Type)
    , m_Value(std::move(crValue)) {}

NNParam::NNParam(std::string strName, QVariant crValue, QVariant::Type Type, bool bEnum)
    : NNParam{strName, strName, crValue, Type, bEnum} {}

auto NNParam::getName() const noexcept -> const std::string& { return m_strName; }

auto NNParam::getHiddenName() const noexcept -> const std::string& { return m_strHiddenName; }

auto NNParam::getValue() const noexcept -> const QVariant& { return m_Value; }

auto NNParam::getType() const noexcept -> QVariant::Type { return m_Type; }

bool NNParam::isEnum() const noexcept { return m_isEnum; }

void NNParam::setValue(const QVariant& crValue, bool isEnum) {
  m_Value = crValue;
  // m_Type = m_Value.type();
  m_isEnum = isEnum;
}

auto NNParam::save() const -> std::string {
  std::stringstream stream;
  // stream << m_strName << '\n';
  // stream << m_strHiddenName << '\n';
  stream << m_isEnum << '\n';
  stream << m_Type << '\n';
  stream << convertToString() << '\n';

  return stream.str();
}

void NNParam::load(std::stringstream& stream) {
  bool isEnum;
  int iType;
  QVariant::Type type;
  std::string strValue;
  QVariant value;
  stream >> isEnum >> iType;
  std::getline(stream, strValue);
  std::getline(stream, strValue, '\r');
  type = static_cast<QVariant::Type>(iType);
  value = convertFromString(type, strValue);
  setValue(value, isEnum);
}

auto NNParam::makeXmlString() const -> const std::string {
  std::stringstream Stream;
  Stream << "<" << m_strHiddenName << ">";
  switch (m_Type) {
    case QVariant::Type::UInt   : Stream << m_Value.toUInt(); break;
    case QVariant::Type::Bool   : Stream << m_Value.toBool(); break;
    case QVariant::Type::List   : Stream << listToXmlString(); break;
    case QVariant::Type::String :
    default                     : Stream << m_Value.toString().toStdString(); break;
  }
  Stream << "</" << m_strHiddenName << ">" << std::endl;
  return Stream.str();
}

auto NNParam::makePyString() const -> const std::string {
  std::stringstream Stream;
  if (m_isEnum) {
    if (m_Type == QVariant::Type::String) Stream << "'" << convertToString() << "'";
    return Stream.str();
  }
  switch (m_Type) {
    case QVariant::Type::List   : Stream << '(' << convertToString() << ')'; break;
    case QVariant::Type::Bool   : Stream << boolToPyString(); break;
    case QVariant::Type::UInt   :
    case QVariant::Type::String :
    default                     : Stream << convertToString();
  }
  return Stream.str();
}

auto NNParam::convertToQString(const QString& qstrDelimiter) const -> QString {
  if (m_isEnum)
    if (m_Type == QVariant::Type::String) return m_Value.toString();
  switch (m_Type) {
    case QVariant::Type::UInt   : return QString::number(m_Value.toUInt());
    case QVariant::Type::Bool   : return m_Value.toBool() ? "True" : "False";
    case QVariant::Type::List   : return listToQString(qstrDelimiter);
    case QVariant::Type::String :
    default                     : return m_Value.toString();
  }
}

auto NNParam::convertToString(const QString& qstrDelimiter) const -> std::string {
  return convertToQString(qstrDelimiter).toStdString();
}

auto NNParam::convertFromString(QVariant::Type type, const std::string& strValue) -> QVariant {
  switch (type) {
    case QVariant::Type::UInt   : return static_cast<unsigned>(std::stoul(strValue));
    case QVariant::Type::Bool   : return strValue == "True" ? true : false;
    case QVariant::Type::List   : return listFromString(strValue);
    case QVariant::Type::String :
    default                     : return QString::fromStdString(strValue);
  }
}

auto NNParam::listToXmlString() const -> std::string {
  auto lVal = m_Value.toList();
  std::stringstream Stream;
  for (const auto& crVal : lVal) Stream << "<v>" << crVal.toUInt() << "</v>";
  return Stream.str();
}

auto NNParam::listToQString(const QString& qstrDelimiter) const -> QString {
  auto crList = m_Value.toList();
  if (crList.empty()) return "";

  auto it = crList.cbegin();
  auto res = QString::number(it->toUInt());
  ++it;
  for (auto ite = crList.cend(); it != ite; ++it) res += qstrDelimiter + QString::number(it->toUInt());
  return res;
}

auto NNParam::listFromString(const std::string& strValue) -> QVariant {
  return QVariant{QList{QString::fromStdString(strValue).split(",")}};
}

auto NNParam::boolToPyString() const -> std::string { return m_Value.toBool() ? "True" : "False"; }

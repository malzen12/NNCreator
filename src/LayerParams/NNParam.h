#pragma once

#include <QVariant>

class NNParam {
public:
  NNParam(std::string strName, std::string strHiddenName, QVariant crValue,
          QVariant::Type Type = QVariant::Type::UInt, bool bEnum = false);
  NNParam(std::string strName, QVariant crValue, QVariant::Type Type = QVariant::Type::UInt,
          bool bEnum = false);

  auto getName() const noexcept -> const std::string&;
  auto getHiddenName() const noexcept -> const std::string&;
  auto getValue() const noexcept -> const QVariant&;
  auto getType() const noexcept -> QVariant::Type;
  bool isEnum() const noexcept;

  void setValue(const QVariant& crValue);

  // auto createPyName() const -> const std::string;
  auto makeXmlString() const -> const std::string;
  auto makePyString() const -> const std::string;

  auto convertToQString(const QString& qstrDelimiter = ", ") const -> QString;
  auto convertToString(const QString& qstrDelimiter = ", ") const -> std::string;

private:
  auto listToXmlString() const -> std::string;
  auto listToQString(const QString& qstrDelimiter = ", ") const -> QString;
  auto boolToPyString() const -> std::string;

private:
  std::string m_strName;
  std::string m_strHiddenName;
  QVariant m_Value;
  QVariant::Type m_Type;
  bool m_isEnum;
};

#pragma once

#include <QVariant>

class NNParam
{
public:
  NNParam(std::string strName, std::string strHiddenName,
          QVariant crValue, QVariant::Type Type = QVariant::Type::UInt, bool bEnum = false);
  NNParam(std::string strName,
          QVariant crValue, QVariant::Type Type = QVariant::Type::UInt, bool bEnum = false);

  const std::string& getName() const noexcept;
  const std::string& getHiddenName() const noexcept;
  const QVariant& getValue() const noexcept;
  QVariant::Type getType() const noexcept;
  bool isEnum() const noexcept;

  void setValue(const QVariant& crValue);

  const std::string createPyName() const;

  const std::string makeXmlString() const;
  const std::string makePyString() const;

  const QString convertToQString(const QString& qstrDelimiter = ", ") const;
private:
  const QString listToQString(const QString &qstrDelimiter = ", ") const;
private:
  std::string m_strName;
  std::string m_strHiddenName;
  QVariant m_Value;
  QVariant::Type m_Type;
  bool m_bEnum;
};


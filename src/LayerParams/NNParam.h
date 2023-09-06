#pragma once

#include <QVariant>

class NNParam
{
public:
    explicit NNParam(const std::string& strName, const QVariant& crValue, QVariant::Type Type = QVariant::Type::UInt, bool bEnum = false);

    const std::string& getName() const noexcept;
    const QVariant& getValue() const noexcept;
    QVariant::Type getType() const noexcept;
    bool isEnum() const noexcept;

    void setValue(const QVariant& crValue);

    std::string makeXmlString() const;

private:
    std::string m_strName;
    QVariant m_Value;
    QVariant::Type m_Type;
    bool m_bEnum;
};


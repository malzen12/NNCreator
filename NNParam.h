#pragma once

#include <QVariant>

class NNParam
{
public:
    explicit NNParam(const std::string& strName, const QVariant& crValue, bool bEnum = false);

    const std::string& getName() const noexcept;
    QVariant getValue() const noexcept;
    bool isEnum() const noexcept;

private:
    std::string m_strName;
    QVariant m_Value;
    bool m_bEnum;
};


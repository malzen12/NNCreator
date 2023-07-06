#pragma once

#include "QString"

class ActivationFunc
{
public:
    enum class activation_func
    {
        relu = 0,
        leaky_relu
    };

    explicit ActivationFunc(activation_func Type);

    static ActivationFunc fromString(const QString& qstrName);

    QString toString() const;

private:
    activation_func m_Type;
};

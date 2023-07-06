#include "ActivationFunc.h"

ActivationFunc::ActivationFunc(activation_func Type)
    : m_Type{Type}
{

}

ActivationFunc ActivationFunc::fromString(const QString& qstrName)
{
    if ("Relu" == qstrName)
        return ActivationFunc{activation_func::relu};
    else if ("LeakyRelu" == qstrName)
        return ActivationFunc{activation_func::leaky_relu};
    else
        throw std::string{"Invalid func name"};
}

QString ActivationFunc::toString() const
{
    if (activation_func::relu == m_Type)
        return "Relu";
    else if (activation_func::leaky_relu == m_Type)
        return "LeakyRelu";
    else
        return "";
}

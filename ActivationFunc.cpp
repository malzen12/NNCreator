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

QStringList ActivationFunc::getAllNames()
{
    QStringList NamesList;
    for (auto iFunc = 0; iFunc < static_cast<int>(activation_func::cnt); ++iFunc)
    {
        NamesList.append(ActivationFunc{static_cast<activation_func>(iFunc)}.toString());
    }
    return NamesList;
}

std::vector<activation_func> ActivationFunc::getAllValues()
{
    std::vector<activation_func> vRes;
    for (auto iFunc = 0; iFunc < static_cast<int>(activation_func::cnt); ++iFunc)
    {
        vRes.push_back(static_cast<activation_func>(iFunc));
    }

    return vRes;
}

std::string ActivationFunc::getClassName()
{
    return "Activation function";
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

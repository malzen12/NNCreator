#include "Optimizer.h"

Optimizer::Optimizer(optimizer Type)
    : m_Type{Type}
{

}

Optimizer Optimizer::fromString(const QString& qstrName)
{
    if ("Relu" == qstrName)
        return Optimizer{optimizer::relu};
    else if ("LeakyRelu" == qstrName)
        return Optimizer{optimizer::leaky_relu};
    else if ("Sigmoid" == qstrName)
        return Optimizer{optimizer::sigmoid};
    else if ("Tanh" == qstrName)
        return Optimizer{optimizer::tanh};
    else if ("Selu" == qstrName)
        return Optimizer{optimizer::selu};
    else
        throw std::string{"Invalid func name"};
}

QStringList Optimizer::getAllNames()
{
    QStringList NamesList;
    for (auto iFunc = 0; iFunc < static_cast<int>(optimizer::cnt); ++iFunc)
    {
        NamesList.append(Optimizer{static_cast<optimizer>(iFunc)}.toString());
    }
    return NamesList;
}

std::vector<optimizer> Optimizer::getAllValues()
{
    std::vector<optimizer> vRes;
    for (auto iFunc = 0; iFunc < static_cast<int>(optimizer::cnt); ++iFunc)
    {
        vRes.push_back(static_cast<optimizer>(iFunc));
    }

    return vRes;
}

std::string Optimizer::getClassName()
{
    return "Otimizer";
}

QString Optimizer::toString() const
{
    if (optimizer::relu == m_Type)
        return "Relu";
    else if (optimizer::leaky_relu == m_Type)
        return "LeakyRelu";
    if (optimizer::sigmoid == m_Type)
        return "Sigmoid";
    else if (optimizer::tanh == m_Type)
        return "Tanh";
    if (optimizer::selu == m_Type)
        return "Selu";
    else
        return "";
}

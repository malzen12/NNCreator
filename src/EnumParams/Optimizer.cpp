#include "Optimizer.h"

Optimizer::Optimizer(optimizer Type)
    : m_Type{Type}
{

}

Optimizer Optimizer::fromString(const QString& qstrName)
{
    if ("Adam" == qstrName)
        return Optimizer{optimizer::adam};
    else if ("AdamW" == qstrName)
        return Optimizer{optimizer::adam_w};
    else if ("SGD" == qstrName)
        return Optimizer{optimizer::sgd};
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
    return "Optimizer";
}

QString Optimizer::toString() const
{
    if (optimizer::adam == m_Type)
        return "Adam";
    else if (optimizer::adam_w == m_Type)
        return "AdamW";
    if (optimizer::sgd == m_Type)
        return "SGD";
    else
        return "";
}

#include "InitializerFunc.h"

InitializerFunc::InitializerFunc(initializer_func Type)
    : m_Type{Type}
{

}

InitializerFunc InitializerFunc::fromString(const QString& qstrName)
{
    if ("Xavier" == qstrName)
        return InitializerFunc{initializer_func::xavier};
    else if ("Xavier2" == qstrName)
        return InitializerFunc{initializer_func::leaky_relu};
    else
        throw std::string{"Invalid func name"};
}

QStringList InitializerFunc::getAllNames()
{
    QStringList NamesList;
    for (auto iFunc = 0; iFunc < static_cast<int>(initializer_func::cnt); ++iFunc)
    {
        NamesList.append(InitializerFunc{static_cast<initializer_func>(iFunc)}.toString());
    }
    return NamesList;
}

std::vector<initializer_func> InitializerFunc::getAllValues()
{
    std::vector<initializer_func> vRes;
    for (auto iFunc = 0; iFunc < static_cast<int>(initializer_func::cnt); ++iFunc)
    {
        vRes.push_back(static_cast<initializer_func>(iFunc));
    }

    return vRes;
}

std::string InitializerFunc::getClassName()
{
    return "Initializer function";
}

QString InitializerFunc::toString() const
{
    if (initializer_func::xavier == m_Type)
        return "Xavier";
    else if (initializer_func::leaky_relu == m_Type)
        return "Xavier2";
    else
        return "";
}

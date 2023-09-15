#include "Dropout.h"

Dropout::Dropout(enu_dropout Type)
    : m_Type{Type}
{

}

Dropout Dropout::fromString(const QString& qstrName)
{
    if ("Dropout" == qstrName)
        return Dropout{enu_dropout::dropout};
    else if ("Dropout1d" == qstrName)
        return Dropout{enu_dropout::dropout1d};
    else if ("AlphaDropout" == qstrName)
        return Dropout{enu_dropout::alpha_dropout};
    else if ("FeatureAlphaDropout" == qstrName)
        return Dropout{enu_dropout::feature_alpha_dropout};
    else
        throw std::string{"Invalid func name"};
}

QStringList Dropout::getAllNames()
{
    QStringList NamesList;
    for (auto iFunc = 0; iFunc < static_cast<int>(enu_dropout::cnt); ++iFunc)
    {
        NamesList.append(Dropout{static_cast<enu_dropout>(iFunc)}.toString());
    }
    return NamesList;
}

std::vector<enu_dropout> Dropout::getAllValues()
{
    std::vector<enu_dropout> vRes;
    for (auto iFunc = 0; iFunc < static_cast<int>(enu_dropout::cnt); ++iFunc)
    {
        vRes.push_back(static_cast<enu_dropout>(iFunc));
    }

    return vRes;
}

std::string Dropout::getClassName()
{
    return "Dropout";
}

QString Dropout::toString() const
{
    if (enu_dropout::dropout == m_Type)
        return "Dropout";
    else if (enu_dropout::dropout1d == m_Type)
        return "Dropout1d";
    else if (enu_dropout::alpha_dropout == m_Type)
        return "AlphaDropout";
    else if (enu_dropout::feature_alpha_dropout == m_Type)
        return "FeatureAlphaDropout";
    else
        return "";
}

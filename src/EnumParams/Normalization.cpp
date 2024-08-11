#include "Normalization.h"

Normalization::Normalization(enu_normalization Type)
  : m_Type{Type}
{}

Normalization Normalization::fromString(const QString& qstrName)
{
  if ("BatchNorm1d" == qstrName)
    return Normalization{enu_normalization::batchnorm1d};
  else if ("LayerNorm" == qstrName)
    return Normalization{enu_normalization::layernorm};
  else
    throw std::string{"Invalid func name"};
}

QStringList Normalization::getAllNames()
{
  QStringList NamesList;
  for (auto iFunc = 0; iFunc < static_cast<int>(enu_normalization::cnt); ++iFunc){
    NamesList.append(Normalization{static_cast<enu_normalization>(iFunc)}.toString());
  }
  return NamesList;
}

std::vector<enu_normalization> Normalization::getAllValues()
{
  std::vector<enu_normalization> vRes;
  for (auto iFunc = 0; iFunc < static_cast<int>(enu_normalization::cnt); ++iFunc){
    vRes.push_back(static_cast<enu_normalization>(iFunc));
  }

  return vRes;
}

std::string Normalization::getClassName()
{
  return "Normalization";
}

QString Normalization::toString() const
{
  switch(m_Type){
    case enu_normalization::batchnorm1d:
      return "BatchNorm1d";
    case enu_normalization::layernorm:
      return "LayerNorm";
    default:
      return "";
  }
}

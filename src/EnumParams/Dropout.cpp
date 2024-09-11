#include "Dropout.h"

Dropout::Dropout(enu_dropout type)
    : m_Type{type} {}

Dropout::Dropout(const std::string& strName) {
  if ("Dropout" == strName)
    m_Type = enu_dropout::dropout;
  else if ("Dropout1d" == strName)
    m_Type = enu_dropout::dropout1d;
  else if ("AlphaDropout" == strName)
    m_Type = enu_dropout::alpha_dropout;
  else if ("FeatureAlphaDropout" == strName)
    m_Type = enu_dropout::feature_alpha_dropout;
  else
    throw std::string{"Invalid func name"};
}

Dropout::Dropout(const QString& qstrName)
    : Dropout{qstrName.toStdString()} {}

auto Dropout::getAllNames() -> QStringList {
  QStringList NamesList;
  for (auto iFunc = 0; iFunc < static_cast<int>(enu_dropout::cnt); ++iFunc)
    NamesList.append(Dropout{static_cast<enu_dropout>(iFunc)}.toString());
  return NamesList;
}

auto Dropout::getAllValues() -> std::vector<enu_dropout> {
  std::vector<enu_dropout> vRes;
  for (auto iFunc = 0; iFunc < static_cast<int>(enu_dropout::cnt); ++iFunc)
    vRes.push_back(static_cast<enu_dropout>(iFunc));
  return vRes;
}

auto Dropout::getClassName() -> std::string { return "Dropout"; }

auto Dropout::toString() const -> QString {
  switch (m_Type) {
    case enu_dropout::dropout               : return "Dropout";
    case enu_dropout::dropout1d             : return "Dropout1d";
    case enu_dropout::alpha_dropout         : return "AlphaDropout";
    case enu_dropout::feature_alpha_dropout : return "FeatureAlphaDropout";
    default                                 : return "";
  }
}

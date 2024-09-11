#include "InitializerFunc.h"

InitializerFunc::InitializerFunc(initializer_func Type)
    : m_Type{Type} {}

InitializerFunc::InitializerFunc(const std::string& strName) {
  if ("Xavier" == strName)
    m_Type = initializer_func::xavier;
  else if ("Uniform" == strName)
    m_Type = initializer_func::uniform;
  else if ("Normal" == strName)
    m_Type = initializer_func::normal;
  else if ("Dirac" == strName)
    m_Type = initializer_func::dirac;
  else if ("KaimingNormal" == strName)
    m_Type = initializer_func::kaiming_normal;
  else
    throw std::string{"Invalid func name"};
}

InitializerFunc::InitializerFunc(const QString& qstrName)
    : InitializerFunc{qstrName.toStdString()} {}

auto InitializerFunc::getAllNames() -> QStringList {
  QStringList NamesList;
  for (auto iFunc = 0; iFunc < static_cast<int>(initializer_func::cnt); ++iFunc)
    NamesList.append(InitializerFunc{static_cast<initializer_func>(iFunc)}.toString());
  return NamesList;
}

auto InitializerFunc::getAllValues() -> std::vector<initializer_func> {
  std::vector<initializer_func> vRes;
  for (auto iFunc = 0; iFunc < static_cast<int>(initializer_func::cnt); ++iFunc)
    vRes.push_back(static_cast<initializer_func>(iFunc));
  return vRes;
}

auto InitializerFunc::getClassName() -> std::string { return "Initializer_function"; }

auto InitializerFunc::toString() const -> QString {
  switch (m_Type) {
    case initializer_func::xavier         : return "Xavier";
    case initializer_func::uniform        : return "Uniform";
    case initializer_func::normal         : return "Normal";
    case initializer_func::dirac          : return "Dirac";
    case initializer_func::kaiming_normal : return "KaimingNormal";
    default                               : return "";
  }
}

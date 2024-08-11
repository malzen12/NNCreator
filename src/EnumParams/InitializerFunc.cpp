#include "InitializerFunc.h"

InitializerFunc::InitializerFunc(initializer_func Type)
  : m_Type{Type}
{}

InitializerFunc InitializerFunc::fromString(const QString& qstrName)
{
  if ("Xavier" == qstrName)
    return InitializerFunc{initializer_func::xavier};
  else if ("Uniform" == qstrName)
    return InitializerFunc{initializer_func::uniform};
  else if ("Normal" == qstrName)
    return InitializerFunc{initializer_func::normal};
  else if ("Dirac" == qstrName)
    return InitializerFunc{initializer_func::dirac};
  else if ("KaimingNormal" == qstrName)
    return InitializerFunc{initializer_func::kaiming_normal};
  else
    throw std::string{"Invalid func name"};
}

QStringList InitializerFunc::getAllNames()
{
  QStringList NamesList;
  for (auto iFunc = 0; iFunc < static_cast<int>(initializer_func::cnt); ++iFunc){
    NamesList.append(InitializerFunc{static_cast<initializer_func>(iFunc)}.toString());
  }
  return NamesList;
}

std::vector<initializer_func> InitializerFunc::getAllValues()
{
  std::vector<initializer_func> vRes;
  for (auto iFunc = 0; iFunc < static_cast<int>(initializer_func::cnt); ++iFunc){
    vRes.push_back(static_cast<initializer_func>(iFunc));
  }

  return vRes;
}

std::string InitializerFunc::getClassName()
{
  return "Initializer_function";
}

QString InitializerFunc::toString() const
{
  switch(m_Type){
    case initializer_func::xavier:
      return "Xavier";
    case initializer_func::uniform:
      return "Uniform";
    case initializer_func::normal:
      return "Normal";
    case initializer_func::dirac:
      return "Dirac";
    case initializer_func::kaiming_normal:
      return "KaimingNormal";
    default:
      return "";
  }
}

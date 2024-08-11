#include "ActivationFunc.h"

ActivationFunc::ActivationFunc(activation_func Type)
  : m_Type{Type}
{}

ActivationFunc ActivationFunc::fromString(const QString& qstrName)
{
  if ("Relu" == qstrName)
    return ActivationFunc{activation_func::relu};
  else if ("LeakyRelu" == qstrName)
    return ActivationFunc{activation_func::leaky_relu};
  else if ("Sigmoid" == qstrName)
    return ActivationFunc{activation_func::sigmoid};
  else if ("Tanh" == qstrName)
    return ActivationFunc{activation_func::tanh};
  else if ("Selu" == qstrName)
    return ActivationFunc{activation_func::selu};
  else if ("Softmax" == qstrName)
    return ActivationFunc{activation_func::softmax};
  else
    throw std::string{"Invalid func name"};
}

QStringList ActivationFunc::getAllNames()
{
  QStringList NamesList;
  for (auto iFunc = 0; iFunc < static_cast<int>(activation_func::cnt); ++iFunc){
    NamesList.append(ActivationFunc{static_cast<activation_func>(iFunc)}.toString());
  }
  return NamesList;
}

std::vector<activation_func> ActivationFunc::getAllValues()
{
  std::vector<activation_func> vRes;
  for (auto iFunc = 0; iFunc < static_cast<int>(activation_func::cnt); ++iFunc){
    vRes.push_back(static_cast<activation_func>(iFunc));
  }
  return vRes;
}

std::string ActivationFunc::getClassName()
{
  return "Activation_function";
}

QString ActivationFunc::toString() const
{
  switch(m_Type){
    case activation_func::relu:
      return "Relu";
    case activation_func::leaky_relu:
      return "LeakyRelu";
    case activation_func::sigmoid:
      return "Sigmoid";
    case activation_func::tanh:
      return "Tanh";
    case activation_func::selu:
      return "Selu";
    case activation_func::softmax:
      return "Softmax";
    default:
      return "";
  }
}

#include "ActivationFunc.h"

ActivationFunc::ActivationFunc(activation_func type)
    : m_Type{type} {}

ActivationFunc::ActivationFunc(const std::string& strName) {
  if ("Relu" == strName)
    m_Type = activation_func::relu;
  else if ("LeakyRelu" == strName)
    m_Type = activation_func::leaky_relu;
  else if ("Sigmoid" == strName)
    m_Type = activation_func::sigmoid;
  else if ("Tanh" == strName)
    m_Type = activation_func::tanh;
  else if ("Selu" == strName)
    m_Type = activation_func::selu;
  else if ("Softmax" == strName)
    m_Type = activation_func::softmax;
  else
    throw std::string{"Invalid func name"};
}

ActivationFunc::ActivationFunc(const QString& qstrName)
    : ActivationFunc{qstrName.toStdString()} {}

auto ActivationFunc::getAllNames() -> QStringList {
  QStringList NamesList;
  for (auto iFunc = 0; iFunc < static_cast<int>(activation_func::cnt); ++iFunc)
    NamesList.append(ActivationFunc{static_cast<activation_func>(iFunc)}.toString());
  return NamesList;
}

auto ActivationFunc::getAllValues() -> std::vector<activation_func> {
  std::vector<activation_func> vRes;
  for (auto iFunc = 0; iFunc < static_cast<int>(activation_func::cnt); ++iFunc)
    vRes.push_back(static_cast<activation_func>(iFunc));
  return vRes;
}

auto ActivationFunc::getClassName() -> std::string { return "Activation_function"; }

auto ActivationFunc::toString() const -> QString {
  switch (m_Type) {
    case activation_func::relu       : return "Relu";
    case activation_func::leaky_relu : return "LeakyRelu";
    case activation_func::sigmoid    : return "Sigmoid";
    case activation_func::tanh       : return "Tanh";
    case activation_func::selu       : return "Selu";
    case activation_func::softmax    : return "Softmax";
    default                          : return "";
  }
}

#include "PaddingMode.h"

PaddingMode::PaddingMode(e_padding_mode Type)
    : m_Type{Type} {}

PaddingMode PaddingMode::fromString(const QString &qstrName) {
  if ("zeros" == qstrName)
    return PaddingMode{e_padding_mode::zeros};
  else if ("reflect" == qstrName)
    return PaddingMode{e_padding_mode::reflect};
  else if ("replicate" == qstrName)
    return PaddingMode{e_padding_mode::replicate};
  else if ("circular" == qstrName)
    return PaddingMode{e_padding_mode::circular};
  else
    throw std::string{"Invalid padding mode"};
}

QStringList PaddingMode::getAllNames() {
  QStringList NamesList;
  for (auto i = 0; i < static_cast<int>(e_padding_mode::cnt); ++i)
    NamesList.append(PaddingMode{static_cast<e_padding_mode>(i)}.toString());
  return NamesList;
}

std::vector<e_padding_mode> PaddingMode::getAllValues() {
  std::vector<e_padding_mode> vRes;
  for (auto iFunc = 0; iFunc < static_cast<int>(e_padding_mode::cnt); ++iFunc)
    vRes.push_back(static_cast<e_padding_mode>(iFunc));

  return vRes;
}

std::string PaddingMode::getClassName() { return "padding_mode"; }

QString PaddingMode::toString() const {
  switch (m_Type) {
    case e_padding_mode::zeros     : return "zeros";
    case e_padding_mode::reflect   : return "reflect"; ;
    case e_padding_mode::replicate : return "replicate"; ;
    case e_padding_mode::circular  : return "circular"; ;
    default                        : return "";
  }
}

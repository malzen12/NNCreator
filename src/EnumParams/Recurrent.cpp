#include "Recurrent.h"

Recurrent::Recurrent(enu_recurrent Type)
    : m_Type{Type} {}

Recurrent Recurrent::fromString(const QString &qstrName) {
  if ("RNN" == qstrName)
    return Recurrent{enu_recurrent::rnn};
  else if ("LSTM" == qstrName)
    return Recurrent{enu_recurrent::lstm};
  else if ("GRU" == qstrName)
    return Recurrent{enu_recurrent::gru};
  else
    throw std::string{"Invalid func name"};
}

QStringList Recurrent::getAllNames() {
  QStringList NamesList;
  for (auto iFunc = 0; iFunc < static_cast<int>(enu_recurrent::cnt); ++iFunc)
    NamesList.append(Recurrent{static_cast<enu_recurrent>(iFunc)}.toString());
  return NamesList;
}

std::vector<enu_recurrent> Recurrent::getAllValues() {
  std::vector<enu_recurrent> vRes;
  for (auto iFunc = 0; iFunc < static_cast<int>(enu_recurrent::cnt); ++iFunc)
    vRes.push_back(static_cast<enu_recurrent>(iFunc));

  return vRes;
}

std::string Recurrent::getClassName() { return "Recurrent"; }

QString Recurrent::toString() const {
  switch (m_Type) {
    case enu_recurrent::rnn  : return "RNN";
    case enu_recurrent::lstm : return "LSTM";
    case enu_recurrent::gru  : return "GRU";
    default                  : return "";
  }
}

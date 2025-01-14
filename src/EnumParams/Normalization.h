#pragma once

#include <vector>
#include <QString>
#include <QStringList>

enum class enu_normalization { batchnorm1d = 0, layernorm, cnt };

class Normalization
{
public:
  explicit Normalization(enu_normalization Type);

  static Normalization fromString(const QString& qstrName);

  static QStringList getAllNames();
  static std::vector<enu_normalization> getAllValues();

  static std::string getClassName();

  QString toString() const;

private:
  enu_normalization m_Type;
};

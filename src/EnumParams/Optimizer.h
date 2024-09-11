#pragma once

#include <vector>
#include <QString>
#include <QStringList>

enum class optimizer { adam = 0, adam_w, sgd, cnt };

class Optimizer
{
public:
  explicit Optimizer(optimizer Type);

  static Optimizer fromString(const QString& qstrName);

  static QStringList getAllNames();
  static std::vector<optimizer> getAllValues();

  static std::string getClassName();

  QString toString() const;

private:
  optimizer m_Type;
};

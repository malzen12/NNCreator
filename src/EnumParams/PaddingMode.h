#pragma once

#include <vector>

#include <QString>
#include <QStringList>

enum class e_padding_mode
{
  zeros = 0,
  reflect,
  replicate,
  circular,
  cnt
};

class PaddingMode
{
public:
  explicit PaddingMode(e_padding_mode Type);

  static PaddingMode fromString(const QString& qstrName);

  static QStringList getAllNames();
  static std::vector<e_padding_mode> getAllValues();

  static std::string getClassName();

  QString toString() const;

private:
  e_padding_mode m_Type;
};

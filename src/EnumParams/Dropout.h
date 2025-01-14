#pragma once

#include <vector>

#include <QString>
#include <QStringList>

enum class enu_dropout { dropout = 0, dropout1d, alpha_dropout, feature_alpha_dropout, cnt };

class Dropout {
public:
  explicit Dropout(enu_dropout type);
  explicit Dropout(const std::string& strName);
  explicit Dropout(const QString& qstrName);

  static auto getAllNames() -> QStringList;
  static auto getAllValues() -> std::vector<enu_dropout>;
  static auto getClassName() -> std::string;

  auto toString() const -> QString;

private:
  enu_dropout m_Type;
};

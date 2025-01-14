#pragma once

#include <vector>

#include <QString>
#include <QStringList>

enum class initializer_func { xavier = 0, uniform, normal, dirac, kaiming_normal, cnt };

class InitializerFunc {
public:
  explicit InitializerFunc(initializer_func Type);
  explicit InitializerFunc(const std::string& strName);
  explicit InitializerFunc(const QString& qstrName);

  static auto getAllNames() -> QStringList;
  static auto getAllValues() -> std::vector<initializer_func>;
  static auto getClassName() -> std::string;

  static auto fromString(const QString& qstrName);

  auto toString() const -> QString;

private:
  initializer_func m_Type;
};

#pragma once

#include <vector>

#include <QString>
#include <QStringList>

enum class activation_func { relu = 0, leaky_relu, sigmoid, tanh, selu, softmax, cnt };

class ActivationFunc {
public:
  explicit ActivationFunc(activation_func type);
  explicit ActivationFunc(const std::string& strName);
  explicit ActivationFunc(const QString& qstrName);

  static auto getAllNames() -> QStringList;
  static auto getAllValues() -> std::vector<activation_func>;
  static auto getClassName() -> std::string;

  static auto fromString(const QString& qstrName);

  auto toString() const -> QString;

private:
  activation_func m_Type;
};

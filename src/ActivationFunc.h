#pragma once

#include <vector>

#include <QString>
#include <QStringList>

enum class activation_func
{
    relu = 0,
    leaky_relu,
    sigmoid,
    tanh,
    selu,
    cnt
};

class ActivationFunc
{
public:
    explicit ActivationFunc(activation_func Type);

    static ActivationFunc fromString(const QString& qstrName);

    static QStringList getAllNames();
    static std::vector<activation_func> getAllValues();

    static std::string getClassName();

    QString toString() const;

private:
    activation_func m_Type;
};

#pragma once

#include <vector>

#include <QString>
#include <QStringList>

enum class initializer_func
{
    xavier = 0,
    leaky_relu,
    cnt
};

class InitializerFunc
{
public:
    explicit InitializerFunc(initializer_func Type);

    static InitializerFunc fromString(const QString& qstrName);

    static QStringList getAllNames();
    static std::vector<initializer_func> getAllValues();

    static std::string getClassName();

    QString toString() const;

private:
    initializer_func m_Type;
};


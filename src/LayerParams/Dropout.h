#pragma once

#include <vector>

#include <QString>
#include <QStringList>

enum class enu_dropout
{
    dropout = 0,
    dropout1d,
    alpha_dropout,
    feature_alpha_dropout,
    cnt
};

class Dropout
{
public:
    explicit Dropout(enu_dropout Type);

    static Dropout fromString(const QString& qstrName);

    static QStringList getAllNames();
    static std::vector<enu_dropout> getAllValues();

    static std::string getClassName();

    QString toString() const;

private:
    enu_dropout m_Type;
};

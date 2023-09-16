#pragma once

#include <vector>

#include <QString>
#include <QStringList>

enum class enu_recurrent
{
    rnn = 0,
    lstm,
    gru,
    cnt
};

class Recurrent
{
public:
    explicit Recurrent(enu_recurrent Type);

    static Recurrent fromString(const QString& qstrName);

    static QStringList getAllNames();
    static std::vector<enu_recurrent> getAllValues();

    static std::string getClassName();

    QString toString() const;

private:
    enu_recurrent m_Type;
};


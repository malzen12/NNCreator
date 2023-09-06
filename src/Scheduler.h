#pragma once

#include <vector>

#include <QString>
#include <QStringList>

enum class enu_scheduler
{
    linear = 0,
    exponential,
    multistep,
    cnt
};

class Scheduler
{
public:
    explicit Scheduler(enu_scheduler Type);

    static Scheduler fromString(const QString& qstrName);

    static QStringList getAllNames();
    static std::vector<enu_scheduler> getAllValues();

    static std::string getClassName();

    QString toString() const;

private:
    enu_scheduler m_Type;
};


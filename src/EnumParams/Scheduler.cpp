#include "Scheduler.h"

Scheduler::Scheduler(enu_scheduler Type)
    : m_Type{Type}
{

}

Scheduler Scheduler::fromString(const QString& qstrName)
{
    if ("Linear" == qstrName)
        return Scheduler{enu_scheduler::linear};
    else if ("Exponential" == qstrName)
        return Scheduler{enu_scheduler::exponential};
    else if ("MultiStep" == qstrName)
        return Scheduler{enu_scheduler::multistep};
    else
        throw std::string{"Invalid func name"};
}

QStringList Scheduler::getAllNames()
{
    QStringList NamesList;
    for (auto iFunc = 0; iFunc < static_cast<int>(enu_scheduler::cnt); ++iFunc)
    {
        NamesList.append(Scheduler{static_cast<enu_scheduler>(iFunc)}.toString());
    }
    return NamesList;
}

std::vector<enu_scheduler> Scheduler::getAllValues()
{
    std::vector<enu_scheduler> vRes;
    for (auto iFunc = 0; iFunc < static_cast<int>(enu_scheduler::cnt); ++iFunc)
    {
        vRes.push_back(static_cast<enu_scheduler>(iFunc));
    }

    return vRes;
}

std::string Scheduler::getClassName()
{
    return "Scheduler";
}

QString Scheduler::toString() const
{
    if (enu_scheduler::linear == m_Type)
        return "Linear";
    else if (enu_scheduler::exponential == m_Type)
        return "Exponential";
    if (enu_scheduler::multistep == m_Type)
        return "MultiStep";
    else
        return "Linear";
}

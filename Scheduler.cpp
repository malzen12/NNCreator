#include "Scheduler.h"

Scheduler::Scheduler(enu_scheduler Type)
    : m_Type{Type}
{

}

Scheduler Scheduler::fromString(const QString& qstrName)
{
    if ("Relu" == qstrName)
        return Scheduler{enu_scheduler::relu};
    else if ("LeakyRelu" == qstrName)
        return Scheduler{enu_scheduler::leaky_relu};
    else if ("Sigmoid" == qstrName)
        return Scheduler{enu_scheduler::sigmoid};
    else if ("Tanh" == qstrName)
        return Scheduler{enu_scheduler::tanh};
    else if ("Selu" == qstrName)
        return Scheduler{enu_scheduler::selu};
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
    return "Otimizer";
}

QString Scheduler::toString() const
{
    if (enu_scheduler::relu == m_Type)
        return "Relu";
    else if (enu_scheduler::leaky_relu == m_Type)
        return "LeakyRelu";
    if (enu_scheduler::sigmoid == m_Type)
        return "Sigmoid";
    else if (enu_scheduler::tanh == m_Type)
        return "Tanh";
    if (enu_scheduler::selu == m_Type)
        return "Selu";
    else
        return "";
}

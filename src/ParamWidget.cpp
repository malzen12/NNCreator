#include "ParamWidget.h"

#include <QHBoxLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QDoubleSpinBox>

#include "ActivationFunc.h"
#include "InitializerFunc.h"
#include "Normalization.h"
#include "Dropout.h"
#include "Optimizer.h"
#include "Scheduler.h"

QStringList get_enum_names(const NNParam& crParam)
{
    if (ActivationFunc::getClassName() == crParam.getName())
        return ActivationFunc::getAllNames();

    else if (InitializerFunc::getClassName() == crParam.getName())
        return InitializerFunc::getAllNames();

    else if (Normalization::getClassName() == crParam.getName())
        return Normalization::getAllNames();

    else if (Dropout::getClassName() == crParam.getName())
        return Dropout::getAllNames();

    else if (Optimizer::getClassName() == crParam.getName())
        return Optimizer::getAllNames();

    else if (Scheduler::getClassName() == crParam.getName())
        return Scheduler::getAllNames();

    else
        return {};
}


ParamWidget::ParamWidget(const NNParam& crParam)
    : m_Param{crParam},
      m_pNameLabel{new QLabel{QString::fromStdString(m_Param.getName())}}
{
    initGUI();
}

const std::string& ParamWidget::getName() const noexcept
{
    return m_Param.getName();
}

const NNParam& ParamWidget::collectValue()
{
    if (m_Param.isEnum())
    {
        m_Param.setValue(static_cast<QComboBox*>(m_pEditorWidget)->currentText());
    }
    else if (m_Param.getType() == QVariant::Type::String)
    {
        m_Param.setValue(static_cast<QLineEdit*>(m_pEditorWidget)->text());
    }
    else if (m_Param.getType() == QVariant::Type::UInt)
    {
        m_Param.setValue(static_cast<QSpinBox*>(m_pEditorWidget)->value());
    }
    else if (m_Param.getType() == QVariant::Type::Double)
    {
        m_Param.setValue(static_cast<QDoubleSpinBox*>(m_pEditorWidget)->value());
    }
    else if (m_Param.getType() == QVariant::Type::List)
    {
        auto StringList = static_cast<QLineEdit*>(m_pEditorWidget)->text().split(".");

        QList<QVariant> lRes;

        for (const auto& qstrAxis : StringList)
        {
            auto sAxis = qstrAxis.toUInt();
            lRes.push_back(sAxis);
        }

        m_Param.setValue(lRes);
    }

    return m_Param;
}

void ParamWidget::initGUI()
{
    if (m_Param.isEnum())
    {
        auto pWdg = new QComboBox;
        pWdg->addItems(get_enum_names(m_Param));
        m_pEditorWidget = pWdg;
    }
    else if (m_Param.getType() == QVariant::Type::String)
    {
        auto pWdg = new QLineEdit;
        pWdg->setText(m_Param.getValue().toString());
        m_pEditorWidget = pWdg;
    }
    else if (m_Param.getType() == QVariant::Type::UInt)
    {
        auto pWdg = new QSpinBox;
        pWdg->setMaximum(5000);
        pWdg->setValue(m_Param.getValue().toUInt());
        m_pEditorWidget = pWdg;
    }
    else if (m_Param.getType() == QVariant::Type::Double)
    {
        auto pWdg = new QDoubleSpinBox;
        pWdg->setDecimals(3);
        pWdg->setRange(0.0001, 2);
        pWdg->setSingleStep(0.001);
        pWdg->setValue(m_Param.getValue().toDouble());
        m_pEditorWidget = pWdg;
    }
    else if (m_Param.getType() == QVariant::Type::List)
    {
        auto pWdg = new QLineEdit;
//        pWdg->setInputMask("999.999.999.999");

        QString Res;
        auto List = m_Param.getValue().toList();
        for (const auto& crVal : List)
        {
            auto sVal = crVal.toUInt();
            Res += QString::number(sVal) + ".";
        }
        pWdg->setText(Res);
        m_pEditorWidget = pWdg;
    }

    auto pLayout = new QHBoxLayout{this};

    pLayout->addWidget(m_pNameLabel);
    pLayout->addWidget(m_pEditorWidget);

    setAttribute(Qt::WA_DeleteOnClose);
}

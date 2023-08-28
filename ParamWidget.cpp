#include "ParamWidget.h"

#include <QHBoxLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>

#include "ActivationFunc.h"
#include "InitializerFunc.h"

QStringList get_enum_names(const NNParam& crParam)
{
    if (ActivationFunc::getClassName() == crParam.getName())
        return ActivationFunc::getAllNames();

    else if (InitializerFunc::getClassName() == crParam.getName())
        return InitializerFunc::getAllNames();

    else
        return {};
}


ParamWidget::ParamWidget(const NNParam& crParam)
    : m_Param{crParam},
      m_pNameLabel{new QLabel{QString::fromStdString(m_Param.getName())}}
{
    initGUI();
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
    else if (m_Param.getType() == QVariant::Type::Int)
    {
        m_Param.setValue(static_cast<QSpinBox*>(m_pEditorWidget)->value());
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
    else if (m_Param.getType() == QVariant::Type::Int)
    {
        auto pWdg = new QSpinBox;
        pWdg->setValue(m_Param.getValue().toUInt());
        m_pEditorWidget = pWdg;
    }


    auto pLayout = new QHBoxLayout{this};

    pLayout->addWidget(m_pNameLabel);
    pLayout->addWidget(m_pEditorWidget);

    setAttribute(Qt::WA_DeleteOnClose);
}

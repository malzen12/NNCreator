#include "ParamWidget.h"

#include <QHBoxLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QCheckBox>

#include "ActivationFunc.h"
#include "InitializerFunc.h"
#include "Normalization.h"
#include "Dropout.h"
#include "Optimizer.h"
#include "Scheduler.h"
#include "Recurrent.h"
#include "PaddingMode.h"

QStringList get_enum_names(const std::string& strParamName)
{
  if (ActivationFunc::getClassName() == strParamName)
    return ActivationFunc::getAllNames();

  else if (InitializerFunc::getClassName() == strParamName)
    return InitializerFunc::getAllNames();

  else if (Normalization::getClassName() == strParamName)
    return Normalization::getAllNames();

  else if (Dropout::getClassName() == strParamName)
    return Dropout::getAllNames();

  else if (Optimizer::getClassName() == strParamName)
    return Optimizer::getAllNames();

  else if (Scheduler::getClassName() == strParamName)
    return Scheduler::getAllNames();

  else if (Recurrent::getClassName() == strParamName)
    return Recurrent::getAllNames();

  else if (PaddingMode::getClassName() == strParamName)
    return PaddingMode::getAllNames();

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
  if (m_Param.isEnum()){
    m_Param.setValue(static_cast<QComboBox*>(m_pEditorWidget)->currentText());
  }else{
    switch(m_Param.getType()){
      case QVariant::Type::String:
        m_Param.setValue(static_cast<QLineEdit*>(m_pEditorWidget)->text());
        break;
      case QVariant::Type::UInt:
        m_Param.setValue(static_cast<QSpinBox*>(m_pEditorWidget)->value());
        break;
      case QVariant::Type::Double:
        m_Param.setValue(static_cast<QDoubleSpinBox*>(m_pEditorWidget)->value());
        break;
      case QVariant::Type::Bool:
        m_Param.setValue(static_cast<QCheckBox*>(m_pEditorWidget)->isChecked());
        break;
      case QVariant::Type::List:{
        auto StringList = static_cast<QLineEdit*>(m_pEditorWidget)->text().split(".");

        QList<QVariant> lRes;
        lRes.reserve(StringList.size());

        for (const auto& qstrAxis : StringList){
          lRes.push_back(qstrAxis.toUInt());
        }

        m_Param.setValue(lRes);
        break;
      }
      default:
        throw std::runtime_error("unknowed type");
    }
  }

  return m_Param;
}

void ParamWidget::initGUI()
{
  if (m_Param.isEnum()){
    auto pWdg = new QComboBox;
    pWdg->addItems(get_enum_names(m_Param.getHiddenName()));
    m_pEditorWidget = pWdg;
  }else{
    switch(m_Param.getType()){
      case QVariant::Type::String:{
        auto pWdg = new QLineEdit;
        pWdg->setText(m_Param.getValue().toString());
        m_pEditorWidget = pWdg;
        break;
      }
      case QVariant::Type::UInt:{
        auto pWdg = new QSpinBox;
        auto sMax = static_cast<std::size_t>(std::numeric_limits<int>::max());
        pWdg->setMaximum(sMax);
        pWdg->setValue(m_Param.getValue().toUInt());
        m_pEditorWidget = pWdg;
        break;
      }
      case QVariant::Type::Double:{
        auto pWdg = new QDoubleSpinBox;
        pWdg->setDecimals(3);
        pWdg->setRange(0.0001, 2);
        pWdg->setSingleStep(0.001);
        pWdg->setValue(m_Param.getValue().toDouble());
        m_pEditorWidget = pWdg;
        break;
      }
      case QVariant::Type::Bool:{
        auto pWdg = new QCheckBox;
        pWdg->setChecked(m_Param.getValue().toBool());
        m_pEditorWidget = pWdg;
        break;
      }
      case QVariant::Type::List:{
        auto pWdg = new QLineEdit;
        //        pWdg->setInputMask("999.999.999.999");

        QString Res = m_Param.convertToQString(".");
        pWdg->setText(Res);
        m_pEditorWidget = pWdg;

        break;
      }
      default:
        throw std::runtime_error("unknowed type");
    }
  }

  auto pLayout = new QHBoxLayout{this};

  pLayout->addWidget(m_pNameLabel);
  pLayout->addWidget(m_pEditorWidget);

  pLayout->setContentsMargins(0,0,0,0);

  setAttribute(Qt::WA_DeleteOnClose);
}

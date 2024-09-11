#include "ParamWidget.h"
#include "ActivationFunc.h"
#include "Dropout.h"
#include "InitializerFunc.h"
#include "Normalization.h"
#include "Optimizer.h"
#include "PaddingMode.h"
#include "Recurrent.h"
#include "Scheduler.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QSpinBox>

QStringList get_enum_names(const std::string& strParamName) {
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
    : m_Param{crParam}
    , m_pNameLabel{new QLabel{QString::fromStdString(m_Param.getName())}} {
  initGUI();
}

auto ParamWidget::getName() const noexcept -> const std::string& { return m_Param.getName(); }

auto ParamWidget::collectValue() -> const NNParam& {
  if (m_Param.isEnum()) {
    m_Param.setValue(static_cast<QComboBox*>(m_pEditorWidget)->currentText());
  } else {
    switch (m_Param.getType()) {
      case QVariant::Type::String : collectQString(); break;
      case QVariant::Type::UInt   : collectUInt(); break;
      case QVariant::Type::Double : collectDouble(); break;
      case QVariant::Type::Bool   : collectBool(); break;
      case QVariant::Type::List   : collectQList(); break;
      default                     : throw std::runtime_error("unknowed type");
    }
  }
  return m_Param;
}

void ParamWidget::initGUI() {
  if (m_Param.isEnum()) {
    initGuiEnum();
  } else {
    switch (m_Param.getType()) {
      case QVariant::Type::String : initGuiString(); break;
      case QVariant::Type::UInt   : initGuiUint(); break;
      case QVariant::Type::Double : initGuiDouble(); break;
      case QVariant::Type::Bool   : initGuiBool(); break;
      case QVariant::Type::List   : initGuiQList(); break;
      default                     : throw std::runtime_error("unknowed type");
    }
  }
  auto pLayout = new QHBoxLayout{this};
  pLayout->addWidget(m_pNameLabel);
  pLayout->addWidget(m_pEditorWidget);

  pLayout->setContentsMargins(0, 0, 0, 0);

  setAttribute(Qt::WA_DeleteOnClose);
}

void ParamWidget::initGuiEnum() {
  auto pWidget = new QComboBox;
  pWidget->addItems(get_enum_names(m_Param.getHiddenName()));
  m_pEditorWidget = pWidget;
}

void ParamWidget::initGuiString() {
  auto pWidget = new QLineEdit;
  pWidget->setText(m_Param.convertToQString());
  m_pEditorWidget = pWidget;
}

void ParamWidget::initGuiUint() {
  auto pWidget = new QSpinBox;
  auto sMax = static_cast<std::size_t>(std::numeric_limits<int>::max());
  pWidget->setMaximum(sMax);
  pWidget->setValue(m_Param.getValue().toUInt());
  m_pEditorWidget = pWidget;
}

void ParamWidget::initGuiDouble() {
  auto pWidget = new QDoubleSpinBox;
  pWidget->setDecimals(3);
  pWidget->setRange(0.0001, 2);
  pWidget->setSingleStep(0.001);
  pWidget->setValue(m_Param.getValue().toDouble());
  m_pEditorWidget = pWidget;
}

void ParamWidget::initGuiBool() {
  auto pWidget = new QCheckBox;
  pWidget->setChecked(m_Param.getValue().toBool());
  m_pEditorWidget = pWidget;
}

void ParamWidget::initGuiQList() {
  auto pWidget = new QLineEdit;
  QString Res = m_Param.convertToQString(", ");
  pWidget->setText(Res);
  m_pEditorWidget = pWidget;
}

void ParamWidget::collectQString() { m_Param.setValue(static_cast<QLineEdit*>(m_pEditorWidget)->text()); }

void ParamWidget::collectUInt() { m_Param.setValue(static_cast<QSpinBox*>(m_pEditorWidget)->value()); }

void ParamWidget::collectDouble() {
  m_Param.setValue(static_cast<QDoubleSpinBox*>(m_pEditorWidget)->value());
}

void ParamWidget::collectBool() { m_Param.setValue(static_cast<QCheckBox*>(m_pEditorWidget)->isChecked()); }

void ParamWidget::collectQList() {
  const auto string = static_cast<QLineEdit*>(m_pEditorWidget)->text();
  const auto stringList = string.split(QRegularExpression("\\W+"), Qt::SkipEmptyParts);
  QList<QVariant> lRes;
  lRes.reserve(stringList.size());
  for (const auto& qstrAxis : stringList) lRes.push_back(qstrAxis.toUInt());
  m_Param.setValue(lRes);
}

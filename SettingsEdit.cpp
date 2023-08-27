#include "SettingsEdit.h"

#include <QVBoxLayout>

SettingsEdit::SettingsEdit()
    : m_pNameEdit{new QLineEdit},
      m_pNeuronCntEdit{new QSpinBox},
      m_pAFuncEdit{new QComboBox},
      m_pConfirm{new QPushButton}
{
    initGUI();
    createConnections();
}

void SettingsEdit::setLayerToEdit(NNLayerWidget* pLayer)
{
    m_pLayerToEdit = pLayer;

    m_Params = m_pLayerToEdit->getSettings();

    initEditors();
}

void SettingsEdit::onChangeName()
{
    if (m_pLayerToEdit)
        m_Params.setName(m_pNameEdit->text().toStdString());
}

void SettingsEdit::onChangeNCnt(int iCnt)
{
//    if (m_pLayerToEdit)
//        m_Params.setNCnt(iCnt);
}

void SettingsEdit::onSetAFunc(const QString& qstrFuncName)
{
//    if (m_pLayerToEdit)
//        m_Params.setAFunc(ActivationFunc::fromString(qstrFuncName));
}

void SettingsEdit::initGUI()
{
    auto qstrRelu = ActivationFunc{ActivationFunc::activation_func::relu}.toString();
    auto qstrLeakyRelu = ActivationFunc{ActivationFunc::activation_func::leaky_relu}.toString();

    m_pAFuncEdit->addItem(qstrRelu);
    m_pAFuncEdit->addItem(qstrLeakyRelu);

    auto pLayout = new QVBoxLayout{this};

    pLayout->addWidget(m_pNameEdit);
    pLayout->addWidget(m_pNeuronCntEdit);
    pLayout->addWidget(m_pAFuncEdit);
    pLayout->addWidget(m_pConfirm);
}

void SettingsEdit::createConnections()
{
    auto bRes = true;

    bRes &= static_cast<bool>(connect(m_pNameEdit, SIGNAL(editingFinished()), SLOT(onChangeName())));
    bRes &= static_cast<bool>(connect(m_pNeuronCntEdit, SIGNAL(valueChanged(int)), SLOT(onChangeNCnt(int))));
    bRes &= static_cast<bool>(connect(m_pAFuncEdit, SIGNAL(activated(QString)), SLOT(onSetAFunc(QString))));

    assert(bRes);
}

void SettingsEdit::initEditors()
{
//    m_pNameEdit->setText(QString::fromStdString(m_Params.getName()));
//    m_pNeuronCntEdit->setValue(m_Params.getNCnt());

//    auto iComboBoxIndex = m_pAFuncEdit->findText(m_Params.getAFunc().toString());
//    m_pAFuncEdit->setCurrentIndex(iComboBoxIndex);
}

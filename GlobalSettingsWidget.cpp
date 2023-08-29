#include "GlobalSettingsWidget.h"

#include <QVBoxLayout>

GlobalSettingsWidget::GlobalSettingsWidget()
    : //m_pSavePathEdit{new QLineEdit},
      //m_pConfirmPathButton{new QPushButton{"Confirm"}},
      m_pInputSizeEdit{new QLineEdit},
      m_pConfirmSizeButton{new QPushButton{"Confirm"}},
      m_pMakeXmlButton{new QPushButton{"Make xml"}}
{
    initGUI();
    createConnections();

}

void GlobalSettingsWidget::onUpdateInputSize()
{
    auto StringList = m_pInputSizeEdit->text().split(".");

    m_vInputSize.clear();

    for (const auto& qstrAxis : StringList)
    {
        auto sAxis = qstrAxis.toUInt();
        if (sAxis)
            m_vInputSize.push_back(sAxis);
    }

    emit inputSizeChanged(m_vInputSize);
}


void GlobalSettingsWidget::initGUI()
{
//    m_pInputSizeEdit->setInputMask("999.999.999.999");

    auto pLayout = new QVBoxLayout{this};

//    pLayout->addWidget(m_pSavePathEdit);
//    pLayout->addWidget(m_pConfirmPathButton);
    pLayout->addWidget(m_pInputSizeEdit);
    pLayout->addWidget(m_pConfirmSizeButton);
    pLayout->addWidget(m_pMakeXmlButton);
}

void GlobalSettingsWidget::createConnections()
{
    bool bRes = true;

    bRes &= static_cast<bool>(connect(m_pMakeXmlButton, SIGNAL(clicked(bool)), SIGNAL(makeXml())));
    bRes &= static_cast<bool>(connect(m_pInputSizeEdit, SIGNAL(editingFinished()), SLOT(onUpdateInputSize())));
    bRes &= static_cast<bool>(connect(m_pConfirmSizeButton, SIGNAL(clicked(bool)), SLOT(onUpdateInputSize())));

    assert(bRes);
}

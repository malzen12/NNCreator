#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>

#include "LayerParams/NNParam.h"

class GlobalSettingsWidget: public QWidget
{
    Q_OBJECT

public:
    GlobalSettingsWidget();

signals:
    void inputSizeChanged(const std::vector<std::size_t>& );
    void makeNetXml(const std::vector<NNParam>& );
    void makeTrainXml(const std::vector<NNParam>& );

private slots:
    void onUpdateInputSize();
    void onMakeNetXml();
    void onMakeTrainXml();

private:
    void initGUI();
    void createConnections();

    std::vector<NNParam> collectParams(QGridLayout* pLayout);
    NNParam collectInputSize();

    QPushButton* m_pConfirmSizeButton;
    QGridLayout* m_pNetSettingsLayout;
    QPushButton* m_pMakeNetXmlButton;

    QGridLayout* m_pTrainSettingsLayout;
    QPushButton* m_pMakeTrainXmlButton;
};


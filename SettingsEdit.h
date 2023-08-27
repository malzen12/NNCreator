#pragma once

#include <QWidget>
#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>

#include "NNLayerWidget.h"
#include "ActivationFunc.h"

class SettingsEdit: public QWidget
{
    Q_OBJECT

public:
    SettingsEdit();

    void setLayerToEdit(NNLayerWidget* pLayer);

private slots:
    void onChangeName();
    void onChangeNCnt(int iCnt);
    void onSetAFunc(const QString& qstrFuncName);

private:
    void initGUI();
    void createConnections();
    void initEditors();

    QLineEdit* m_pNameEdit;
    QSpinBox* m_pNeuronCntEdit;
    QComboBox* m_pAFuncEdit;
    QPushButton* m_pConfirm;

    NNLayerParams m_Params;

    NNLayerWidget* m_pLayerToEdit;
};

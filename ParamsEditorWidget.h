#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

#include "NNLayerWidget.h"
#include "ActivationFunc.h"

class ParamsEditorWidget: public QWidget
{
    Q_OBJECT

public:
    ParamsEditorWidget();

public slots:
    void onSetParams(const NNLayerParams& crParams);

signals:
    void paramsChanged(const NNLayerParams& crParams);
    void deleteActive();

private slots:
    void onConfirmParams();

private:
    void initGUI();
    void createConnections();

    void initEditors();
    const NNLayerParams& collectParams();


    QLabel* m_pNameLabel;

    QVBoxLayout* m_pParamsLayout;

    QPushButton* m_pConfirm;
    QPushButton* m_pDelete;

    NNLayerParams m_Params;
};

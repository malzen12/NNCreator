#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

#include "NNLayerWidget.h"

class ParamsEditorWidget: public QWidget
{
    Q_OBJECT

public:
    ParamsEditorWidget();

public slots:
    void onSetParams(const std::shared_ptr<NNLayerParams>& spParams);

signals:
    void paramsChanged(const std::shared_ptr<NNLayerParams>& crParams);
    void deleteActive();

private slots:
    void onConfirmParams();

private:
    void initGUI();
    void createConnections();

    void initEditors();
    const std::shared_ptr<NNLayerParams>& collectParams();


    QLabel* m_pNameLabel;

    QVBoxLayout* m_pParamsLayout;

    QPushButton* m_pConfirm;
    QPushButton* m_pDelete;

    std::shared_ptr<NNLayerParams> m_spParams;
};

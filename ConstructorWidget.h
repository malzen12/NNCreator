#pragma once

#include <QWidget>
#include <QMenu>

#include "NNLayerWidget.h"

class ConstructorWidget: public QWidget
{
    Q_OBJECT

public:
    ConstructorWidget();

signals:
    void settingsChanged(NNLayerParams);

public slots:
    void onSetSettings(std::size_t sId, const NNLayerParams& crSettings);
    void onDeleteLayer(NNLayerWidget* pLayer);

private slots:
    void onAddLayer(const QPoint& crPoint, const NNLayerParams& crParams);
    void onProcActions(QAction* /*pAction*/);
    void onChangeActive(std::size_t sId);
    void onMakeForward(std::size_t sId);

private:
    void initGUI();
    void createConnections();

    void makeActive(std::size_t sId, bool bActive);

    void mousePressEvent(QMouseEvent* pEvent) final;
    void mouseMoveEvent(QMouseEvent* pEvent) final;
    void paintEvent(QPaintEvent* pEvent) final;

private:
    QMenu* m_pMenu;
    std::vector<NNLayerWidget*> m_vLayers;

    std::size_t m_sActive;
};


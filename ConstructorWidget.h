#pragma once

#include <QWidget>
#include <QMenu>

#include "NNLayerWidget.h"

class ConstructorWidget: public QWidget
{
    Q_OBJECT

public:
    ConstructorWidget();

    void mousePressEvent(QMouseEvent* pEvent) final;
    void mouseMoveEvent(QMouseEvent* pEvent) final;

private slots:
    void onAddLayer(const QPoint& crPoint);
    void onProcActions(QAction* /*pAction*/);
    void onDeleteLayer(int iId);

private:
    void initGUI();
    void createConnections();

private:
    QMenu* m_pMenu;
    std::vector<NNLayerWidget*> m_vLayers;
};


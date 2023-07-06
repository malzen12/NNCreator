#pragma once

#include <QPushButton>

#include "NNLayerSettings.h"

class NNLayerWidget: public QWidget
{
    Q_OBJECT

public:
    explicit NNLayerWidget(int iId);

    int getId() const;

    void setSettings(const NNLayerSettings& crSettings) noexcept;
    const NNLayerSettings& getSettings() const noexcept;

    void deleteLayer();

    bool isGrabbed() const;

    void mousePressEvent(QMouseEvent* pEvent) final;
    void mouseReleaseEvent(QMouseEvent* pEvent) final;

signals:
    void layerDeleted(int);

private slots:
    void onProcActions(QAction* pAction);

private:
    void initGUI();
    void createConnections();

    QMenu* m_pMenu;

    int m_iId;
    bool m_bGrabbed;

    NNLayerSettings m_Settings;
};

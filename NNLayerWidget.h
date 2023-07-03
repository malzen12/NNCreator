#pragma once

#include <QPushButton>

class NNLayerWidget: public QWidget
{
    Q_OBJECT

public:
    explicit NNLayerWidget(int iId);

    int getId() const;

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
};

#pragma once

#include <QPushButton>

class NNLayerWidget: public QPushButton
{
    Q_OBJECT

public:
    explicit NNLayerWidget(int iId);

    int getId() const;

    void deleteLayer();

signals:
    void layerDeleted(int);

private slots:
    void onProcActions(QAction* pAction);

private:
    void initGUI();
    void createConnections();

    QMenu* m_pMenu;
    int m_iId;
};

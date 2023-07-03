#include "NNLayerWidget.h"

#include <iostream>

#include <QMenu>

static const QString c_qstrDeleteActionName = "delete layer";

NNLayerWidget::NNLayerWidget(int iId)
    : m_pMenu{new QMenu{this}},
      m_iId{iId}
{
    setAttribute(Qt::WA_DeleteOnClose);

    initGUI();
    createConnections();
}

int NNLayerWidget::getId() const
{
    return m_iId;
}

void NNLayerWidget::deleteLayer()
{
    close();
    emit layerDeleted(getId());
}

void NNLayerWidget::onProcActions(QAction* pAction)
{
    if (c_qstrDeleteActionName == pAction->text())
    {
        deleteLayer();
    }
}

void NNLayerWidget::initGUI()
{
    setFlat(true);

    setFixedSize(100, 20);

    auto Palette = palette();
    Palette.setColor(QPalette::Window, QColor{125, 125, 255});
    Palette.setColor(QPalette::Button, QColor{125, 125, 255});
    setPalette(Palette);

    setAutoFillBackground(true);

    setContextMenuPolicy(Qt::CustomContextMenu);

    m_pMenu->addAction(new QAction{c_qstrDeleteActionName});

    setMenu(m_pMenu);
}

void NNLayerWidget::createConnections()
{
    auto bRes = true;

    bRes = static_cast<bool>(connect(m_pMenu, SIGNAL(triggered(QAction*)), SLOT(onProcActions(QAction*))));

    assert(bRes);
}

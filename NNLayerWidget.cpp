#include "NNLayerWidget.h"

#include <iostream>

#include <QMenu>
#include <QMouseEvent>

static const QString c_qstrDeleteActionName = "delete layer";

NNLayerWidget::NNLayerWidget(int iId)
    : m_pMenu{new QMenu{this}},
      m_iId{iId},
      m_bGrabbed{false}
{
    setAttribute(Qt::WA_DeleteOnClose);

    initGUI();
    createConnections();
}

int NNLayerWidget::getId() const
{
    return m_iId;
}

void NNLayerWidget::setSettings(const NNLayerSettings& crSettings) noexcept
{
    m_Settings = crSettings;
}

const NNLayerSettings& NNLayerWidget::getSettings() const noexcept
{
    return m_Settings;
}

void NNLayerWidget::deleteLayer()
{
    close();
    emit layerDeleted(getId());
}

bool NNLayerWidget::isGrabbed() const
{
    return m_bGrabbed;
}

void NNLayerWidget::mousePressEvent(QMouseEvent* pEvent)
{
    if (pEvent->button() == Qt::RightButton)
    {
        auto Pos = mapToGlobal(pEvent->pos());

        m_pMenu->popup(Pos);
    }
    if (pEvent->button() == Qt::LeftButton)
    {
        m_bGrabbed = true;
    }
}

void NNLayerWidget::mouseReleaseEvent(QMouseEvent* pEvent)
{
    m_bGrabbed = false;
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
    setFixedSize(100, 20);

    auto Palette = palette();
    Palette.setColor(QPalette::Window, QColor{125, 125, 255});
    Palette.setColor(QPalette::Button, QColor{125, 125, 255});
    setPalette(Palette);

    setAutoFillBackground(true);

    setContextMenuPolicy(Qt::CustomContextMenu);

    m_pMenu->addAction(new QAction{c_qstrDeleteActionName});
}

void NNLayerWidget::createConnections()
{
    auto bRes = true;

    bRes = static_cast<bool>(connect(m_pMenu, SIGNAL(triggered(QAction*)), SLOT(onProcActions(QAction*))));

    assert(bRes);
}

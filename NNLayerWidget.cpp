#include "NNLayerWidget.h"

#include <iostream>

#include <QMenu>
#include <QMouseEvent>

static const QString c_qstrDeleteActionName = "delete layer";

NNLayerWidget::NNLayerWidget(std::size_t sId, const NNLayerParams& crParams)
    : m_sId{sId},
      m_bGrabbed{false},
      m_Params{crParams},
      m_bValidParams{true}
{
    setAttribute(Qt::WA_DeleteOnClose);

    initGUI();
}

std::size_t NNLayerWidget::getId() const
{
    return m_sId;
}

void NNLayerWidget::setParams(const NNLayerParams& crParams) noexcept
{
    m_Params = crParams;
}

const NNLayerParams& NNLayerWidget::getParams() const noexcept
{
    return m_Params;
}

void NNLayerWidget::deleteLayer()
{
    close();
}

bool NNLayerWidget::isGrabbed() const
{
    return m_bGrabbed;
}

void NNLayerWidget::mousePressEvent(QMouseEvent* pEvent)
{
    if (pEvent->button() == Qt::LeftButton)
    {
        m_bGrabbed = true;
        m_GrabbedPos = pEvent->pos();
        makeActive(true);
    }
    else if (pEvent->button() == Qt::RightButton)
    {
        emit makeForward(m_sId);
    }
}

void NNLayerWidget::mouseReleaseEvent(QMouseEvent* /*pEvent*/)
{
    m_bGrabbed = false;
}

void NNLayerWidget::makeActive(bool bActive)
{
    m_bActive = bActive;
    if (m_bActive)
        emit becomeActive(m_sId);

    updateStyle();
}

const QPoint& NNLayerWidget::getGrabbedPos() const noexcept
{
    return m_GrabbedPos;
}

void NNLayerWidget::addForward(NNLayerWidget* pForward)
{
    auto it = std::find(m_vForwards.begin(), m_vForwards.end(), pForward);
    if (it != m_vForwards.end())
        return ;

    m_vForwards.push_back(pForward);
}

void NNLayerWidget::removeForward(NNLayerWidget* pForward)
{
    auto itForward = std::find(m_vForwards.begin(), m_vForwards.end(), pForward);

    if (itForward != m_vForwards.end())
        m_vForwards.erase(itForward);
}

const std::vector<NNLayerWidget*>& NNLayerWidget::getForward() const noexcept
{
    return m_vForwards;
}

void NNLayerWidget::resetInputSize()
{
    m_vInputSize.clear();
}

void NNLayerWidget::addInputSize(const std::vector<std::size_t>& vInputSize) ///< @todo
{
    m_vInputSize = vInputSize;
    m_bValidParams = m_Params.checkInputSize(m_vInputSize);
    updateStyle();
}

std::vector<std::size_t> NNLayerWidget::calcOutputSize() const
{
    return m_Params.calcOutputSize(m_vInputSize);
}

void NNLayerWidget::initGUI()
{
    setFixedSize(100, 20);

    setAutoFillBackground(true);

    setContextMenuPolicy(Qt::CustomContextMenu);

    makeActive(true);
}

QColor make_color(bool bActive, bool bValid)
{
    if (bValid)
        return bActive?  QColor{125, 255, 125} : QColor{125, 125, 255};
    else
        return bActive?  QColor{200, 200, 125} : QColor{255, 125, 125};
}

void NNLayerWidget::updateStyle()
{
    auto Color = make_color(m_bActive, m_bValidParams);

    auto Palette = palette();
    Palette.setColor(QPalette::Window, Color);
    Palette.setColor(QPalette::Button, Color);
    setPalette(Palette);
}

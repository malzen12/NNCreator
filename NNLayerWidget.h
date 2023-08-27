#pragma once

#include <QPushButton>

#include "NNLayerParams.h"

class NNLayerWidget: public QWidget
{
    Q_OBJECT

public:
    explicit NNLayerWidget(std::size_t sId, const NNLayerParams& crParams);

    std::size_t getId() const;

    void setParams(const NNLayerParams& crParams) noexcept;
    const NNLayerParams& getParams() const noexcept;

    void deleteLayer();

    bool isGrabbed() const;

    void mousePressEvent(QMouseEvent* pEvent) final;
    void mouseReleaseEvent(QMouseEvent* pEvent) final;

    void makeActive(bool bActive);

    const QPoint& getGrabbedPos() const noexcept;

    void addForward(NNLayerWidget* pForward);
    void removeForward(NNLayerWidget* pForward);
    const std::vector<NNLayerWidget*>& getForward() const noexcept;

    void resetInputSize();
    void addInputSize(const std::vector<std::size_t>& vInputSize);
    std::vector<std::size_t> calcOutputSize() const;

signals:
    void becomeActive(std::size_t);
    void makeForward(std::size_t);

private:
    void initGUI();
    void updateStyle();

    std::size_t m_sId;
    bool m_bGrabbed;
    QPoint m_GrabbedPos;

    bool m_bActive;

    NNLayerParams m_Params;

    std::vector<std::size_t> m_vInputSize;
    bool m_bValidParams;

    std::vector<NNLayerWidget*> m_vForwards;
};

#pragma once

#include <QWidget>
#include <QMenu>

#include "NNLayerWidget.h"

class ConstructorWidget: public QWidget
{
    Q_OBJECT

    using bfs_proc = std::function<void(NNLayerWidget*, NNLayerWidget*)>;

public:
    ConstructorWidget();

    std::string makeXmlString() const;
    std::string makePyString() const;
    bool isEmpty() const noexcept;

signals:
    void paramsChanged(const std::shared_ptr<NNLayerParams>&);

public slots:
    void onSetParams(const std::shared_ptr<NNLayerParams>& spParams);
    void onDeleteActive();

    void onSetInputSize(const std::vector<std::size_t>& vInputSize);

    void onSetOutputPath(const QString& qstrPath);

private slots:
    void onAddLayer(const QPoint& crPoint, const std::shared_ptr<NNLayerParams>& spParams);
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

    void checkSizes();
    NNLayerWidget* findStart() const;

    void bfs(NNLayerWidget* pStart, bfs_proc fCurrentProc, bfs_proc fForwardsProc) const;

private:
    std::string m_strPath;

    QMenu* m_pMenu;
    std::vector<NNLayerWidget*> m_vLayers;

    std::size_t m_sActive;
    std::vector<std::size_t> m_vInputSize;
};


#include <QEvent>
#include <QMouseEvent>
#include <QPainter>

#include "ConstructorWorkSpace.h"


#include <iostream>
ConstructorWorkSpace::ConstructorWorkSpace(const std::unordered_map<std::size_t, NNLayerWidget*>& vLayers, const std::unordered_set<std::size_t>& ActiveSet, QMenu* pMenu)
  : m_refvLayers(vLayers),
    m_refActiveSet(ActiveSet),
    m_pMenu(pMenu)
{
  initGUI();
}

void ConstructorWorkSpace::initGUI()
{

  setObjectName("ConstructorWorkSpace");
  this->setStyleSheet("QWidget#ConstructorWorkSpace{background-color: yellow}");

}

void ConstructorWorkSpace::paintEvent(QPaintEvent *pEvent)
{
  QWidget::paintEvent(pEvent);

  QPainter Painter{this};
  Painter.fillRect(QRect{{0, 0}, this->size()},QBrush{Qt::lightGray});
  Painter.setPen(QPen(Qt::black, 4, Qt::SolidLine, Qt::FlatCap));

  for(const auto [_, pLayer] : m_refvLayers){
    for(const auto pForward : pLayer->getForward()){
      auto From = pLayer->pos() + QPoint{pLayer->rect().center().x(), pLayer->rect().bottom()};
      auto To = pForward->pos() + QPoint{pForward->rect().center().x(), 0};

      Painter.drawLine(From, To);
      Painter.drawEllipse(To, 5, 5);
    }
  }
}

void ConstructorWorkSpace::mousePressEvent(QMouseEvent* pEvent)
{
  if (pEvent->button() == Qt::RightButton){
    auto Pos = mapToGlobal(pEvent->pos());
    m_pMenu->popup(Pos);
  }
  QWidget::mousePressEvent(pEvent);
}

void ConstructorWorkSpace::mouseMoveEvent(QMouseEvent* pEvent)
{
  NNLayerWidget *pLayer;
  if (auto temp = std::find_if(
          m_refvLayers.begin(), m_refvLayers.end(),
          [](const auto pLayer) { return pLayer.second->isGrabbed(); });
      temp != m_refvLayers.end())
    pLayer = temp->second;
  else
    return;

  const int k_margin = 5;

  QPoint layersPos{this->size().width(), this->size().height()};
  QSize layersPosEnd{};

  for(const auto i : m_refActiveSet){
    auto pCurrent = m_refvLayers.at(i);
    layersPos.rx() = std::min(layersPos.x(), pCurrent->pos().x());
    layersPos.ry() = std::min(layersPos.y(), pCurrent->pos().y());
    layersPosEnd.rwidth() = std::max(layersPosEnd.width(), pCurrent->pos().x() + pCurrent->size().width());
    layersPosEnd.rheight() = std::max(layersPosEnd.height(), pCurrent->pos().y() + pCurrent->size().height());
  }
  QSize layersSize{layersPosEnd};
  layersSize.rwidth() -= layersPos.x();
  layersSize.rheight() -= layersPos.y();
  auto temp = layersPos - pLayer->pos();
  auto eventWithCorPos = pEvent->pos() - pLayer->getGrabbedPos() + temp ;
  QPoint shift{eventWithCorPos};

  if(eventWithCorPos.x() - k_margin < 0)
    shift.rx() = k_margin;
  if(eventWithCorPos.y() - k_margin < 0)
    shift.ry() = k_margin;
  if(shift.x() != eventWithCorPos.x() || shift.y() != eventWithCorPos.y())
    layersPos = shift;

  for(const auto i : m_refActiveSet){
    auto pCurrent = m_refvLayers.at(i);
    auto pos = pCurrent->pos();
    pCurrent->move(pos - layersPos + shift);
  }

  QPoint resize{0, 0};
  auto mainSize = this->size();
  if(auto tmp = (layersPos.x() + layersSize.width()) - mainSize.width() + k_margin; tmp > 0)
    resize.rx() = tmp;
  if(auto tmp = (layersPos.y() + layersSize.height()) - mainSize.height() + k_margin; tmp > 0)
    resize.ry() = tmp;
  if(resize.x() != 0 || resize.y() != 0){
    QPoint point = resize + QPoint{this->size().width(), this->size().height()};
    width = point.x();
    height = point.y();
    this->resize(point.x(), point.y());
  }


  update();
}

void ConstructorWorkSpace::resizeEvent(QResizeEvent*)
{
//  this->resize(width, height);
}

void ConstructorWorkSpace::add(const QPoint& crPoint, NNLayerWidget* pLayer)
{
  pLayer->setParent(this);
  pLayer->move(mapFromParent(crPoint));
  pLayer->show();
}

void ConstructorWorkSpace::setWidth(int value)
{
  width = value;
}

void ConstructorWorkSpace::setHeight(int value)
{
  height = value;
}

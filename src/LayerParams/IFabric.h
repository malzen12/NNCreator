#pragma once

#include "NNWidget.h"
#include <QObject>

class IFabric : public QObject {
  Q_OBJECT
protected:
  using KeyType = std::size_t;
  explicit IFabric(QString name);

public:
  auto getFunc(const QString& type) const -> NNWidget*;
  virtual auto createCopy(const NNWidget* pLayer, const QPoint& point) -> KeyType = 0;
  virtual auto bufferCopy(const NNWidget* pLayer) const -> NNWidget* = 0;
  auto load(const QString& subType, KeyType id = SIZE_MAX) const -> NNWidget*;
  auto create(const QString& type) const -> NNWidget*;

signals:
  void changeWidget(NNWidget*);
  void createAndConnectWidget(NNWidget*);
  void createWidget(NNWidget*);
  void createWidget(NNWidget*, QPoint);

protected:
  auto impl(const QString& subType, KeyType id = SIZE_MAX) const -> NNWidget*;

  static auto idCorrection(KeyType id = SIZE_MAX) -> KeyType;
  static auto exampl(/*const QString&, */ KeyType id = SIZE_MAX) -> NNWidget*;

protected:
  std::unordered_map<QString, decltype(&IFabric::exampl)> m_FabricMap;
  QString m_strType;
};



#pragma once

#include "IFabric.h"

class FabricManager : public QObject {
  Q_OBJECT
  using KeyType = std::size_t;

public:
  FabricManager();

  auto createCopy(const NNWidget* pLayer, const QPoint& point) -> KeyType;
  auto bufferCopy(const NNWidget* pLayer) const -> NNWidget*;
  auto load(const QString& type, const QString& subType, KeyType id) const -> NNWidget*;
  auto create(const QString& type, const QString& subtype) const -> NNWidget*;

  auto chooseFabric(const QString& type) const -> IFabric*;
  void connectFabric();

signals:
  void changeWidget(NNWidget*);
  void createAndConnectWidget(NNWidget*);
  void createWidget(NNWidget*);
  void createWidget(NNWidget*, QPoint);

private slots:
  void onCreate(const std::vector<QString>& tags);

private:
  auto createImpl(const std::vector<QString>& tags) const -> NNWidget*;
  auto impl(const QString& type, const QString& subType, KeyType id = SIZE_MAX) const -> NNWidget*;
  auto findTypeAndSubtype(const std::vector<QString>& tags) const -> std::pair<QString, QString>;

private:
  std::unordered_map<QString, IFabric*> m_fabrics;
};

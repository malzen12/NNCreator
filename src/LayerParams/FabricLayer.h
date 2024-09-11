#pragma once

#include "NNLayerWidget.h"
#include <QObject>

#define BEFORE 0

class FabricLayer : public QObject {
  Q_OBJECT

  using KeyType = std::size_t;

public:
  FabricLayer();

signals:
  void changeWidget(NNLayerWidget*);
  void createAndConnectWidget(NNLayerWidget*);
  void createWidget(NNLayerWidget*);
  void createWidget(NNLayerWidget*, QPoint);
#if BEFORE
public slots:
  auto onCreateCopy(const NNLayerWidget& layer, const QPoint& point) -> KeyType;
  void onCreate(const std::vector<QString>& tags);
#else
public:
  auto onCreateCopy(const NNLayerWidget& layer, const QPoint& point) -> KeyType;
private slots:
  void onCreate(const std::vector<QString>& tags);
#endif

private:
  auto createImpl(const std::vector<QString>& tags) -> NNLayerWidget*;
  auto Impl(const std::vector<QString>& tags) const -> std::pair<QString, QString>;

  static auto exampl(const QString& /*, bool isCopy*/) -> NNLayerWidget* { return {}; }

  static auto createLinear(const QString& layer) -> NNLayerWidget*;
  static auto createConvolution(const QString& layer) -> NNLayerWidget*;

private:
  std::unordered_map<QString, decltype(&FabricLayer::exampl)> m_FactorMap;

  QString m_strType{"Layer"};
};

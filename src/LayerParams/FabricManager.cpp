#include "FabricManager.h"
#include "FabricAdapter.h"
#include "FabricLayer.h"
#include <iostream>
#include "Macros.h"

FabricManager::FabricManager() {
  m_fabrics["Layer"] = new FabricLayer;
  m_fabrics["Adapter"] = new FabricAdapter;

  for(const auto& [name, fab]: m_fabrics)
    CONNECT_CHECK(connect(fab,  SIGNAL(createWidget(NNWidget*, QPoint)),
                          this, SIGNAL(createWidget(NNWidget*, QPoint))));

}

auto FabricManager::chooseFabric(const QString& type) const -> IFabric* {
  if (m_fabrics.find(type) != m_fabrics.cend()) return m_fabrics.at(type);
  std::cerr << "You don't have such type of " << type.toStdString() << " in "
            << " Fabrics" << std::endl;
  return nullptr;
}

void FabricManager::onCreate(const std::vector<QString>& tags) {
  auto pWidget = createImpl(tags);
  if (! pWidget) return;
  auto modif = QGuiApplication::keyboardModifiers();
  switch (modif) {
    case Qt::ShiftModifier : emit changeWidget(pWidget); break;
    case Qt::AltModifier   : emit createAndConnectWidget(pWidget); break;
    case Qt::NoModifier    :
    default                : emit createWidget(pWidget);
  }
}

auto FabricManager::create(const QString& type, const QString& subtype) const -> NNWidget* {
  return chooseFabric(type)->create(subtype);
}

auto FabricManager::load(const QString& type, const QString& subType, KeyType id) const -> NNWidget* {
  return chooseFabric(type)->load(subType, id);
}

auto FabricManager::createImpl(const std::vector<QString>& tags) const -> NNWidget* {
  const auto [type, layer] = findTypeAndSubtype(tags);
  return chooseFabric(type)->load(layer);
}

auto FabricManager::findTypeAndSubtype(const std::vector<QString>& tags) const
    -> std::pair<QString, QString> {
  KeyType index = 0;
  if(tags.empty()) {
    std::cerr << "Tags are empty" << std::endl;
    return {{}, {}};
  };
  QString type = tags[index];
  QString layer{};
  if (m_fabrics.find(type) == m_fabrics.end()) {
    std::cerr << "You don't have " << type.toStdString() << " in txt file" << std::endl;
    return {{}, {}};
  };

  for (index = 2; index < tags.size(); ++index) layer += tags[index];
  return {type, layer};
}

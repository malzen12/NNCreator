#include "IFabric.h"
#include <iostream>

IFabric::IFabric(QString name)
    : m_strType(std::move(name)) {}

auto IFabric::idCorrection(KeyType id) -> KeyType {
  if (id == SIZE_MAX) return NNWidget::createId();
  return id;
}

auto IFabric::load(const QString& type, KeyType id) const -> NNWidget* {
  return impl(type, id);
}

auto IFabric::create(const QString& type) const -> NNWidget* {
  return impl(type, SIZE_MAX);
}

auto IFabric::impl(const QString& type, KeyType id) const -> NNWidget* {
  if (type.isEmpty()) return nullptr;
  if (m_FabricMap.find(type) != m_FabricMap.cend()) return m_FabricMap.at(type)(idCorrection(id));
  std::cerr << "You don't have such type of " << type.toStdString() << " in " << m_strType.toStdString()
            << std::endl;
  return nullptr;
}

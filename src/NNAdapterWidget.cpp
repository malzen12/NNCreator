#include "NNAdapterWidget.h"
#include <iostream>
#include <QApplication>
#include <QMouseEvent>
#include <QVBoxLayout>

auto NNAdapterWidget::save() const -> std::string {
  std::stringstream stream;
  stream << "Adapter" << '\n';
  stream << m_id << '\n';
  stream << m_spParams->save() /* << '\n'*/;
  const auto pos_ = pos();
  stream << pos_.x() << ' ' << pos_.y() << '\n' << '\n';

  return stream.str();
}

void NNAdapterWidget::updateStyle() {}

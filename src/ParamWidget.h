#pragma once

#include "NNParam.h"
#include <QLabel>
#include <QWidget>

class ParamWidget : public QWidget {
  Q_OBJECT
public:
  explicit ParamWidget(const NNParam& crParam);

  auto getName() const noexcept -> const std::string&;
  auto getParam() -> NNParam&;

  auto collectValue() -> const NNParam&;

private:
  void initGUI();
  void initGuiEnum();
  void initGuiString();
  void initGuiUint();

  void collectQList();
  void collectQString();
  void collectUInt();
  void collectDouble();
  void collectBool();

private:
  NNParam m_Param;

  QLabel* m_pNameLabel;
  QWidget* m_pEditorWidget;
  void initGuiDouble();
  void initGuiBool();
  void initGuiQList();
};

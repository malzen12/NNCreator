#pragma once

#include "NNWidget.h"

class NNLayerWidget : public NNWidget {
  Q_OBJECT
  using KeyType = std::size_t;
  using Vector = std::vector<KeyType>;
  using VectorOfVector = std::vector<Vector>;

public:
  NNLayerWidget(KeyType id, const std::shared_ptr<NNLayerParams>& spParams);
  NNLayerWidget(const NNLayerWidget& another);

  auto getParams() const noexcept -> const std::shared_ptr<NNLayerParams>&;
  auto getInputSize() const -> const VectorOfVector&;
  auto getMainOutputSize() -> Vector;
  auto getOptionalOutputSize(KeyType index) -> Vector;

  void setParams(const std::shared_ptr<NNLayerParams>& spParams) noexcept;

  void updateStyle() final;

  auto save() const -> std::string final;

  void load(std::stringstream& stream);

  void resetInputSize();
  void resetInputAndOutputSize();
  void checkIsValid();
  void addInputSize(const Vector& vInputSize);

  auto calcOutputSize() -> const VectorOfVector&;

private:
  void initGUI();

  auto createName() const -> QString;

private:
  std::shared_ptr<NNLayerParams> m_spParams;
  VectorOfVector m_vInputSizes;
  VectorOfVector m_vOutputSizes{{}};

  QLabel* m_pInputLabel;
  QLabel* m_pNameLabel;
  QLabel* m_pOutputLabel;
};

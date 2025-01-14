#pragma once

#include "NNWidget.h"

class NNAdapterWidget : public NNWidget {
  // class NNLayerWidget : public QWidget {
  Q_OBJECT
  using KeyType = std::size_t;
  using Vector = std::vector<KeyType>;
  using VectorOfVector = std::vector<Vector>;

public:
  NNAdapterWidget(KeyType id, const std::shared_ptr<NNLayerParams>& spParams);
  NNAdapterWidget(const NNAdapterWidget& another);

  auto getInputSize() const -> const VectorOfVector& {};
  auto getMainOutputSize() -> Vector{};
  auto getOptionalOutputSize(KeyType index) -> Vector{};

  void updateStyle() final;

  auto save() const -> std::string final;

  void load(std::stringstream& stream){};

  void deleteLayer();

  void resetInputSize(){};
  void resetInputAndOutputSize(){};
  void checkIsValid(){};
  void addInputSize(const Vector& vInputSize){};

  auto calcOutputSize() -> const VectorOfVector& {};

private:
  void initGUI();

  auto createName() const -> QString;

private:
  std::shared_ptr<NNLayerParams> m_spParams;
  VectorOfVector m_vInputSizes;
  VectorOfVector m_vOutputSizes{{}};
};


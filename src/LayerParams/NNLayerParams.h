#pragma once

#include "NNParam.h"
#include <memory>
#include <sstream>
#include <string>
#include <vector>

class NNLayerParams {
protected:
  using KeyType = std::size_t;
  using Vector = std::vector<KeyType>;
  using VectorOfVector = std::vector<Vector>;
  using InputSizeType = std::vector<Vector>;
  using NNParamContainer = std::vector<NNParam>;

public:
  NNLayerParams() = default;
  explicit NNLayerParams(const std::string& strName, const NNParamContainer& vParams);
  virtual ~NNLayerParams() = default;

  auto getName() const noexcept -> const std::string&;
  auto getParams() const noexcept -> const NNParamContainer&;
  bool isValid() const noexcept;

  void setName(const std::string& strName) noexcept;
  void setParams(const NNParamContainer& vParams) noexcept;

  virtual auto getDisplayName() const noexcept -> QString;

  virtual auto copy() const -> std::shared_ptr<NNLayerParams> = 0;
  virtual auto checkInputSize(const InputSizeType& vInputSizes, bool& isGlobalError) const -> std::string = 0;
  virtual auto calcOutputSize(const InputSizeType& vInputSizes) const -> VectorOfVector = 0;

  auto makeXmlString() const -> std::string;

  auto save() const -> std::string;
  void load(std::stringstream& stream);

protected:
  auto copyImpl() const -> NNParamContainer;

protected:
  std::string m_strName;
  NNParamContainer m_vParams;
};

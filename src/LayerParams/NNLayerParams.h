#pragma once

#include "NNParam.h"
#include <memory>
#include <string>
#include <vector>

class NNLayerParams {
protected:
  using KeyType = std::size_t;
  using Vector = std::vector<KeyType>;
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
  virtual bool checkInputSize(const InputSizeType& vInputSizes) const = 0;
  virtual auto calcOutputSize(const InputSizeType& vInputSizes) const -> Vector = 0;

  std::string makeXmlString() const;

protected:
  auto copyImpl() const -> NNParamContainer;

protected:
  std::string m_strName;
  NNParamContainer m_vParams;
};

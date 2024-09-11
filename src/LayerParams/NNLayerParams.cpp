#include "NNLayerParams.h"
#include <sstream>

NNLayerParams::NNLayerParams(const std::string& strName, const NNParamContainer& vParams)
    : m_strName{strName}
    , m_vParams{vParams} {}

auto NNLayerParams::getName() const noexcept -> const std::string& { return m_strName; }

auto NNLayerParams::getParams() const noexcept -> const NNParamContainer& { return m_vParams; }

bool NNLayerParams::isValid() const noexcept { return ! m_vParams.empty(); }

void NNLayerParams::setName(const std::string& strName) noexcept { m_strName = strName; }

void NNLayerParams::setParams(const std::vector<NNParam>& vParams) noexcept { m_vParams = vParams; }

auto NNLayerParams::getDisplayName() const noexcept -> QString { return QString::fromStdString(m_strName); }

auto NNLayerParams::makeXmlString() const -> std::string {
  std::stringstream Stream;
  Stream << "<type>" << m_strName << "</type>" << std::endl;
  for (const auto& spParams : m_vParams) Stream << spParams.makeXmlString();
  return Stream.str();
}

auto NNLayerParams::copyImpl() const -> NNParamContainer {
  std::vector<NNParam> vParams;
  for (const auto param : m_vParams) vParams.push_back(param);
  return vParams;
}

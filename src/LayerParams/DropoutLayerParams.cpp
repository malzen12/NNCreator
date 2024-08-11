#include "DropoutLayerParams.h"

DropoutLayerParams::DropoutLayerParams(const std::string& strName, const std::vector<NNParam>& vParams)
  : NNLayerParams(strName, vParams)
{
  assert(m_vParams.size() >= 2);
}

QString DropoutLayerParams::getDisplayName() const noexcept
{
  return m_vParams[0].getValue().toString();
}

bool DropoutLayerParams::checkInputSize(const std::vector<std::size_t>& vInputSize) const
{
  return !vInputSize.empty();
}

std::vector<std::size_t> DropoutLayerParams::calcOutputSize(const std::vector<std::size_t>& vInputSize) const
{
  return vInputSize;
}


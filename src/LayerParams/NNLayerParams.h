#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>

#include "NNParam.h"

class NNLayerParams
{
public:
    NNLayerParams() = default;
    explicit NNLayerParams(const std::string& strName, const std::vector<NNParam>& vParams);
    virtual ~NNLayerParams() = default;

    static std::shared_ptr<NNLayerParams> makeLinear();
    static std::shared_ptr<NNLayerParams> makeConv1d();
    static std::shared_ptr<NNLayerParams> makePool();
    static std::shared_ptr<NNLayerParams> makeEmbedding();
    static std::shared_ptr<NNLayerParams> makeReshape();
    static std::shared_ptr<NNLayerParams> makeNormalization();
    static std::shared_ptr<NNLayerParams> makeActivation();
    static std::shared_ptr<NNLayerParams> makeConcatinate();
    static std::shared_ptr<NNLayerParams> makeDropout();

    void setName(const std::string& strName) noexcept;
    const std::string& getName() const noexcept;

    const std::vector<NNParam>& getParams() const noexcept;
    void setParams(const std::vector<NNParam>& vParams) noexcept;

    bool isValid() const noexcept;

    virtual QString getDisplayName() const noexcept;

    virtual bool checkInputSize(const std::vector<std::size_t>& vInputSize) const = 0;
    virtual std::vector<std::size_t> calcOutputSize(const std::vector<std::size_t>& vInputSize) const = 0;

    std::string makeXmlString() const;

protected:
    std::string m_strName;
    std::vector<NNParam> m_vParams;
};


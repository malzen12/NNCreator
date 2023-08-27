#pragma once

#include <string>
#include <vector>
#include <functional>

#include "NNParam.h"

class NNLayerParams
{
    using check_input_size_func = std::function<bool(const std::vector<NNParam>&, const std::vector<std::size_t>&)>;
    using calc_output_size_func = std::function<std::vector<std::size_t>(const std::vector<NNParam>&, const std::vector<std::size_t>&)>;

public:
    NNLayerParams() = default;
    explicit NNLayerParams(const std::string& strName, const std::vector<NNParam>& vParams, check_input_size_func fCheckInput, calc_output_size_func fCalcOutput);

    static NNLayerParams makeLinear();
    static NNLayerParams makeConv1d();
    static NNLayerParams makePool();
    static NNLayerParams makeEmbedding();
    static NNLayerParams makeReshape();
    static NNLayerParams makeNormalization();
    static NNLayerParams makeActivation();

    void setName(const std::string& strName) noexcept;
    const std::string& getName() const noexcept;

    const std::vector<NNParam>& getParams() const noexcept;
    void setParams(const std::vector<NNParam>& vParams) noexcept;

    bool isValid() const noexcept;

    bool checkInputSize(const std::vector<std::size_t>& vInputSize) const;
    std::vector<std::size_t> calcOutputSize(const std::vector<std::size_t>& vInputSize) const;

private:
    std::string m_strName;
    std::vector<NNParam> m_vParams;

    check_input_size_func m_fCheckInput;
    calc_output_size_func m_fCalcOutput;
};


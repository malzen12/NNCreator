#include "NNLayerParams.h"

NNLayerParams::NNLayerParams(const std::string& strName, const std::vector<NNParam>& vParams, check_input_size_func fCheckInput, calc_output_size_func fCalcOutput)
    : m_strName{strName},
      m_vParams{vParams},
      m_fCheckInput{std::move(fCheckInput)},
      m_fCalcOutput{std::move(fCalcOutput)}
{

}

NNLayerParams NNLayerParams::makeLinear()
{
    auto strName = "Linear";

    std::vector<NNParam> vParams = {NNParam{"input size", 0}, NNParam{"output size", 0}};

    check_input_size_func fCheckInput = [](const std::vector<NNParam>& vParams, const std::vector<std::size_t>& vInputSize)
    {
        return vParams[0].getValue().toUInt() == vInputSize.back();
    };

    calc_output_size_func fCalcOutput = [](const std::vector<NNParam>& vParams, const std::vector<std::size_t>& vInputSize)
    {
        auto vRes = vInputSize;
        vRes.back() = vParams[1].getValue().toUInt();
        return vRes;
    };

    return NNLayerParams{strName, vParams, fCheckInput, fCalcOutput};
}

NNLayerParams NNLayerParams::makeConv1d()
{
    auto strName = "Conv1d";
    std::vector<NNParam> vParams = {NNParam{"in depth", 0}, NNParam{"out depth", 0}, NNParam{"kernel size", 0}, NNParam{"stride", 1}, NNParam{"padding", 0}, NNParam{"dilitation", 1}};

    check_input_size_func fCheckInput = [](const std::vector<NNParam>& vParams, const std::vector<std::size_t>& vInputSize)
    {
        auto sKernel = vParams[2].getValue().toUInt();
        auto sInDepth = vParams[0].getValue().toUInt();

        return  vInputSize.size() > 1 &&
                vInputSize.back() > sKernel &&
                vInputSize[vInputSize.size() - 2] == sInDepth;
    };

    calc_output_size_func fCalcOutput = [](const std::vector<NNParam>& vParams, const std::vector<std::size_t>& vInputSize)
    {
        assert(vInputSize.size() > 1);

        auto sOutDepth = vParams[1].getValue().toUInt();

        auto sKernel = vParams[2].getValue().toUInt();
        auto sStride = vParams[3].getValue().toUInt();
        auto sPadding = vParams[4].getValue().toUInt();
        auto sDilitation = vParams[5].getValue().toUInt();

        auto vRes = vInputSize;
        vRes.back() = (vInputSize.back() + 2 * sPadding - sDilitation * (sKernel - 1) - 1) / sStride + 1;
        vRes[vRes.size() - 2] = sOutDepth;

        return vRes;
    };

    return NNLayerParams{strName, vParams, fCheckInput, fCalcOutput};
}

NNLayerParams NNLayerParams::makePool()
{
    auto strName = "Pool";
    std::vector<NNParam> vParams = {NNParam{"kernel size", 0}, NNParam{"stride", 1}, NNParam{"padding", 0}, NNParam{"dilitation", 1}};

    check_input_size_func fCheckInput = [](const std::vector<NNParam>& vParams, const std::vector<std::size_t>& vInputSize)
    {
        auto sKernel = vParams[0].getValue().toUInt();

        return  vInputSize.back() > sKernel;
    };

    calc_output_size_func fCalcOutput = [](const std::vector<NNParam>& vParams, const std::vector<std::size_t>& vInputSize)
    {
        auto sKernel = vParams[0].getValue().toUInt();
        auto sStride = vParams[1].getValue().toUInt();
        auto sPadding = vParams[2].getValue().toUInt();
        auto sDilitation = vParams[3].getValue().toUInt();

        auto vRes = vInputSize;
        vRes.back() = (vInputSize.back() + 2 * sPadding - sDilitation * (sKernel - 1) - 1) / sStride + 1;

        return vRes;
    };

    return NNLayerParams{strName, vParams, fCheckInput, fCalcOutput};
}

NNLayerParams NNLayerParams::makeEmbedding()
{
    auto strName = "Embedding";
    std::vector<NNParam> vParams = {NNParam{"vocab size", 0}, NNParam{"embedding size", 0}};
    check_input_size_func fCheckInput = [](const std::vector<NNParam>& /*vParams*/, const std::vector<std::size_t>& /*vInputSize*/)
    {
        return true;
    };

    calc_output_size_func fCalcOutput = [](const std::vector<NNParam>& vParams, const std::vector<std::size_t>& vInputSize)
    {
        auto vRes = vInputSize;
        vRes.push_back(vParams[1].getValue().toUInt());

        return vRes;
    };

    return NNLayerParams{strName, vParams, fCheckInput, fCalcOutput};
}

NNLayerParams NNLayerParams::makeReshape()
{
    auto strName = "Reshape";
    std::vector<NNParam> vParams = {NNParam{"size array", "[1, 2, 3]", QVariant::Type::String}}; ///< @todo

    check_input_size_func fCheckInput = [](const std::vector<NNParam>& /*vParams*/, const std::vector<std::size_t>& /*vInputSize*/)
    {
        return true;
    };

    calc_output_size_func fCalcOutput = [](const std::vector<NNParam>& vParams, const std::vector<std::size_t>& /*vInputSize*/)
    {
        auto List = vParams[0].getValue().toList();
        std::vector<std::size_t> vRes;

        for (const auto& crValue : List)
        {
            vRes.push_back(crValue.toUInt());
        }

        return vRes;
    };

    return NNLayerParams{strName, vParams, fCheckInput, fCalcOutput};
}

NNLayerParams NNLayerParams::makeNormalization()
{
    auto strName = "Normalization";
    std::vector<NNParam> vParams = {NNParam{"strange param", 0}};

    check_input_size_func fCheckInput = [](const std::vector<NNParam>& /*vParams*/, const std::vector<std::size_t>& /*vInputSize*/)
    {
        return true;
    };

    calc_output_size_func fCalcOutput = [](const std::vector<NNParam>& /*vParams*/, const std::vector<std::size_t>& vInputSize)
    {
        return vInputSize;
    };

    return NNLayerParams{strName, vParams, fCheckInput, fCalcOutput};
}

NNLayerParams NNLayerParams::makeActivation()
{
    auto strName = "Activation function";

    std::vector<NNParam> vParams = {NNParam{"activation type", "relu", QVariant::Type::String, true}};///< @todo activation func

    check_input_size_func fCheckInput = [](const std::vector<NNParam>& /*vParams*/, const std::vector<std::size_t>& /*vInputSize*/)
    {
        return true;
    };

    calc_output_size_func fCalcOutput = [](const std::vector<NNParam>& /*vParams*/, const std::vector<std::size_t>& vInputSize)
    {
        return vInputSize;
    };

    return NNLayerParams{strName, vParams, fCheckInput, fCalcOutput};
}

NNLayerParams NNLayerParams::makeConcatinate()
{
    auto strName = "Concatinate";

    std::vector<NNParam> vParams = {NNParam{"axis", 0}};

    check_input_size_func fCheckInput = [](const std::vector<NNParam>& vParams, const std::vector<std::size_t>& vInputSize)
    {
        auto sAxis = vParams[0].getValue().toUInt();
        return sAxis < vInputSize.size();
    };

    calc_output_size_func fCalcOutput = [](const std::vector<NNParam>& /*vParams*/, const std::vector<std::size_t>& vInputSize)
    {
        return vInputSize;
    };

    return NNLayerParams{strName, vParams, fCheckInput, fCalcOutput};
}

void NNLayerParams::setName(const std::string& strName) noexcept
{
    m_strName = strName;
}

const std::string& NNLayerParams::getName() const noexcept
{
    return m_strName;
}

const std::vector<NNParam>& NNLayerParams::getParams() const noexcept
{
    return m_vParams;
}

void NNLayerParams::setParams(const std::vector<NNParam>& vParams) noexcept
{
    m_vParams = vParams;
}

bool NNLayerParams::isValid() const noexcept
{
    return !m_vParams.empty();
}

bool NNLayerParams::checkInputSize(const std::vector<std::size_t>& vInputSize) const
{
    return m_fCheckInput(m_vParams, vInputSize);
}

std::vector<std::size_t> NNLayerParams::calcOutputSize(const std::vector<std::size_t>& vInputSize) const
{
    return m_fCalcOutput(m_vParams, vInputSize);
}

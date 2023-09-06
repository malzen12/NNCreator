#include "NNLayerParams.h"

#include "ActivationFunc.h"
#include "InitializerFunc.h"

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

    std::vector<NNParam> vParams = {NNParam{"input_size", 0},
                                    NNParam{"output_size", 0},
                                    NNParam{InitializerFunc::getClassName(),
                                            InitializerFunc{static_cast<initializer_func>(0)}.toString(),
                                            QVariant::Type::String, true}};

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
    std::vector<NNParam> vParams = {NNParam{"in_depth", 0},
                                    NNParam{"out_depth", 0},
                                    NNParam{"kernel_size", 0},
                                    NNParam{"stride", 1},
                                    NNParam{"padding", 0},
                                    NNParam{"dilitation", 1},
                                    NNParam{InitializerFunc::getClassName(),
                                            InitializerFunc{static_cast<initializer_func>(0)}.toString(),
                                            QVariant::Type::String, true}};

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
    std::vector<NNParam> vParams = {NNParam{"kernel_size", 0}, NNParam{"stride", 1}, NNParam{"padding", 0}, NNParam{"dilitation", 1}};

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
    std::vector<NNParam> vParams = {NNParam{"vocab_size", 0}, NNParam{"embedding_size", 0}};
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
    std::vector<NNParam> vParams = {NNParam{"size", QList<QVariant>{}, QVariant::Type::List}}; ///< @todo

    check_input_size_func fCheckInput = [](const std::vector<NNParam>& vParams, const std::vector<std::size_t>& /*vInputSize*/)
    {
        return !vParams[0].getValue().toList().empty();
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
    std::vector<NNParam> vParams = {NNParam{"strange_param", 0}};

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
    auto strName = ActivationFunc::getClassName();

    std::vector<NNParam> vParams = {NNParam{ActivationFunc::getClassName(),
                                            ActivationFunc{activation_func::relu}.toString(),
                                            QVariant::Type::String, true}};

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

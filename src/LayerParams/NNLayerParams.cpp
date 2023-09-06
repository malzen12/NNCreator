#include "NNLayerParams.h"

#include <sstream>

#include "ActivationFunc.h"
#include "InitializerFunc.h"

#include "LinearLayerParams.h"
#include "Conv1dLayerParams.h"
#include "PoolLayerParams.h"
#include "EmbeddingLayerParams.h"
#include "ReshapeLayerParams.h"
#include "NormalizationLayerParams.h"
#include "ActivationLayerParams.h"
#include "ConcatinateLayerParams.h"

NNLayerParams::NNLayerParams(const std::string& strName, const std::vector<NNParam>& vParams)
    : m_strName{strName},
      m_vParams{vParams}
{

}

std::shared_ptr<NNLayerParams> NNLayerParams::makeLinear()
{
    auto strName = "Linear";

    std::vector<NNParam> vParams = {NNParam{"input_size", 0},
                                    NNParam{"output_size", 0},
                                    NNParam{InitializerFunc::getClassName(),
                                            InitializerFunc{static_cast<initializer_func>(0)}.toString(),
                                            QVariant::Type::String, true}};

    return std::make_shared<LinearLayerParams>(strName, vParams);
}

std::shared_ptr<NNLayerParams> NNLayerParams::makeConv1d()
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

    return std::make_shared<Conv1dLayerParams>(strName, vParams);
}

std::shared_ptr<NNLayerParams> NNLayerParams::makePool()
{
    auto strName = "Pool";
    std::vector<NNParam> vParams = {NNParam{"kernel_size", 0}, NNParam{"stride", 1}, NNParam{"padding", 0}, NNParam{"dilitation", 1}};

    return std::make_shared<PoolLayerParams>(strName, vParams);
}

std::shared_ptr<NNLayerParams> NNLayerParams::makeEmbedding()
{
    auto strName = "Embedding";
    std::vector<NNParam> vParams = {NNParam{"vocab_size", 0}, NNParam{"embedding_size", 0}};

    return std::make_shared<EmbeddingLayerParams>(strName, vParams);
}

std::shared_ptr<NNLayerParams> NNLayerParams::makeReshape()
{
    auto strName = "Reshape";
    std::vector<NNParam> vParams = {NNParam{"size", QList<QVariant>{}, QVariant::Type::List}}; ///< @todo

    return std::make_shared<ReshapeLayerParams>(strName, vParams);
}

std::shared_ptr<NNLayerParams> NNLayerParams::makeNormalization()
{
    auto strName = "Normalization";
    std::vector<NNParam> vParams = {NNParam{"strange_param", 0}};

    return std::make_shared<NormalizationLayerParams>(strName, vParams);
}

std::shared_ptr<NNLayerParams> NNLayerParams::makeActivation()
{
    auto strName = ActivationFunc::getClassName();

    std::vector<NNParam> vParams = {NNParam{ActivationFunc::getClassName(),
                                            ActivationFunc{activation_func::relu}.toString(),
                                            QVariant::Type::String, true}};

    return std::make_shared<ActivationLayerParams>(strName, vParams);
}

std::shared_ptr<NNLayerParams> NNLayerParams::makeConcatinate()
{
    auto strName = "Concatinate";
    std::vector<NNParam> vParams = {NNParam{"axis", 0}};

    return std::make_shared<ConcatinateLayerParams>(strName, vParams);
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

std::string NNLayerParams::makeXmlString() const
{
    std::stringstream Stream;

    Stream << "<type>" << m_strName << "</type>" << std::endl;

    for (const auto& spParams : m_vParams)
        Stream << spParams.makeXmlString();

    return Stream.str();
}

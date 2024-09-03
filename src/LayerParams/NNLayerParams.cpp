#include "NNLayerParams.h"

#include <sstream>

#include "ActivationFunc.h"
#include "InitializerFunc.h"
#include "Normalization.h"
#include "Dropout.h"
#include "Recurrent.h"
#include "PaddingMode.h"

#include "LinearLayerParams.h"
#include "BilinearLayerParams.h"
#include "Conv1dLayerParams.h"
#include "Conv2dLayerParams.h"
#include "PoolLayerParams.h"
#include "EmbeddingLayerParams.h"
#include "ReshapeLayerParams.h"
#include "NormalizationLayerParams.h"
#include "ActivationLayerParams.h"
#include "Pool2dLayerParams.h"
#include "DropoutLayerParams.h"
#include "FlattenLayerParams.h"
#include "RecurrentLayerParams.h"

NNLayerParams::NNLayerParams(const std::string& strName, const std::vector<NNParam>& vParams)
  : m_strName{strName},
    m_vParams{vParams}
{}
//in_features, out_features, bias=True
std::shared_ptr<NNLayerParams> NNLayerParams::makeLinear()
{
  auto strName = "Linear";
  std::vector<NNParam> vParams = {NNParam{"Input I[*,?]", "in_features", 0},
                                  NNParam{"Output O[*,?]", "out_features", 0},
                                  NNParam{"Bias", "bias", true, QVariant::Type::Bool},
                                  NNParam{"Initializer Func",
                                          InitializerFunc::getClassName(),
                                          InitializerFunc{static_cast<initializer_func>(0)}.toString(),
                                          QVariant::Type::String, true}};

  return std::make_shared<LinearLayerParams>(strName, vParams);
}
//in1_features, in2_features, out_features, bias=True
std::shared_ptr<NNLayerParams> NNLayerParams::makeBilinear()
{
  auto strName = "Bilinear";

  std::vector<NNParam> vParams = {NNParam{"Input 1 I[*,?]", "in1_features", 0},
                                  NNParam{"Input 2 I[*,?]", "in2_features", 0},
                                  NNParam{"Output O[*,?]", "out_features", 0},
                                  NNParam{"Bias", "bias", true, QVariant::Type::Bool},
                                  NNParam{"Initializer Func",
                                          InitializerFunc::getClassName(),
                                          InitializerFunc{static_cast<initializer_func>(0)}.toString(),
                                          QVariant::Type::String, true}};

  return std::make_shared<BilinearLayerParams>(strName, vParams);
}
//in_channels, out_channels, kernel_size,
//stride=1, padding=0, dilation=1, groups=1, bias=True, padding_mode='zeros'
//(N,C in ,L
std::shared_ptr<NNLayerParams> NNLayerParams::makeConv1d()
{
  auto strName = "Conv1d";
  std::vector<NNParam> vParams = {NNParam{"Input I[N,?,0]", "in_channels", 0},
                                  NNParam{"Output O[N,?,0]", "out_channels", 0},
                                  NNParam{"Kernel Size", "kernel_size", 1},
                                  NNParam{"Stride", "stride", 1},
                                  NNParam{"Padding", "padding", 0},
                                  NNParam{"Dilation", "dilation", 1},
                                  NNParam{"Groups", "groups", 1},
                                  NNParam{"Bias", "bias", true, QVariant::Type::Bool},
                                  NNParam{"Padding Mode", "padding_mode",
                                          PaddingMode{static_cast<e_padding_mode>(0)}.toString(),
                                          QVariant::Type::String, true},
                                  NNParam{"Initializer Func",
                                          InitializerFunc::getClassName(),
                                          InitializerFunc{static_cast<initializer_func>(0)}.toString(),
                                          QVariant::Type::String, true}};

  return std::make_shared<Conv1dLayerParams>(strName, vParams);
}
//in_channels, out_channels, kernel_size,
//stride=1, padding=0, dilation=1, groups=1,
//bias=True, padding_mode='zeros'
std::shared_ptr<NNLayerParams> NNLayerParams::makeConv2d()
{
  auto strName = "Conv2d";
  std::vector<NNParam> vParams = {NNParam{"Input I[N,?,1,0]", "in_channels", 0},
                                  NNParam{"Output O[N,?,1,0]", "out_channels", 0},
                                  NNParam{"Kernel Size", "kernel_size",
                                          QList<QVariant>{1, 1}, QVariant::Type::List},
                                  NNParam{"Stride", "stride",
                                          QList<QVariant>{1, 1}, QVariant::Type::List},
                                  NNParam{"Padding", "padding",
                                          QList<QVariant>{0, 0}, QVariant::Type::List},
                                  NNParam{"Dilation", "dilation",
                                          QList<QVariant>{1, 1}, QVariant::Type::List},
                                  NNParam{"Bias", "bias", true, QVariant::Type::Bool},
                                  NNParam{"Padding Mode", "padding_mode",
                                          PaddingMode{static_cast<e_padding_mode>(0)}.toString(),
                                          QVariant::Type::String, true},
                                  NNParam{"Initializer Func",
                                          InitializerFunc::getClassName(),
                                          InitializerFunc{static_cast<initializer_func>(0)}.toString(),
                                          QVariant::Type::String, true}};

  return std::make_shared<Conv2dLayerParams>(strName, vParams);
}

std::shared_ptr<NNLayerParams> NNLayerParams::makePool()
{
  auto strName = "Pool";
  std::vector<NNParam> vParams = {NNParam{"kernel_size", 0},
                                  NNParam{"stride", 1},
                                  NNParam{"padding", 0},
                                  NNParam{"dilitation", 1}};

  return std::make_shared<PoolLayerParams>(strName, vParams);
}

std::shared_ptr<NNLayerParams> NNLayerParams::makePool2d()
{
  auto strName = "Pool2d";
  std::vector<NNParam> vParams = {NNParam{"kernel_size", QList<QVariant>{0, 0}, QVariant::Type::List},
                                  NNParam{"stride", QList<QVariant>{1, 1}, QVariant::Type::List},
                                  NNParam{"padding", QList<QVariant>{0, 0}, QVariant::Type::List},
                                  NNParam{"dilitation", QList<QVariant>{1, 1}, QVariant::Type::List},
                                  NNParam{InitializerFunc::getClassName(),
                                          InitializerFunc{static_cast<initializer_func>(0)}.toString(),
                                          QVariant::Type::String, true}};

  return std::make_shared<Pool2dLayerParams>(strName, vParams);
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
  std::vector<NNParam> vParams = {NNParam{Normalization::getClassName(),
                                          Normalization{enu_normalization::batchnorm1d}.toString(),
                                          QVariant::Type::String, true},
                                  NNParam{"strange_param", 0}};

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

std::shared_ptr<NNLayerParams> NNLayerParams::makeDropout()
{
  auto strName = "Dropout";
  std::vector<NNParam> vParams = {NNParam{Dropout::getClassName(),
                                          Dropout{enu_dropout::dropout}.toString(),
                                          QVariant::Type::String, true},
                                  NNParam{"p", 0, QVariant::Type::Double}};

  return std::make_shared<DropoutLayerParams>(strName, vParams);
}

std::shared_ptr<NNLayerParams> NNLayerParams::makeFlatten()
{
  auto strName = "Flatten";
  std::vector<NNParam> vParams = {NNParam{"axis_to_flat", QList<QVariant>{0, 1}, QVariant::Type::List}};

  return std::make_shared<FlattenLayerParams>(strName, vParams);
}

std::shared_ptr<NNLayerParams> NNLayerParams::makeRecurrent()
{
  auto strName = "Recurrent";
  std::vector<NNParam> vParams = {NNParam{Recurrent::getClassName(),
                                          Recurrent{enu_recurrent::rnn}.toString(),
                                          QVariant::Type::String, true},
                                  NNParam{"input_size", 0},
                                  NNParam{"hidden_size", 0},
                                  NNParam{"num_layers", 0},
                                  NNParam{"bias", false, QVariant::Type::Bool},
                                  NNParam{"bidirectional", false, QVariant::Type::Bool}};

  return std::make_shared<RecurrentLayerParams>(strName, vParams);
}

std::shared_ptr<NNLayerParams> NNLayerParams::makeMyRecurrent()
{
  auto strName = "MyRecurrent";
  std::vector<NNParam> vParams = {NNParam{"input_size", 0},
                                  NNParam{"hidden_layers", 0},
                                  NNParam{InitializerFunc::getClassName(),
                                          InitializerFunc{static_cast<initializer_func>(0)}.toString(),
                                          QVariant::Type::String, true}};

  return std::make_shared<LinearLayerParams>(strName, vParams);
}

std::shared_ptr<NNLayerParams> NNLayerParams::createLayer()
{
//  m_fabricLayer->create();
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

QString NNLayerParams::getDisplayName() const noexcept
{
  return QString::fromStdString(m_strName);
}

std::string NNLayerParams::makeXmlString() const
{
  std::stringstream Stream;

  Stream << "<type>" << m_strName << "</type>" << std::endl;

  for(const auto& spParams : m_vParams)
    Stream << spParams.makeXmlString();

  return Stream.str();
}

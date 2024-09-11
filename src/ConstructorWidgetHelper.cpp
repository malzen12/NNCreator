#include "ConstructorWidgetHelper.h"
#include <iostream>

#define ISVALID                                                 \
  if (! isValid()) {                                            \
    std::cerr << "Graph is not valid" << __LINE__ << std::endl; \
    return {};                                                  \
  }

#define ISVALIDVOID                                             \
  if (! isValid()) {                                            \
    std::cerr << "Graph is not valid" << __LINE__ << std::endl; \
    return;                                                     \
  }

void PyPrinter::endl() { m_Stream << '\n'; }

auto PyPrinter::getStreamRef() -> std::stringstream& { return m_Stream; }

auto PyPrinter::createName(const NNLayerWidget* pParam) -> std::string {
  std::stringstream stream;
  stream << "self." << pParam->getParams()->getName() << "_" << pParam->getId();
  return stream.str();
}

auto PyPrinter::createNameEnum(const NNLayerWidget* pParam) -> std::string {
  std::stringstream stream;
  stream << "self." << pParam->getParams()->getDisplayName().toStdString() << "_" << pParam->getId();
  return stream.str();
}

auto PyPrinter::createNameMyRecurrent(const NNLayerWidget* pParam) -> std::string {
  std::stringstream stream;
  // clang-format off
  stream << "self." << "Linear" << "_" << pParam->getId() << "h";
  // clang-format on
  return stream.str();
}

void PyPrinter::printCopy(const KeyType index, const Branch::BranchReturnValue& branch) {
  const auto set = branch.layersBranch.at(index);
  const auto itFront = set.cbegin();
  auto it = std::next(itFront);
  for (auto ite = set.cend(); ite != it; it++)
    *this << "x" << *it << " = x" << *itFront << ".clone()" << '\n';
}

void PyPrinter::printNameForward(const NNLayerWidget* pLayer, const Branch::BranchReturnValue& branch) {
  auto pParam = pLayer->getParams();
  auto& pParamParam = pParam->getParams();
  bool isEnum = pParamParam.front().isEnum();

  const auto iCurrent    = pLayer->getId();
  const auto branchIndex = *branch.layersBranch.at(iCurrent).cbegin();
  if (! isEnum) {
    if (pParam->getName() == "MyRecurrent") {
      KeyType n = std::next(pParamParam.crbegin())->getValue().toInt();
      for (KeyType i = 0; i < n; ++i) {
        // clang-format off
        *this << "x" << branchIndex << " = " << createNameMyRecurrent(pLayer)
              << "(x" << branchIndex << ")"
              << '\n';
        // clang-format on
      }
    } else if (pParam->getName() == "Bilinear") {
      if (auto& itSet = branch.merger.at(iCurrent); itSet.size() == 2) {
        auto first  = Branch::hashSetFirst(itSet);
        auto second = Branch::hashSetSecond(itSet);
        // clang-format off
        *this << "x" << branchIndex << " = " << createName(pLayer)
              << "(x" << first << ", x" << second << ")"
              << '\n';
        // clang-format on
      } else {
        throw(std::runtime_error("Bilinear need 2 input"));
      }
    } else {
      // clang-format off
      *this << "x" << branchIndex << " = " << createName(pLayer)
            << "(x" << branchIndex << ")" << '\n';
      // clang-format on
    }

  } else {
    auto enamName = pParamParam.cbegin()->getName();
    if (enamName == "Recurrent") {
      // clang-format off
      *this << "x" << branchIndex << ", y = " << createNameEnum(pLayer)
            << "(x" << branchIndex << ")" << '\n';
      // clang-format on
    } else if (enamName == "Normalization") {
      // clang-format off
      *this << createNameEnum(pLayer) << " = nn." << pParam->getDisplayName().toStdString()
            << R"((x.size()))" << '\n';
      *this << "x" << branchIndex << " = " << createNameEnum(pLayer)
            << "(x" << branchIndex << ")" << '\n';
      // clang-format on
    }
  }
}

void PyPrinter::createInitLValue(const NNLayerWidget* pCurrent) {
  auto pParam = pCurrent->getParams();
  auto& pParamParam = pParam->getParams();
  bool isEnum = pParamParam.front().isEnum();
  if (! isEnum) {
    if (pParam->getName() == "MyRecurrent")
      *this << createNameMyRecurrent(pCurrent) << '\n';
    else
      *this << createName(pCurrent);
  } else {
    auto enamName = pParamParam.front().getName();
    if (enamName == "Recurrent")
      *this << createNameEnum(pCurrent);
    else if (enamName == "Normalization")
      return;
  }
}

void PyPrinter::createInitRValue(const NNLayerWidget* pCurrent) {
  auto pParam = pCurrent->getParams();
  auto& pParamParam = pParam->getParams();
  bool isEnum = pParamParam.front().isEnum();
  if (! isEnum) {
    if (pParam->getName() == "MyRecurrent") {
      auto it = pParamParam.cbegin();
      // clang-format off
      m_Stream << " = nn." << "Linear" << "("
               << it->makePyString() << ", " << it->makePyString() << ")\n";
      // clang-format on
    } else {
      auto enamName = pParamParam.front().getName();
      if (enamName == "Normalization") return;
      m_Stream << " = nn." << pParam->getName() << "(";
      if (! pParamParam.empty()) {
        auto it = pParamParam.cbegin();
        m_Stream << it->getHiddenName() << '=' << it->makePyString();
        if (pParamParam.size() > 1) {
          ++it;
          for (auto ite = pParamParam.cend(); std::next(it) != ite; ++it)
            m_Stream << ", " << it->getHiddenName() << '=' << it->makePyString();
        }
      }
      m_Stream << ")\n";
    }
  } else {
    auto it = pParamParam.cbegin();
    // clang-format off
    m_Stream << " = nn." << pParam->getDisplayName().toStdString() << "("
             << it++->makePyString() << ", "
             << it++->makePyString() << ", "
             << it++->makePyString() << ", ";
    if (pParamParam.front().getValue() == "RNN")
      m_Stream << R"('tanh')" << ", ";
    m_Stream << it++->makePyString() << ", "
             << R"(False, 0.0)" << ", " << it->makePyString() << ")\n";
    // clang-format on
  }
}

GraphManager::GraphManager(const NNLayerContainer& m_vLayers)
    : m_refvLayers(m_vLayers) {
  init();
}

auto GraphManager::print() -> std::string {
  ISVALID
  pyPrinter << R"(class Net(nn.Module):)" << '\n';
  ++pyPrinter << R"(def __init__(self):)" << '\n';
  ++pyPrinter << R"(super().__init__())" << '\n';
  printInit();

  --pyPrinter << R"(def forward(self, x0):)" << '\n';
  ++pyPrinter;
  printForward();
  pyPrinter << R"(return x0)" << '\n';
  return pyPrinter.getStreamRef().str();
}

auto GraphManager::getFinishIndex() const noexcept -> KeyType { return iFinish; }

auto GraphManager::getStartIndex() const noexcept -> KeyType { return iStart; }

void GraphManager::addEdge(KeyType from, KeyType to) {
  m_layersIndex[from].insert(to);
  ++m_layerInputCounter[to];
  m_backward[to].insert(from);
}

void GraphManager::addVertex(KeyType index) {
  m_backward[index].insert({});
  m_layersIndex[index].insert({});
  m_layerInputCounter[index] = 0;
  if (m_refvLayers.empty()) iStart = iFinish = 0;
}

void GraphManager::deleteEdge(KeyType from, KeyType to) {
  m_layersIndex[from].erase(m_layersIndex[from].find(to));
  --m_layerInputCounter[to];
  m_backward[to].erase(std::find(m_backward[to].cbegin(), m_backward[to].cend(), from));
}

void GraphManager::deleteVertex(KeyType index) {
  for (const auto i : m_layersIndex[index]) {
    m_backward[i].erase(std::find(m_backward[i].cbegin(), m_backward[i].cend(), index));
    --m_layerInputCounter[i];
  }
  for (const auto i : m_backward[index])
    m_layersIndex[i].erase(std::find(m_layersIndex[i].cbegin(), m_layersIndex[i].cend(), index));
  m_layersIndex.erase(index);
  m_layerInputCounter.erase(index);
  m_backward.erase(index);
}

void GraphManager::checkSizes(const Vector& vInputSize) {
  initStartAndFinish();
  ISVALIDVOID
  if (m_refvLayers.empty()) return;
  m_refvLayers.at(iStart)->addInputSize(vInputSize);
  m_calcucatedDfsSize.clear();
  m_calcucatedDfsSize.insert(iStart);
  backwardDfsForSizes(m_refvLayers.at(iFinish));
}

auto GraphManager::getForward(KeyType index) const -> const HashSet& { return m_layersIndex.at(index); }

void GraphManager::backwardDfsForSizes(NNLayerWidget* pLayer) {
  ISVALIDVOID
  if (m_calcucatedDfsSize.size() == m_refvLayers.size()) return;
  auto index = pLayer->getId();

  if (m_calcucatedDfsSize.find(index) == m_calcucatedDfsSize.end()) {
    m_calcucatedDfsSize.insert(index);
    if (pLayer->getParams()->getName() == "Bilinear") {
      if (m_backward.at(index).size() == 2) {
        for (const auto i : m_backward.at(index)) {
          backwardDfsForSizes(m_refvLayers.at(i));
          pLayer->addInputSize(m_refvLayers.at(i)->calcOutputSize());
        }
      } else {
        setInvalid();
        std::string str = "Bilinear need to have 2 input tensor";
        std::cerr << str << std::endl;
        emit notValid(str);
        return;
      }
    } else if (m_backward.find(index) != m_backward.end()) {
      for (const auto i : m_backward.at(index)) {
        backwardDfsForSizes(m_refvLayers.at(i));
        pLayer->addInputSize(m_refvLayers.at(i)->calcOutputSize());
      }
    }
  }
}

void GraphManager::setInvalid() { m_isValid = false; }

void GraphManager::printInit() {
  for (const auto [id, pCurrent] : m_refvLayers) {
    pyPrinter.createInitLValue(pCurrent);
    pyPrinter.createInitRValue(pCurrent);
  }
  pyPrinter.endl();
}

void GraphManager::printForward() {
  m_calcucatedDfs.clear();
  if (! m_refvLayers.empty()) branch = Branch{m_layersIndex, m_layerInputCounter}.calculate(iStart);
  backwardDfs(m_refvLayers.at(iFinish));
}

void GraphManager::backwardDfs(const NNLayerWidget* pLayer) {
  ISVALIDVOID
  if (m_calcucatedDfs.size() == m_refvLayers.size()) return;
  auto index = pLayer->getId();

  if (m_calcucatedDfs.find(index) == m_calcucatedDfs.end()) {
    m_calcucatedDfs.insert(index);
    if (pLayer->getParams()->getName() == "Bilinear") {
      if (m_backward.at(index).size() == 2) {
        for (const auto i : m_backward.at(index)) backwardDfs(m_refvLayers.at(i));
        pyPrinter.printNameForward(pLayer, branch);
      } else {
        setInvalid();
        std::string str = "Bilinear need to have 2 input tensor";
        std::cerr << str << std::endl;
        emit notValid(str);
        return;
      }
    } else {
      if (m_backward.find(index) != m_backward.end())
        for (const auto i : m_backward.at(index)) backwardDfs(m_refvLayers.at(i));
      pyPrinter.printNameForward(pLayer, branch);
      if (m_layersIndex[index].size() == 2) pyPrinter.printCopy(index, branch);
    }
  }
}

void GraphManager::clear() noexcept {
  m_backward.clear();
  m_calcucatedDfs.clear();
  m_calcucatedDfsSize.clear();
  m_layersIndex.clear();
  m_layerInputCounter.clear();
}

void GraphManager::init() {
  clear();
  initStartAndFinish();
  if (! m_refvLayers.empty()) branch = Branch{m_layersIndex, m_layerInputCounter}.calculate(iStart);
}

bool GraphManager::isValid() { return m_isValid; }

void GraphManager::initStartAndFinish() {
  m_isValid = true;
  if (m_layersIndex.empty()) return;

  auto iPossibleStarts   = std::count_if(m_layerInputCounter.cbegin(), m_layerInputCounter.cend(),
                                         [](const auto& v) { return v.second == 0; });
  auto iPossibleFinishes = std::count_if(m_layersIndex.cbegin(), m_layersIndex.cend(),
                                         [](const auto& v) { return v.second.size() == 0; });

  switch (iPossibleStarts) {
    case 1 :
      iStart = std::find_if(m_layerInputCounter.cbegin(), m_layerInputCounter.cend(), [](const auto& v) {
                 return v.second == 0;
               })->first;
      break;
    case 0 : {
      m_isValid       = false;
      std::string str = "circular dependensis";
      std::cerr << str << std::endl;
      emit notValid(str);
      break;
    }
    default : {
      m_isValid       = false;
      std::string str = "Too many roots " + std::to_string(iPossibleStarts);
      std::cerr << str << std::endl;
      emit notValid(str);
    }
  }

  switch (iPossibleFinishes) {
    case 1 :
      iFinish = std::find_if(m_layersIndex.cbegin(), m_layersIndex.cend(), [](const auto& v) {
                  return v.second.size() == 0;
                })->first;
      break;
    case 0 : {
      m_isValid       = false;
      std::string str = "circular dependensis ";
      std::cerr << str << std::endl;
      emit notValid(str);
      break;
    }
    default : {
      m_isValid       = false;
      std::string str = "Too many exits " + std::to_string(iPossibleFinishes);
      std::cerr << str << std::endl;
      emit notValid(str);
    }
  }
}

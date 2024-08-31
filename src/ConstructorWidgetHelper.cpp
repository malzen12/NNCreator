#include "ConstructorWidgetHelper.h"

#define ISVALID \
if(!isValid()){ \
  std::cerr << "Graph is not valid" << __LINE__ <<std::endl; \
  return {};}

#define ISVALIDVOID \
if(!isValid()){ \
  std::cerr << "Graph is not valid" << __LINE__ <<std::endl; \
  return ;}

NNLayerWidget* get_layer2(std::size_t sId, std::vector<NNLayerWidget*> vLayers)
{
  auto itLayer = std::find_if(vLayers.begin(), vLayers.end(),
                              [sId](NNLayerWidget* pWdg){
                                return pWdg->getId() == sId + 1;
                              });

  if (vLayers.end() == itLayer)
    throw std::runtime_error("invalid id passed _get_layer2_");

  return *itLayer;
}


std::stringstream& PyPrinter::getStreamRef(){
  return Stream;
}

const std::string PyPrinter::createName(const NNLayerWidget* pParam){
  std::stringstream tempStream;
  tempStream << "self." << pParam->getParams()->getName()
             << "_" << pParam->getId();
  return  tempStream.str();
}

const std::string PyPrinter::createNameEnum(const NNLayerWidget* pParam){
  std::stringstream tempStream;
  tempStream << "self." << pParam->getParams()->getDisplayName().toStdString()
             << "_" << pParam->getId();
  return  tempStream.str();
}

const std::string PyPrinter::createNameMyRecurrent(const NNLayerWidget* pParam){
  std::stringstream tempStream;
  tempStream << "self." << "Linear"
             << "_" << pParam->getId() << "h";
  return  tempStream.str();
}

void PyPrinter::printCopy(const std::size_t index, const BranchReturnValue& branch){
  const auto itFront = branch.vLayersBranch.at(index).cbegin();
  auto it = std::next(itFront);
  for(auto ite = branch.vLayersBranch.at(index).cend(); ite != it; it++){
    *this << "x" << *it
        << " = x" << *itFront
        << ".clone()" << '\n';
  }
}

void PyPrinter::printNameForward(const NNLayerWidget* pLayer, const BranchReturnValue& branch){
  auto pParam = pLayer->getParams();
  auto& pParamParam = pParam->getParams();
  bool isEnum = pParamParam.front().isEnum();
  auto iCurrent = pLayer->getId();
  const auto branchIndex = *branch.vLayersBranch.at(iCurrent).cbegin();
  if(!isEnum){
    if(pParam->getName() == "MyRecurrent"){
      std::size_t n = std::next(pParamParam.crbegin())->getValue().toInt();
      for(std::size_t i = 0; i < n; ++i ){
        *this << "x" << branchIndex
            << " = " << createNameMyRecurrent(pLayer)
            << "(x" << branchIndex
            << ")" << '\n';
      }
    }else if(pParam->getName() == "Bilinear"){
      if(branch.mergerMap.at(iCurrent).size() == 2){
        *this << "x" << branchIndex
            << " = " << createName(pLayer)
            << "(x" << branch.mergerMap.at(iCurrent).front()
            << ", x" << branch.mergerMap.at(iCurrent).back()
            << ")" << '\n';
      }
      else{
        throw(std::runtime_error("Bilinear need 2 input"));
      }
    }
    else{
      *this << "x" << branchIndex
          << " = " << createName(pLayer)
          << "(x" << branchIndex
          << ")" << '\n';
    }

  }else{
    auto enamName = pParamParam.cbegin()->getName();
    if(enamName == "Recurrent"){
      *this << "x" << branchIndex
          << ", y = " << createNameEnum(pLayer)
          << "(x" << branchIndex
          << ")" << '\n';
    }else if(enamName == "Normalization"){
      *this << createNameEnum(pLayer)
          << " = nn." << pParam->getDisplayName().toStdString()
          << R"((x.size()))" << '\n';
      *this << "x" << branchIndex
          << " = " << createNameEnum(pLayer)
          << "(x" << branchIndex
          << ")" << '\n';
    }
  }
}

void PyPrinter::createInitLValue(const NNLayerWidget* pCurrent){
  auto pParam = pCurrent->getParams();
  auto& pParamParam = pParam->getParams();
  bool isEnum = pParamParam.front().isEnum();
  if(!isEnum){
    if(pParam->getName() == "MyRecurrent"){
      *this << createNameMyRecurrent(pCurrent) << '\n';
    }else{
      *this << createName(pCurrent);
    }
  }
  else{
    auto enamName = pParamParam.front().getName();
    if(enamName == "Recurrent"){
      *this << createNameEnum(pCurrent);
    }else if(enamName == "Normalization"){
      return;
    }
  }
}

void PyPrinter::createInitRValue(const NNLayerWidget* pCurrent){
  auto pParam = pCurrent->getParams();
  auto& pParamParam = pParam->getParams();
  bool isEnum = pParamParam.front().isEnum();
  if(!isEnum){
    if(pParam->getName() == "MyRecurrent"){
      auto it = pParamParam.cbegin();
      Stream << " = nn." << "Linear" << "("
             << it->makePyString() << ", "
             << it->makePyString() << ")\n";
    }else{
      auto enamName = pParamParam.front().getName();
      if(enamName == "Normalization"){
        return;
      }

      Stream << " = nn." << pParam->getName()
             << "(";
      if(!pParamParam.empty()){
        auto it = pParamParam.cbegin();
        Stream << it->getHiddenName() << '='
               << it->makePyString();
        if(pParamParam.size() > 1){
          ++it;
          for(auto ite = pParamParam.cend(); std::next(it) != ite; ++it){
            Stream << ", " << it->getHiddenName()
                   << '=' << it->makePyString();
          }
        }
      }
      Stream << ")\n";
    }
  }
  else{
    auto it = pParamParam.cbegin();
    Stream << " = nn."
           << pParam->getDisplayName().toStdString()
           << "("
           << it++->makePyString() << ", "
           << it++->makePyString() << ", "
           << it++->makePyString() << ", ";
    if(pParamParam.front().getValue() == "RNN")
      Stream << R"('tanh')" << ", ";
    Stream << it++->makePyString() << ", "
           << R"(False, 0.0)" << ", "
           << it->makePyString() << ")\n";
  }
}














GraphManager::GraphManager(const std::unordered_map<std::size_t, NNLayerWidget*>& m_vLayers)
  :m_refvLayers(m_vLayers){
  init();
}

const std::string GraphManager::print(){
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

std::size_t GraphManager::getFinishIndex(){
  return iFinish;
}

std::size_t GraphManager::getStartIndex(){
  return iStart;
}

void GraphManager::addEdge(std::size_t from, std::size_t to){
  m_vLayersIndex[from].insert(to);
  ++m_vLayerInputCounter[to];
  m_backward[to].insert(from);
}

void GraphManager::addVertex(std::size_t index){
  m_backward[index].insert({});
  m_vLayersIndex[index].insert({});
  m_vLayerInputCounter[index] = 0;
  if(m_refvLayers.empty()){
    iStart = iFinish = 0;
  }
}

void GraphManager::deleteEdge(std::size_t from, std::size_t to){
  m_vLayersIndex[from].erase(m_vLayersIndex[from].find(to));
  --m_vLayerInputCounter[to];
  m_backward[to].erase(std::find(m_backward[to].cbegin(), m_backward[to].cend(), from));
}

void GraphManager::deleteVertex(std::size_t index){
  for(const auto i:m_vLayersIndex[index]){
    m_backward[i].erase(std::find(m_backward[i].cbegin(), m_backward[i].cend(), index));
    --m_vLayerInputCounter[i];
  }
  for(const auto i:m_backward[index]){
    m_vLayersIndex[i].erase(std::find(m_vLayersIndex[i].cbegin(), m_vLayersIndex[i].cend(), index));
  }
  m_vLayersIndex.erase(index);
  m_vLayerInputCounter.erase(index);
  m_backward.erase(index);
}

void GraphManager::checkSizes(const std::vector<std::size_t>& vInputSize)
{
  initStartAndFinish();
  ISVALIDVOID
  if(m_refvLayers.empty())
    return;
  m_refvLayers.at(iStart)->addInputSize(vInputSize);
  m_calcucatedDfsSize.clear();
  m_calcucatedDfsSize.insert(iStart);
  backwardDfsForSizes(m_refvLayers.at(iFinish));
}

void GraphManager::backwardDfsForSizes(NNLayerWidget* pCurrent)
{
  ISVALIDVOID
  if(m_calcucatedDfsSize.size() == m_refvLayers.size())
    return;
  if(!isValid())
    return;
  auto iCurrent = pCurrent->getId();
  if(pCurrent->getParams()->getName() == "Bilinear"){
    if(m_calcucatedDfsSize.find(iCurrent) == m_calcucatedDfsSize.end()){
      m_calcucatedDfsSize.insert(iCurrent);
      auto it = m_backward.at(iCurrent).cbegin();
      auto first = *it++;
      backwardDfsForSizes(m_refvLayers.at(first));
      pCurrent->addInputSize(m_refvLayers.at(first)->calcOutputSize());
      if(m_backward.at(iCurrent).size() == 2){
        auto second = *it;
        backwardDfsForSizes(m_refvLayers.at(second));
        pCurrent->addInputSize(m_refvLayers.at(second)->calcOutputSize());
      }else{
        setInvalid();
        std::string str = "Bilinear need to have 2 input tensor";
        std::cerr << str << std::endl;
        emit notValid(str);
        return;
      }
    }
  }else{
    if(m_calcucatedDfsSize.find(iCurrent) == m_calcucatedDfsSize.end()){
      m_calcucatedDfsSize.insert(iCurrent);
      if(m_backward.find(iCurrent) != m_backward.end()){
        if(m_backward.at(iCurrent).size() > 0){
          auto it = *m_backward.at(iCurrent).cbegin();
          backwardDfsForSizes(m_refvLayers.at(it));
          pCurrent->addInputSize(m_refvLayers.at(it)->calcOutputSize());
        }
      }
    }
  }
}

void GraphManager::setInvalid(){
  m_isValid = false;
}



void GraphManager::printInit(){
  for(const auto [id, pCurrent]: m_refvLayers){
    pyPrinter.createInitLValue(pCurrent);
    pyPrinter.createInitRValue(pCurrent);
  }
  pyPrinter.endl();
}

void GraphManager::printForward(){
  m_calcucatedDfs.clear();
  if(!m_refvLayers.empty()){
    branch = Branch{m_vLayersIndex, m_vLayerInputCounter}.calculate(iStart);
  }
  backwardDfs(m_refvLayers.at(iFinish));
}

void GraphManager::backwardDfs(const NNLayerWidget* pCurrent)
{
  if(m_calcucatedDfs.size() == m_refvLayers.size())
    return;
  if(!isValid())
    return;
  auto iCurrent = pCurrent->getId();
  if(pCurrent->getParams()->getName() == "Bilinear"){
    if(m_calcucatedDfs.find(iCurrent) == m_calcucatedDfs.end()){
      m_calcucatedDfs.insert(iCurrent);
      auto it = m_backward.at(iCurrent).cbegin();
      std::size_t first = *it++;
      backwardDfs(m_refvLayers.at(first));
      if(m_backward.at(iCurrent).size() == 2){
        std::size_t second = *it;
        backwardDfs(m_refvLayers.at(second));
        pyPrinter.printNameForward(pCurrent, branch);
      }else{
        setInvalid();
        std::string str = "Bilinear need to have 2 input tensor";
        std::cerr << str << std::endl;
        emit notValid(str);
        return;
      }
    }
  }else{
    if(m_calcucatedDfs.find(iCurrent) == m_calcucatedDfs.end()){
      m_calcucatedDfs.insert(iCurrent);
      if(m_backward.find(iCurrent) != m_backward.end()){
        if(m_backward.at(iCurrent).size() > 0){
          std::size_t it = *m_backward.at(iCurrent).cbegin();
          backwardDfs(m_refvLayers.at(it));
        }
      }
      pyPrinter.printNameForward(pCurrent, branch);
      if(m_vLayersIndex[iCurrent].size() == 2){
        pyPrinter.printCopy(iCurrent, branch);
      }
    }
  }
}

void GraphManager::clear()
{
  m_backward.clear();
  m_calcucatedDfs.clear();
  m_calcucatedDfsSize.clear();
  m_vLayersIndex.clear();
  m_vLayerInputCounter.clear();
}


void GraphManager::init(){
  clear();
  initContainers();
  initStartAndFinish();
  if(!m_refvLayers.empty()){
    branch = Branch{m_vLayersIndex, m_vLayerInputCounter}.calculate(iStart);
  }
}

void GraphManager::initContainers(){
  for(const auto [id, pCurrent]: m_refvLayers)
    for(const auto pNext: pCurrent->getForward())
      m_backward[pNext->getId()].insert(id);
  for(const auto [id, pCurrent]: m_refvLayers)
    for(const auto pNext: pCurrent->getForward())
      m_vLayersIndex[id].insert(pNext->getId());
  for(auto i = 0u; i < m_vLayersIndex.size(); ++i)
    m_vLayerInputCounter[i] = m_backward[i].size();
}

bool GraphManager::isValid(){
  return m_isValid;
}

void GraphManager::initStartAndFinish(){
  m_isValid = true;
  if(!m_vLayersIndex.empty()){

    auto iPossibleStarts = std::count_if(m_vLayerInputCounter.cbegin(), m_vLayerInputCounter.cend(), [](const auto& v){
      return v.second == 0;
    });
    auto iPossibleFinishes = std::count_if(m_vLayersIndex.cbegin(), m_vLayersIndex.cend(), [](const auto& v){
      return v.second.size() == 0;
    });

    switch(iPossibleStarts){
      case 1:
        iStart = std::find_if(m_vLayerInputCounter.cbegin(), m_vLayerInputCounter.cend(), [](const auto& v){
              return v.second == 0;
            })->first;
        break;
      case 0:{
        m_isValid = false;
        std::string str = "circular dependensis";
        std::cerr << str << std::endl;
        emit notValid(str);
        //          throw std::runtime_error("circular dependensis");
        break;
      }
      default:{
        m_isValid = false;
        std::string str = "Too many roots " + std::to_string(iPossibleStarts);
        std::cerr << str << std::endl;
        emit notValid(str);
        //          throw std::runtime_error("Too many roots");
      }
    }
    switch(iPossibleFinishes){
      case 1:

        iFinish = std::find_if(m_vLayersIndex.cbegin(), m_vLayersIndex.cend(), [](const auto& v){
          return v.second.size() == 0;
        })->first;
        break;
      case 0:{
        m_isValid = false;
        std::string str = "circular dependensis ";
        std::cerr << str << std::endl;
        emit notValid(str);
        //          throw std::runtime_error("circular dependensis");
        break;
      }
      default:{
        m_isValid = false;
        std::string str = "Too many exits " + std::to_string(iPossibleFinishes);;
        std::cerr << str << std::endl;
        emit notValid(str);
        //          throw std::runtime_error("Too many exits");
      }
    }
  }
}

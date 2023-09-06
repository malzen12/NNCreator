#include "MainWidget.h"

#include <fstream>
#include <sstream>

#include <QHBoxLayout>

std::string pop_path(std::vector<NNParam>& vParams, const std::string& strName)
{
    auto itPath = std::find_if(vParams.begin(), vParams.end(), [&strName](const NNParam& crParam)
    {
        return strName == crParam.getName();
    });

    if (vParams.end() == itPath)
        return "";

    auto strPath = itPath->getValue().toString().toStdString();

    vParams.erase(itPath);

    return strPath;
}

MainWidget::MainWidget()
    : m_pGlobalSettingsWidget{new GlobalSettingsWidget},
      m_pConstructorWidget{new ConstructorWidget},
      m_pSettingsEdit{new ParamsEditorWidget}
{
    initGUI();
    createConnections();
}

void MainWidget::onWriteNetXml(const std::vector<NNParam>& vParams) const
{
    auto vParamsCopy = vParams;
    auto strPath = pop_path(vParamsCopy, "NN Xml path");

    writeXml(strPath, m_pConstructorWidget->makeXmlString());
}

void MainWidget::onWriteTrainXml(const std::vector<NNParam>& vParams) const
{
    auto vParamsCopy = vParams;
    auto strPath = pop_path(vParamsCopy, "Train Xml path");

    std::stringstream Stream;
    for (const auto& crParam : vParamsCopy)
        Stream << crParam.makeXmlString();

    writeXml(strPath, Stream.str());
}

void MainWidget::initGUI()
{
    auto pLayout = new QHBoxLayout{this};

    pLayout->addWidget(m_pGlobalSettingsWidget, 10);
    pLayout->addWidget(m_pConstructorWidget, 80);
    pLayout->addWidget(m_pSettingsEdit, 10);

    resize(1500, 1000);
}

void MainWidget::createConnections()
{
    auto bRes = true;

    bRes &= static_cast<bool>(connect(m_pConstructorWidget, SIGNAL(paramsChanged(std::shared_ptr<NNLayerParams>)), m_pSettingsEdit, SLOT(onSetParams(std::shared_ptr<NNLayerParams>))));

    bRes &= static_cast<bool>(connect(m_pSettingsEdit, SIGNAL(paramsChanged(std::shared_ptr<NNLayerParams>)), m_pConstructorWidget, SLOT(onSetParams(std::shared_ptr<NNLayerParams>))));
    bRes &= static_cast<bool>(connect(m_pSettingsEdit, SIGNAL(deleteActive()), m_pConstructorWidget, SLOT(onDeleteActive())));

    bRes &= static_cast<bool>(connect(m_pGlobalSettingsWidget, SIGNAL(makeNetXml(std::vector<NNParam>)), SLOT(onWriteNetXml(std::vector<NNParam>))));
    bRes &= static_cast<bool>(connect(m_pGlobalSettingsWidget, SIGNAL(makeTrainXml(std::vector<NNParam>)), SLOT(onWriteTrainXml(std::vector<NNParam>))));

    bRes &= static_cast<bool>(connect(m_pGlobalSettingsWidget, SIGNAL(inputSizeChanged(std::vector<std::size_t>)), m_pConstructorWidget, SLOT(onSetInputSize(std::vector<std::size_t>))));

    assert(bRes);
}

void MainWidget::writeXml(const std::string& strXmlPath, const std::string& strXmlBody) const
{
    std::fstream FStream{strXmlPath, std::fstream::out};
    assert(FStream.is_open());

    FStream << "<?Stream version=\"1.0\"?>" << std::endl;
    FStream << "<data>" << std::endl;

    FStream << strXmlBody;

    FStream << "</data>" << std::endl;
}

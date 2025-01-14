#include "ParamsEditorWidget.h"

#include <QHBoxLayout>
#include <QToolButton>

#include "Macros.h"
#include "ParamWidget.h"
#include "qscrollarea.h"

Edge::Edge(KeyType id)
    : m_pButton{new QToolButton}
    , m_pLabel{new QLabel{QString::number(id)}}
    , m_id{id} {
  addWidget(m_pButton);
  addWidget(m_pLabel);
  createConnections();
}

void Edge::createConnections() {
  // auto bRes = true;
  auto func = [&]() {
    emit deletedEdge(m_id);
    m_pButton->close();
    m_pLabel->close();
    delete this;
  };
  CONNECT_CHECK(connect(m_pButton, &QToolButton::clicked, func))
}

ParamsEditorWidget::ParamsEditorWidget()
    : m_pNameLabel{new QLabel}
    , m_pForwardLayout{new QVBoxLayout}
    , m_pParamsLayout{new QVBoxLayout}
    , m_pConfirm{new QPushButton{"Confirm"}}
    , m_pDelete{new QPushButton{"Delete"}}
    , m_pErrors{new QLabel} {
  initGUI();
  createConnections();
}

void ParamsEditorWidget::onForward(const HashSet& forward) {
  initForward(forward);
  update();
}

void ParamsEditorWidget::onSetParams(const std::shared_ptr<NNLayerParams>& spParams) {
  m_spParams = spParams;
  initEditors();
  update();
}

void ParamsEditorWidget::onSetErrors(const std::string& strErrors) {
  int width{0}, height{0};
  auto qstrError = QString::fromStdString(strErrors);
  const auto list = qstrError.split('\n');
  for (const auto& qstr : list) {
    width = std::max(width, fontMetrics().horizontalAdvance(qstr));
    height += fontMetrics().height();
  }
  m_pErrors->setText(qstrError);
  m_pErrors->resize(width, height);
}

void ParamsEditorWidget::onConfirmParams() {
  if (! m_spParams->isValid()) return;
  m_spParams = collectParams();
  emit paramsChanged(m_spParams);
}

void ParamsEditorWidget::onClearForward() {
  while (auto pItem = m_pForwardLayout->takeAt(0)) {
    auto layout = pItem->layout()->takeAt(0);
    auto button = layout->widget();
    button->close();

    layout = pItem->layout()->takeAt(0);
    auto label = layout->widget();
    label->close();
    delete layout;
  }
}

void ParamsEditorWidget::onClearEditors() {
  while (auto pItem = m_pParamsLayout->takeAt(0)) {
    auto pWidget = pItem->widget();
    pWidget->close();
  }
  m_pNameLabel->clear();
}

void ParamsEditorWidget::onClearErrors() { m_pErrors->clear(); }

void ParamsEditorWidget::initGUI() {
  auto pButtonsLayout = new QHBoxLayout;

  pButtonsLayout->addWidget(m_pConfirm);
  pButtonsLayout->addWidget(m_pDelete);

  pButtonsLayout->setContentsMargins(0, 0, 0, 0);

  auto pMainLayout = new QVBoxLayout{this};

  pMainLayout->addWidget(m_pNameLabel);
  pMainLayout->addLayout(m_pForwardLayout);
  pMainLayout->addLayout(m_pParamsLayout);
  pMainLayout->addLayout(pButtonsLayout);

  m_pErrors->setStyleSheet("QLabel {background : white; color : red; }");
  m_pErrors->setScaledContents(true);
  auto pErrors = new QScrollArea;
  pErrors->setWidget(m_pErrors);
  pMainLayout->addWidget(pErrors);

  // pMainLayout->addWidget(m_pErrors);
  pMainLayout->addSpacerItem(new QSpacerItem{10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding});

  pMainLayout->setContentsMargins(0, 0, 0, 0);
}

void ParamsEditorWidget::createConnections() {
  // clang-format off
      CONNECT_CHECK(connect(m_pConfirm, SIGNAL(clicked(bool)), SLOT(onConfirmParams())));
      CONNECT_CHECK(connect(m_pDelete,  SIGNAL(clicked(bool)), SIGNAL(deleteActive())));
  // clang-format on
}

void ParamsEditorWidget::initForward(const HashSet& forward) {
  onClearForward();
  for (const auto id : forward) {
    auto pEdge = new Edge{id};
    CONNECT_CHECK(connect(pEdge, SIGNAL(deletedEdge(KeyType)), SIGNAL(deletedEdge(KeyType))));
    m_pForwardLayout->addLayout(pEdge);
  }
  m_pForwardLayout->update();
}

void ParamsEditorWidget::initEditors() {
  onClearEditors();
  if (m_spParams) m_pNameLabel->setText(QString::fromStdString(m_spParams->getName()));
  for (const auto& spParam : m_spParams->getParams()) //////////
    m_pParamsLayout->addWidget(new ParamWidget{spParam});
  m_pForwardLayout->update();
}

auto ParamsEditorWidget::collectParams() -> const std::shared_ptr<NNLayerParams>& {
  std::vector<NNParam> vParams;
  vParams.reserve(m_pParamsLayout->count());
  for (auto i = 0; i < m_pParamsLayout->count(); ++i) {
    const auto pWidget = static_cast<ParamWidget*>(m_pParamsLayout->itemAt(i)->widget());
    if (pWidget) vParams.push_back(pWidget->collectValue());
  }
  m_spParams->setParams(vParams);
  return m_spParams;
}

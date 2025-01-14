#include "GlobalWarningWidget.h"
#include <QGridLayout>

GlobalWarningWidget::GlobalWarningWidget()
    : m_pWarningLabel{new QLabel}
    , m_pScrollArea{new QScrollArea} {
  auto pLayout = new QGridLayout{this};

  m_pWarningLabel->setText("Warning area");
  m_pWarningLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  m_pScrollArea->setWidget(m_pWarningLabel);
  pLayout->addWidget(m_pScrollArea);
  pLayout->setMargin(0);
}

void GlobalWarningWidget::clearWarnings() {
  m_pWarningLabel->clear();
  width = 0;
  height = 0;
  qstrWarning.clear();
}

void GlobalWarningWidget::collecteWarning(const std::string& warning) {
  QString temp{QString::fromStdString(warning)};
  if (! qstrWarning.isEmpty()) qstrWarning += "\n";
  qstrWarning += temp;
  width = std::max(width, fontMetrics().horizontalAdvance(temp));
  height += fontMetrics().height();
}

void GlobalWarningWidget::printWarnings() {
  if (qstrWarning.isEmpty()) {
    m_pWarningLabel->setText("OK");
    m_pWarningLabel->setStyleSheet("QLabel {background : white; color : green; }");
    width = fontMetrics().horizontalAdvance("OK");
    height = fontMetrics().height();
  } else {
    m_pWarningLabel->setText(qstrWarning);
    m_pWarningLabel->setStyleSheet("QLabel {background : white; color : red; }");
    m_pWarningLabel->resize(width, height);
  }
}

void GlobalWarningWidget::resizeEvent(QResizeEvent*) {
  m_pWarningLabel->setMinimumSize(m_pWarningLabel->parentWidget()->size());
  m_pWarningLabel->resize(width, height);
}

void GlobalWarningWidget::showEvent(QShowEvent*) { resizeEvent(nullptr); }

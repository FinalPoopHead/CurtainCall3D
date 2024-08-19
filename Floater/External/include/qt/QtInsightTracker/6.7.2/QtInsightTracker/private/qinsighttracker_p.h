// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#ifndef QINSIGHTTRACKER_P_H
#define QINSIGHTTRACKER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtInsightTracker/private/qinsighttracker_p_p.h>
#include <QtInsightTracker/private/qinsightconfiguration_p.h>
#include <QtCore/QObject>

QT_BEGIN_NAMESPACE

class QInsightReporter;
class QInsightEventFilter;
class QInsightCategoryFilter;

class Q_INSIGHTTRACKER_EXPORT InsightTrackerImpl
{
    Q_DISABLE_COPY(InsightTrackerImpl)

public:
    static InsightTrackerImpl &instance();

    virtual ~InsightTrackerImpl();

    void
    sendScreenView(const QString &name,
                   const std::optional<InsightTracker::ContextDataDouble> &context = std::nullopt);
    void sendClickEvent(
            const QString &name, const QString &category,
            const std::optional<InsightTracker::ContextDataDouble> &context = std::nullopt);
    void sendEvent(const QString &event, const QString &object,
                   const QString &objectName,
                   const std::optional<InsightTracker::EventCoordinates> &coordinates = std::nullopt,
                   const std::optional<QString> &data = std::nullopt);
    void sendDimensionData(const InsightTracker::DimensionData &data) const;

    void startNewSession();
    void clearCache();
    bool isEnabled() const;
    void setEnabled(bool enabled);
    QInsightConfigurationPrivate *configuration();
    void setCategoryFilter(QInsightCategoryFilter *filter);

private:
    InsightTrackerImpl();
    void init();
    void deinit();

    QInsightConfigurationPrivate m_config;
    QInsightReporter *m_reporter = nullptr;
    std::unique_ptr<QInsightEventFilter> m_eventFilter;
    QThread *m_workerThread = nullptr;
    bool m_initialized = false;
};

QT_END_NAMESPACE

#endif // QINSIGHTTRACKER_P_H

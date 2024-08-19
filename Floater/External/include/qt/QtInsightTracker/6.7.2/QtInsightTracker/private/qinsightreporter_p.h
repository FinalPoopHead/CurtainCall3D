// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#ifndef QINSIGHTREPORTER_H
#define QINSIGHTREPORTER_H

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

#include <QtInsightTracker/private/qinsightstorage_p.h>
#include <QtInsightTracker/private/qinsighttracker_p_p.h>
#include <QtInsightTracker/private/qinsightconfiguration_p.h>
#include <QtInsightTracker/qtinsighttracker-config.h>
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QUrl>

QT_BEGIN_NAMESPACE

class QNetworkAccessManager;

// Snowplow constants
static const QLatin1String SNOWPLOW_SCHEMA("schema");
static const QLatin1String SNOWPLOW_DATA("data");
static const QLatin1String SNOWPLOW_EVENT("e");
static const QLatin1String SNOWPLOW_UNSTRUCTURED_EVENT("ue");
static const QLatin1String SNOWPLOW_EVENT_TIMESTAMP("dtm");
//static const QLatin1String SNOWPLOW_EVENT_SENT_TIMESTAMP("stm");
static const QLatin1String SNOWPLOW_EVENT_ID("eid");
static const QLatin1String SNOWPLOW_TRACKER_VERSION("tv");
static const QLatin1String SNOWPLOW_SESSION_ID("sid");
static const QLatin1String SNOWPLOW_LANGUAGE("lang");
static const QLatin1String SNOWPLOW_PLATFORM("p");
static const QLatin1String SNOWPLOW_CONTEXT("co");
static const QLatin1String SNOWPLOW_APP_ID("aid");
static const QLatin1String SNOWPLOW_TRACKER_NAMESPACE("tna");
static const QLatin1String SNOWPLOW_USER_ID("duid");

static const QLatin1String SNOWPLOW_SCHEMA_CONTEXT("iglu:com.snowplowanalytics.snowplow/contexts/jsonschema/1-0-0");
static const QLatin1String SNOWPLOW_SCHEMA_PAYLOAD_DATA("iglu:com.snowplowanalytics.snowplow/payload_data/jsonschema/1-0-4");
static const QLatin1String SNOWPLOW_SCHEMA_DEVICE("iglu:io.qt/device/jsonschema/1-0-1");
static const QLatin1String SNOWPLOW_SCHEMA_APPLICATION("iglu:io.qt/application/jsonschema/1-0-1");
static const QLatin1String SNOWPLOW_SCHEMA_EVENT_CLICK("iglu:io.qt/event_click/jsonschema/1-0-1");
static const QLatin1String SNOWPLOW_SCHEMA_SCREEN_VIEW("iglu:io.qt/screen_view/jsonschema/1-0-1");
static const QLatin1String SNOWPLOW_SCHEMA_QEVENT("iglu:io.qt/qevent/jsonschema/1-0-1");
static const QLatin1String SNOWPLOW_SCHEMA_CONTEXT_DATA("iglu:io.qt/context_data/jsonschema/1-0-0");
static const QLatin1String SNOWPLOW_SCHEMA_CONTEXT_DIMENSION("iglu:io.qt/context_dimension/jsonschema/1-0-0");

static const QLatin1String QT_TRACKER_VERSION(QT_INSIGHT_TRACKER_VERSION_STR);

static const QLatin1String INSIGHT_SERVER_URL("https://%1/com.snowplowanalytics.snowplow/tp2");
static const int MINIMUM_SYNC_INTERVAL = 1000;

class Q_INSIGHTTRACKER_EXPORT QInsightReporter : public QObject
{
    Q_OBJECT
public:
    QInsightReporter();

    void init(QInsightConfigurationPrivate *config);

    void trackDeviceAndAppInfo();

    void trackScreenView(const QString &name,
            const std::optional<InsightTracker::ContextDataDouble> &context = std::nullopt);
    void trackClickEvent(
            const QString &name, const QString &category,
            const std::optional<InsightTracker::ContextDataDouble> &context = std::nullopt);
    void trackEvent(const QString &event, const QString &object, const QString &objectName,
                    const std::optional<InsightTracker::EventCoordinates> &coordinates = std::nullopt,
                    const std::optional<QString> &data = std::nullopt);
    void trackDimensionData(const InsightTracker::DimensionData &data);

    void startNewSession();
    void clearCache();
    ~QInsightReporter() override;

private:
    QJsonObject createUnstructuredEvent(const QJsonArray &contextArray);
    void startSyncTimer();

    void addEvent(const QJsonArray &contextArray);
    void sync();
    void sendToBackend(const QSet<quint64> &ids, const QByteArray &payload);
    void fetchRemoteConfig();

    const QString m_appName;
    QString m_sessionId;
    QUrl m_syncUrl;
    QUrl m_configUrl;
    QNetworkAccessManager *m_qnam = nullptr;
    QInsightConfigurationPrivate *m_config = nullptr;
    std::unique_ptr<QInsightStorage> m_storage = nullptr;
    QString m_previousScreen;
    QTimer m_syncTimer;
    QTimer m_syncConfigTimer;
};

QT_END_NAMESPACE

#endif // QINSIGHTREPORTER_H

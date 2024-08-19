// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#ifndef QINSIGHTEVENTFILTER_H
#define QINSIGHTEVENTFILTER_H

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

#include <QtInsightTracker/qtinsighttracker_global.h>
#include <QtInsightTracker/private/qinsighttracker_p_p.h>
#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtCore/QMap>

QT_BEGIN_NAMESPACE

class InsightTrackerImpl;
class QInsightCategoryFilter;
class QAccessibleEvent;
class QPlatformAccessibility;

class Q_INSIGHTTRACKER_EXPORT QInsightEventFilter : public QObject
{
    Q_OBJECT
public:
    QInsightEventFilter(InsightTrackerImpl *tracker);
    ~QInsightEventFilter();

    static QString generateName(QObject *receiver);
    static QString objectName(QObject *receiver);

    void setCategoryFilter(QInsightCategoryFilter *filter);

protected:
    bool eventFilter(QObject *receiver, QEvent *event) override;

private:
    bool isTracked(QEvent *event);

private:
    InsightTrackerImpl *m_tracker;
    QInsightCategoryFilter *m_categoryFilter = nullptr;
    QEvent::Type m_type = QEvent::None;
    QString m_receiver;
    QString m_receiverName;
    QString m_data;
    std::optional<InsightTracker::EventCoordinates> m_coordinates;
    quint64 m_timestamp = 0;
    bool m_pendingResize = false;
    bool m_pendingInput = false;
    QMap<int, QString> m_events;
};

QT_END_NAMESPACE

#endif // QINSIGHTEVENTFILTER_H

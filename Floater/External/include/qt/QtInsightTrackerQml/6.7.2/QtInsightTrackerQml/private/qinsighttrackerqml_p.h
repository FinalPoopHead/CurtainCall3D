// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#ifndef INSIGHTTRACKERQML_P_H
#define INSIGHTTRACKERQML_P_H

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

#include <QtInsightTracker/QInsightTracker>
#include <QtInsightTracker/private/qinsightcategoryfilter_p.h>
#include <QtInsightTrackerQml/private/qinsightcategory_p.h>
#include <QtInsightTrackerQml/private/qinsightqmlconfiguration_p.h>
#include <QtInsightTrackerQml/qtinsighttrackerqmlexports.h>
#include <QtCore/QtGlobal>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlInfo>

QT_BEGIN_NAMESPACE

class Q_INSIGHTTRACKERQML_EXPORT QInsightTrackerQml : public QObject, public QInsightCategoryFilter
{
    Q_OBJECT
    QML_NAMED_ELEMENT(InsightTracker)
    QML_ADDED_IN_VERSION(1, 0)
    QML_SINGLETON
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)

public:
    QInsightTrackerQml();
    ~QInsightTrackerQml();

    Q_INVOKABLE void interaction(const QString &name, const QString &category = QString()) const;
    Q_INVOKABLE void interaction(const QString &name, const QString &contextKey,
                                 double contextValue, const QString &category = QString()) const;
    Q_INVOKABLE void transition(const QString &name) const;
    Q_INVOKABLE void transition(const QString &name, const QString &contextKey,
                                double contextValue) const;
    Q_INVOKABLE void dimensionData(const QString &key, QString value) const;

    Q_INVOKABLE void clearCache();
    Q_INVOKABLE void startNewSession();
    Q_INVOKABLE bool isEnabled() const;
    Q_INVOKABLE void setEnabled(bool enabled);

    QString objectCategory(QObject *receiver) const override;

Q_SIGNALS:
    void enabledChanged(bool enabled);
};

QT_END_NAMESPACE

#endif // INSIGHTTRACKERQML_P_H

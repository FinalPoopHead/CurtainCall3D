// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#ifndef QINSIGHTQMLCONFIGURATION_H
#define QINSIGHTQMLCONFIGURATION_H

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

#include <QtInsightTracker/QInsightConfiguration>
#include <QtInsightTrackerQml/qtinsighttrackerqmlexports.h>
#include <QtQml/QQmlEngine>

QT_BEGIN_NAMESPACE

class Q_INSIGHTTRACKERQML_EXPORT QInsightQmlConfiguration : public QInsightConfiguration
{
    Q_OBJECT
    QML_NAMED_ELEMENT(InsightConfiguration)
    QML_ADDED_IN_VERSION(1, 0)

public:
    explicit QInsightQmlConfiguration(QObject *parent = nullptr);

    bool isValid() override;

    QString server() const override;
    void setServer(const QString &server) override;
    QString token() const override;
    void setToken(const QString &token) override;
    QString deviceModel() const override;
    void setDeviceModel(const QString &deviceModel) override;
    QString deviceVariant() const override;
    void setDeviceVariant(const QString &deviceVariant) override;
    QString deviceScreenType() const override;
    void setDeviceScreenType(const QString &deviceScreenType) override;
    QString appBuild() const override;
    void setAppBuild(const QString &appBuild) override;
    QString platform() const override;
    void setPlatform(const QString &platform) override;
    QString storageType() const override;
    void setStorageType(const QString &storageType) override;
    QString storagePath() const override;
    void setStoragePath(const QString &storagePath) override;
    int storageSize() const override;
    void setStorageSize(int storageSize) override;
    int syncInterval() const override;
    void setSyncInterval(int syncInterval) override;
    int batchSize() const override;
    void setBatchSize(int batchSize) override;
    QStringList categories() override;
    void setCategories(const QStringList &categories) override;
    QStringList events() override;
    void setEvents(const QStringList &events) override;
    QString userId() override;
    void setUserId(const QString &userId) override;
    bool remoteConfig() const override;
    void setRemoteConfig(bool enabled) override;
    int remoteConfigInterval() const override;
    void setRemoteConfigInterval(int syncInterval) override;

private:
    QInsightConfiguration *m_config;
};

QT_END_NAMESPACE

#endif // QINSIGHTQMLCONFIGURATION_H

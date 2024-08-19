// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#ifndef QINSIGHTCONFIGURATION_P_H
#define QINSIGHTCONFIGURATION_P_H

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
#include <QtInsightTracker/QInsightConfiguration>
#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE

class QSettings;

class Q_INSIGHTTRACKER_EXPORT QInsightConfigurationPrivate : public QInsightConfiguration
{
    Q_OBJECT
public:
    explicit QInsightConfigurationPrivate(QObject *parent = nullptr);
    ~QInsightConfigurationPrivate() = default;

    bool loadConfig();
    bool applyConfig(const QByteArray &data);

    QDateTime lastSync() const;
    void setLastSync(const QDateTime &time);
    bool lastShutdown() const;

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
    enum ConfigItem {
        Server,
        Token,
        DeviceModel,
        DeviceVariant,
        DeviceScreenType,
        AppBuild,
        Platform,
        StorageType,
        StoragePath,
        StorageSize,
        SyncInterval,
        BatchSize,
        Categories,
        Events,
        UserId,
        RemoteConfig,
        RemoteConfigInterval
    };

    typedef QMap<ConfigItem, QVariant> ConfigMap;

    bool parseConfig(const QByteArray &data, ConfigMap &config);

    void setServer(const QString &server, ConfigMap &config);
    void setToken(const QString &token, ConfigMap &config);
    void setDeviceModel(const QString &deviceModel, ConfigMap &config);
    void setDeviceVariant(const QString &deviceVariant, ConfigMap &config);
    void setDeviceScreenType(const QString &deviceScreenType, ConfigMap &config);
    void setAppBuild(const QString &appBuild, ConfigMap &config);
    void setPlatform(const QString &platform, ConfigMap &config);
    void setStorageType(const QString &storageType, ConfigMap &config);
    void setStoragePath(const QString &storagePath, ConfigMap &config);
    void setStorageSize(int storageSize, ConfigMap &config);
    void setSyncInterval(int syncInterval, ConfigMap &config);
    void setBatchSize(int batchSize, ConfigMap &config);
    void setCategories(const QStringList &categories, ConfigMap &config);
    void setEvents(const QStringList &events, ConfigMap &config);
    void setUserId(const QString &userId, ConfigMap &config);
    void setRemoteConfig(bool enabled, ConfigMap &config);
    void setRemoteConfigInterval(int syncInterval, ConfigMap &config);

private:
    ConfigMap m_localConfig;
    ConfigMap m_remoteConfig;

    QString m_settingsPath;
    QByteArray m_hash;
    QFile m_remoteConfigFile;
    bool m_lastShutdown;
};

QT_END_NAMESPACE

#endif // QINSIGHTCONFIGURATION_P_H

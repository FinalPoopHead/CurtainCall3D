// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#ifndef QINSIGHTCONFIGURATION_H
#define QINSIGHTCONFIGURATION_H

#include <QtInsightTracker/qtinsighttracker_global.h>
#include <QtCore/QObject>
#include <QtCore/QString>

QT_BEGIN_NAMESPACE

class Q_INSIGHTTRACKER_EXPORT QInsightConfiguration : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(QInsightConfiguration)

    Q_PROPERTY(bool valid READ isValid FINAL)
    Q_PROPERTY(QString server READ server WRITE setServer NOTIFY serverChanged FINAL)
    Q_PROPERTY(QString token READ token WRITE setToken NOTIFY tokenChanged FINAL)
    Q_PROPERTY(QString deviceModel READ deviceModel WRITE setDeviceModel NOTIFY deviceModelChanged FINAL)
    Q_PROPERTY(QString deviceVariant READ deviceVariant WRITE setDeviceVariant NOTIFY deviceVariantChanged FINAL)
    Q_PROPERTY(QString deviceScreenType READ deviceScreenType WRITE setDeviceScreenType NOTIFY deviceScreenTypeChanged FINAL)
    Q_PROPERTY(QString appBuild READ appBuild WRITE setAppBuild NOTIFY appBuildChanged FINAL)
    Q_PROPERTY(QString platform READ platform WRITE setPlatform NOTIFY platformChanged FINAL)
    Q_PROPERTY(QString storageType READ storageType WRITE setStorageType NOTIFY storageTypeChanged FINAL)
    Q_PROPERTY(QString storagePath READ storagePath WRITE setStoragePath NOTIFY storagePathChanged FINAL)
    Q_PROPERTY(int storageSize READ storageSize WRITE setStorageSize NOTIFY storageSizeChanged FINAL)
    Q_PROPERTY(int syncInterval READ syncInterval WRITE setSyncInterval NOTIFY syncIntervalChanged FINAL)
    Q_PROPERTY(int batchSize READ batchSize WRITE setBatchSize NOTIFY batchSizeChanged FINAL)
    Q_PROPERTY(QStringList categories READ categories WRITE setCategories NOTIFY categoriesChanged FINAL)
    Q_PROPERTY(QStringList event READ events WRITE setEvents NOTIFY eventsChanged FINAL)
    Q_PROPERTY(QString userId READ userId WRITE setUserId NOTIFY userIdChanged FINAL)
    Q_PROPERTY(int remoteConfigInterval READ remoteConfigInterval WRITE setRemoteConfigInterval NOTIFY remoteConfigIntervalChanged FINAL)
    Q_PROPERTY(bool remoteConfig READ remoteConfig WRITE setRemoteConfig NOTIFY remoteConfigChanged FINAL)

public:
    explicit QInsightConfiguration(QObject *parent = nullptr);
    ~QInsightConfiguration() override = default;

    virtual bool isValid() = 0;

    virtual QString server() const = 0;
    virtual void setServer(const QString &server) = 0;
    virtual QString token() const = 0;
    virtual void setToken(const QString &token) = 0;
    virtual QString deviceModel() const = 0;
    virtual void setDeviceModel(const QString &deviceModel) = 0;
    virtual QString deviceVariant() const = 0;
    virtual void setDeviceVariant(const QString &deviceVariant) = 0;
    virtual QString deviceScreenType() const = 0;
    virtual void setDeviceScreenType(const QString &deviceScreenType) = 0;
    virtual QString appBuild() const = 0;
    virtual void setAppBuild(const QString &appBuild) = 0;
    virtual QString platform() const = 0;
    virtual void setPlatform(const QString &platform) = 0;
    virtual QString storageType() const = 0;
    virtual void setStorageType(const QString &storageType) = 0;
    virtual QString storagePath() const = 0;
    virtual void setStoragePath(const QString &storagePath) = 0;
    virtual int storageSize() const = 0;
    virtual void setStorageSize(int storageSize) = 0;
    virtual int syncInterval() const = 0;
    virtual void setSyncInterval(int syncInterval) = 0;
    virtual int batchSize() const = 0;
    virtual void setBatchSize(int batchSize) = 0;
    virtual QStringList categories() = 0;
    virtual void setCategories(const QStringList &categories) = 0;
    virtual QStringList events() = 0;
    virtual void setEvents(const QStringList &events) = 0;
    virtual QString userId() = 0;
    virtual void setUserId(const QString &userId) = 0;
    virtual bool remoteConfig() const = 0;
    virtual void setRemoteConfig(bool enabled) = 0;
    virtual int remoteConfigInterval() const = 0;
    virtual void setRemoteConfigInterval(int syncInterval) = 0;

Q_SIGNALS:
    void serverChanged(const QString &server);
    void tokenChanged(const QString &token);
    void deviceModelChanged(const QString &deviceModel);
    void deviceVariantChanged(const QString &deviceVariant);
    void deviceScreenTypeChanged(const QString &deviceScreenType);
    void appBuildChanged(const QString &appBuild);
    void platformChanged(const QString &platform);
    void storageTypeChanged(const QString &storageType);
    void storagePathChanged(const QString &storagePath);
    void storageSizeChanged(int storageSize);
    void syncIntervalChanged(int syncInterval);
    void batchSizeChanged(int batchSize);
    void categoriesChanged(const QStringList &categories);
    void eventsChanged(const QStringList &events);
    void userIdChanged(const QString &userId);
    void remoteConfigChanged(bool enabled);
    void remoteConfigIntervalChanged(int syncInterval);
};

QT_END_NAMESPACE

#endif // QINSIGHTCONFIGURATION_H

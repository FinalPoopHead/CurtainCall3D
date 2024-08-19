// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#ifndef QSQLITESTORAGE_H
#define QSQLITESTORAGE_H

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
#include <QtInsightTracker/private/qinsightstorage_p.h>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

QT_BEGIN_NAMESPACE

class Q_INSIGHTTRACKER_EXPORT QSQLiteStorage : public QInsightStorage
{
public:
    explicit QSQLiteStorage(const QString &name = {}, int maxRecords = 0);

    bool open() override;
    bool add(const QByteArray &payload) override;
    bool remove(quint64 id) override;
    bool remove(const QSet<quint64> &ids) override;
    bool removeAll() override;
    QList<EventData> get(qsizetype n, qsizetype offset = 0) override;
    quint64 size() override;

    ~QSQLiteStorage() override;

protected:
    QString m_name;
    int m_maxRecords;
    QSqlDatabase m_db;
    QSqlQuery m_query;
};

QT_END_NAMESPACE

#endif // QSQLITESTORAGE_H

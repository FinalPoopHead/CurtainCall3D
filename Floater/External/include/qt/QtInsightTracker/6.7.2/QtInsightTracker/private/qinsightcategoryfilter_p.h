// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#ifndef QINSIGHTCATEGORYFILTER_P_H
#define QINSIGHTCATEGORYFILTER_P_H

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
#include <QtCore/QObject>
#include <QtCore/QEvent>

QT_BEGIN_NAMESPACE

class Q_INSIGHTTRACKER_EXPORT QInsightCategoryFilter
{
public:
    virtual QString objectCategory(QObject *receiver) const = 0;
};

QT_END_NAMESPACE

#endif // QINSIGHTCATEGORYFILTER_P_H

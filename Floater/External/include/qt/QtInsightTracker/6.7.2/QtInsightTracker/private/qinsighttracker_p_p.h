// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#ifndef QINSIGHTTRACKER_P_P_H
#define QINSIGHTTRACKER_P_P_H

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

#include <QtCore/QObject>

QT_BEGIN_NAMESPACE

namespace InsightTracker {

template <typename T>
struct ContextData
{
    ContextData(QString key, T value)
    {
        this->key = key;
        this->value = value;
    }
    QString key;
    T value;
};
struct EventCoordinates {
    EventCoordinates(int x, int y) {
        this->x = x;
        this->y = y;
    }
    int x;
    int y;
};

typedef ContextData<double> ContextDataDouble;
typedef ContextData<QString> DimensionData;

} // namespace InsightTracker

QT_END_NAMESPACE

#endif // QINSIGHTTRACKER_P_P_H

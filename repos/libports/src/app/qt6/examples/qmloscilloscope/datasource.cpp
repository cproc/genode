// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "datasource.h"

#include <QAreaSeries>
#include <QQuickItem>
#include <QQuickView>
#include <QRandomGenerator>
#include <QtMath>
#include <QXYSeries>

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)

DataSource::DataSource(QQuickView *appViewer, QObject *parent)
    : QObject(parent)
    , m_appViewer(appViewer)
{
    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();

    generateData(0, 5, 1024);
}

void DataSource::update(QAbstractSeries *series)
{
    if (!series)
        return;

    static double apos = 0;
    static double bpos = 0;
    static double cpos = 0;

    auto xySeries = static_cast<QXYSeries *>(series);

    QList<QPointF> points;

    points.reserve(_width);

    for (int x = 0; x < _width; x += 20) {
        double a = sin((3.14 * x + apos) / (_width / 2));
        double b = sin((2.8 * x + bpos) / (_width / 3));
        double c = sin((0.77 * x + cpos) / (_width / 5));
        double y = 0.3 * a + 0.3 * b + 0.3 * c;
        points.append(QPointF(x, y));
    }

    apos -= 9.3;
    bpos += 15.1;
    cpos += 15.12;

    // Use replace instead of clear + append, it's optimized for performance
    xySeries->replace(points);

#if 0
    if (series) {
        auto xySeries = static_cast<QXYSeries *>(series);
        m_index++;
        if (m_index > m_data.count() - 1)
            m_index = 0;

        QList<QPointF> points = m_data.at(m_index);
        // Use replace instead of clear + append, it's optimized for performance
        xySeries->replace(points);
    }
#endif
}

void DataSource::generateData(int type, int rowCount, int colCount)
{
    _width = colCount;

#if 0
    // Remove previous data
    m_data.clear();

    // Append the new data depending on the type
    for (int i(0); i < rowCount; i++) {
        QList<QPointF> points;
        points.reserve(colCount);
        for (int j(0); j < colCount; j++) {
            qreal x(0);
            qreal y(0);
            switch (type) {
            case 0:
                // data with sin + random component
                y = qSin(M_PI / 50 * j) + 0.5 + QRandomGenerator::global()->generateDouble();
                x = j;
                break;
            case 1:
                // linear data
                x = j;
                y = (qreal) i / 10;
                break;
            default:
                // unknown, do nothing
                break;
            }
            points.append(QPointF(x, y));
        }
        m_data.append(points);
    }
#endif
}

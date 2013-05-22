/*
 * \brief  Nitpicker QPA plugin
 * \author Christian Prochaska
 * \date   2013-05-08
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */


#include <QDebug>
#include <QtGui/QPlatformIntegrationPlugin>
#include "qnitpickerintegration.h"

QT_BEGIN_NAMESPACE

class QNitpickerIntegrationPlugin : public QPlatformIntegrationPlugin
{
public:
    QStringList keys() const;
    QPlatformIntegration *create(const QString&, const QStringList&);
};

QStringList QNitpickerIntegrationPlugin::keys() const
{
    QStringList list;
    list << "Nitpicker";
    return list;
}

QPlatformIntegration *QNitpickerIntegrationPlugin::create(const QString& system, const QStringList& paramList)
{
    Q_UNUSED(paramList);
    if (system.toLower() == "nitpicker")
        return new QNitpickerIntegration;

    return 0;
}

Q_EXPORT_STATIC_PLUGIN(QNitpickerIntegrationPlugin)
Q_EXPORT_PLUGIN2(nitpicker, QNitpickerIntegrationPlugin)

QT_END_NAMESPACE

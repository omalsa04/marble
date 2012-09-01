//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2011 Guillaume Martres <smarter@ubuntu.com>
//

#ifndef MARBLE_SATELLITESMODEL_H
#define MARBLE_SATELLITESMODEL_H

#include <QStringList>

#include "TrackerPluginModel.h"

namespace Marble {

class MarbleClock;

class SatellitesModel : public TrackerPluginModel
{
    Q_OBJECT
public:
    SatellitesModel( GeoDataTreeModel *treeModel,
                     const PluginManager *pluginManager,
                     const MarbleClock *clock );

    void loadSettings( const QHash<QString, QVariant> &settings );
    void setPlanet( const QString &lcPlanet );
    void updateVisibility();

    void parseFile( const QString &id, const QByteArray &file );

protected:
    void parseCatalog( const QString &id, const QByteArray &file );
    void parseTLE( const QString &id, const QByteArray &file );

private:
    const MarbleClock *m_clock;
    QStringList m_enabledIds;
    QString m_lcPlanet;
};

} // namespace Marble

#endif // MARBLE_SATELLITESMODEL_H

//
// This file is part of the Marble Desktop Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2008 Torsten Rahn <tackat@kde.org>
// Copyright 2009 Jens-Michael Hoffmann <jensmh@gmx.de>
//


// Own
#include "PluginManager.h"

// Qt
#include <QtCore/QList>
#include <QtCore/QPluginLoader>
#include <QtCore/QTime>

// Local dir
#include "MarbleDirs.h"
#include "MarbleDebug.h"
#include "RenderPlugin.h"
#include "NetworkPlugin.h"
#include "PositionProviderPlugin.h"
#include "AbstractFloatItem.h"

namespace Marble
{

class PluginManagerPrivate
{
 public:
    PluginManagerPrivate()
            : m_pluginsLoaded(false)
    {
    }

    void loadPlugins();

    bool m_pluginsLoaded;
    QList<RenderPlugin *> m_renderPluginTemplates;
    QList<NetworkPlugin *> m_networkPluginTemplates;
    QList<PositionProviderPlugin *> m_positionProviderPluginTemplates;
};

PluginManager::PluginManager( QObject *parent )
    : QObject(parent),
      d( new PluginManagerPrivate() )
{
}

PluginManager::~PluginManager()
{
    // The plugin instances returned by QPluginLoader are shared by all QPluginLoaders.
    // If more than one MarbleWidget is used, deleting them here leads to dangling
    // pointers and double deletions in the other PluginManagers.
    // TODO: According to QPluginLoader::unload deletion happens automatically on application
    // termination, but it should be checked that this really is the case (and remove this TODO)
    // qDeleteAll( d->m_renderPluginTemplates );
    // qDeleteAll( d->m_networkPluginTemplates );
    // qDeleteAll( d->m_positionProviderPluginTemplates );
    delete d;
}

QList<AbstractFloatItem *> PluginManager::createFloatItems() const
{
    QList<AbstractFloatItem *> floatItemList;

    d->loadPlugins();

    QList<RenderPlugin *>::const_iterator i = d->m_renderPluginTemplates.constBegin();
    QList<RenderPlugin *>::const_iterator const end = d->m_renderPluginTemplates.constEnd();
    for (; i != end; ++i) {
        AbstractFloatItem * const floatItem = qobject_cast<AbstractFloatItem *>(*i);
        if ( floatItem ) {
            floatItemList.append( qobject_cast<AbstractFloatItem *>( floatItem->
                                                                     pluginInstance() ));
        }
    }

    return floatItemList;
}

QList<RenderPlugin *> PluginManager::createRenderPlugins() const
{
    QList<RenderPlugin *> result;

    d->loadPlugins();

    QList<RenderPlugin *>::const_iterator i = d->m_renderPluginTemplates.constBegin();
    QList<RenderPlugin *>::const_iterator const end = d->m_renderPluginTemplates.constEnd();
    for (; i != end; ++i) {
        result.append( (*i)->pluginInstance() );
    }
    return result;
}

QList<NetworkPlugin *> PluginManager::createNetworkPlugins() const
{
    QList<NetworkPlugin *> result;

    d->loadPlugins();

    QList<NetworkPlugin *>::const_iterator pos = d->m_networkPluginTemplates.constBegin();
    QList<NetworkPlugin *>::const_iterator const end = d->m_networkPluginTemplates.constEnd();
    for (; pos != end; ++pos ) {
        result.append( (*pos)->newInstance() );
    }
    return result;
}

QList<PositionProviderPlugin *> PluginManager::createPositionProviderPlugins() const
{
    QList<PositionProviderPlugin *> result;

    d->loadPlugins();

    QList<PositionProviderPlugin *>::const_iterator pos = d->m_positionProviderPluginTemplates.constBegin();
    QList<PositionProviderPlugin *>::const_iterator const end = d->m_positionProviderPluginTemplates.constEnd();
    for (; pos != end; ++pos ) {
        result.append( (*pos)->newInstance() );
    }
    return result;
}

/** Append obj to the given plugins list if it inherits both T and U */
template<class T, class U>
bool appendPlugin( QObject * obj, const QString &fileName, QList<T*> &plugins )
{
    if ( qobject_cast<T*>( obj ) && qobject_cast<U*>( obj ) ) {
        Q_ASSERT( obj->metaObject()->superClass() ); // all our plugins have a super class
        mDebug() <<  obj->metaObject()->superClass()->className()
                << "plugin loaded from" << MarbleDirs::pluginPath( fileName );
        T* plugin = qobject_cast<T*>( obj );
        Q_ASSERT( plugin ); // checked above
        plugins.append( plugin );
        return true;
    }

    return false;
}

void PluginManagerPrivate::loadPlugins()
{
    if (m_pluginsLoaded)
    {
        return;
    }

    QTime t;
    t.start();
    mDebug() << "Starting to load Plugins.";

    QStringList pluginFileNameList = MarbleDirs::pluginEntryList( "", QDir::Files );

    MarbleDirs::debug();

    qDeleteAll( m_renderPluginTemplates );
    m_renderPluginTemplates.clear();

    qDeleteAll( m_networkPluginTemplates );
    m_networkPluginTemplates.clear();

    qDeleteAll( m_positionProviderPluginTemplates );
    m_positionProviderPluginTemplates.clear();

    foreach( const QString &fileName, pluginFileNameList ) {
        // mDebug() << fileName << " - " << MarbleDirs::pluginPath( fileName );
        QPluginLoader loader( MarbleDirs::pluginPath( fileName ) );

        QObject * obj = loader.instance();

        if ( obj ) {
            bool isPlugin = appendPlugin<RenderPlugin, RenderPluginInterface>
                       ( obj, fileName, m_renderPluginTemplates );
            isPlugin = isPlugin || appendPlugin<NetworkPlugin, NetworkPluginInterface>
                       ( obj, fileName, m_networkPluginTemplates );
            isPlugin = isPlugin || appendPlugin<PositionProviderPlugin, PositionProviderPluginInterface>
                       ( obj, fileName, m_positionProviderPluginTemplates );
            if ( !isPlugin ) {
                mDebug() << "Plugin failure:" << fileName << "is a plugin, but it does not implement the "
                        << "right interfaces or it was compiled against an old version of Marble. Ignoring it.";
	    }
        } else {
            mDebug() << "Plugin failure:" << fileName << "is not a valid Marble Plugin:"
                     << loader.errorString();
	}
    }

    m_pluginsLoaded = true;
    qDebug("Time elapsed: %d ms", t.elapsed());
}

}

#include "PluginManager.moc"

//
// This file is part of the Marble Desktop Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2009      Bastian Holst <bastianholst@gmx.de>
//

// Self
#include "WikipediaItem.h"

// Qt
#include <QtGui/QAction>
#include <QtCore/QDebug>
#include <QtGui/QIcon>
#include <QtGui/QPixmap>
#include <QtGui/QMouseEvent>
#include <QtWebKit/QWebView>

// Marble
#include "GeoPainter.h"
#include "ViewportParams.h"
#include "GeoSceneLayer.h"

using namespace Marble;

WikipediaItem::WikipediaItem( QObject *parent )
    : AbstractDataPluginItem( parent ),
      m_browser( 0 )
{
    m_action = new QAction( this );
    connect( m_action, SIGNAL( triggered() ), this, SLOT( openBrowser() ) );
}

WikipediaItem::~WikipediaItem() {
    delete m_browser;
}

QString WikipediaItem::name() const {
    return id();
}

QString WikipediaItem::itemType() const {
    return "wikipediaItem";
}
     
bool WikipediaItem::initialized() {
    return true;
}
    
void WikipediaItem::addDownloadedFile( const QString& url, const QString& type ) {
    // There shouldn't be downloaded files for this item
}

bool WikipediaItem::operator<( const AbstractDataPluginItem *other ) const {
    return this->id() < other->id();
}
   
bool WikipediaItem::paint( GeoPainter *painter, ViewportParams *viewport,
                           const QString& renderPos, GeoSceneLayer * layer )
{
    painter->drawPixmap( coordinates(), m_pixmap );
    
    updatePaintPosition( viewport, m_pixmap.size() );
    
    return true;
}

qreal WikipediaItem::longitude() {
    return coordinates().longitude();
}
    
void WikipediaItem::setLongitude( qreal longitude ) {
    GeoDataCoordinates updatedCoordinates = coordinates();
    updatedCoordinates.setLongitude( longitude );
    setCoordinates( updatedCoordinates );
}
    
qreal WikipediaItem::latitude() {
    return coordinates().latitude();
}

void WikipediaItem::setLatitude( qreal latitude ) {
    GeoDataCoordinates updatedCoordinates = coordinates();
    updatedCoordinates.setLatitude( latitude );
    setCoordinates( updatedCoordinates );
}

QUrl WikipediaItem::url() {
    return m_url;
}

void WikipediaItem::setUrl( const QUrl& url ) {
    m_url = url;
}

QUrl WikipediaItem::thumbnailImageUrl() {
    return m_thumbnailImageUrl;
}

void WikipediaItem::setThumbnailImageUrl( const QUrl& thumbnailImageUrl ) {
    m_thumbnailImageUrl = thumbnailImageUrl;
}

QAction *WikipediaItem::action() {
    m_action->setText( id() );
    return m_action;
}

void WikipediaItem::openBrowser( ) {
    if( m_browser ) {
        delete m_browser;
    }
    m_browser = new QWebView();
    m_browser->load( url() );
    m_browser->show();
    connect( m_browser, SIGNAL( titleChanged(QString) ),
             m_browser, SLOT( setWindowTitle(QString) ) );
}

void WikipediaItem::setPixmap( const QPixmap& pixmap ) {
    m_pixmap = pixmap;
}
    
void WikipediaItem::setIcon( const QIcon& icon ) {
    m_action->setIcon( icon );
}

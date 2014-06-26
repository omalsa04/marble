//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2013      Mohammed Nafees <nafees.technocool@gmail.com>
//

#include "GeoDataUpdate.h"
#include "GeoDataTypes.h"

namespace Marble
{

class GeoDataUpdatePrivate
{
public:
    GeoDataUpdatePrivate();
    GeoDataChange* m_change;
    QString m_targetHref;
};

GeoDataUpdatePrivate::GeoDataUpdatePrivate() :
    m_change( 0 ), m_targetHref( "" )
{
}

GeoDataUpdate::GeoDataUpdate() :
    d( new GeoDataUpdatePrivate )
{
}

GeoDataUpdate::GeoDataUpdate( const Marble::GeoDataUpdate &other ) :
    GeoDataObject(), d( new GeoDataUpdatePrivate( *other.d ) )
{
}

GeoDataUpdate &GeoDataUpdate::operator=( const GeoDataUpdate &other )
{
    GeoDataObject::operator =( other );
    *d = *other.d;
    return *this;
}

bool GeoDataUpdate::operator==(const GeoDataUpdate& other) const
{
    bool const changeEmpty = !d->m_change || d->m_change->size() == 0;
    bool const otherChangeEmpty = !other.d->m_change || other.d->m_change->size() == 0;

    if( changeEmpty != otherChangeEmpty ) {
        return false;
    } else if( d->m_change && other.d->m_change && *d->m_change != *other.d->m_change ) {
        return false;
    }

    return d->m_targetHref == other.d->m_targetHref;
}

bool GeoDataUpdate::operator!=(const GeoDataUpdate& other) const
{
    return !this->operator==(other);
}

GeoDataUpdate::~GeoDataUpdate()
{
    delete d;
}

const char *GeoDataUpdate::nodeType() const
{
    return GeoDataTypes::GeoDataUpdateType;
}

QString GeoDataUpdate::targetHref() const
{
    return d->m_targetHref;
}

void GeoDataUpdate::setTargetHref( const QString &targetHref )
{
    d->m_targetHref = targetHref;
}

GeoDataChange* GeoDataUpdate::change() const
{
    return d->m_change;
}

void GeoDataUpdate::setChange( GeoDataChange* change )
{
    delete d->m_change;
    d->m_change = change;
    if ( d->m_change ) {
        d->m_change->setParent( this );
    }
}

}

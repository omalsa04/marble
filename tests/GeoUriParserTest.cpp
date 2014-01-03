//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2014       Levente Kurusa <levex@linux.com>
//

#include <QtTest>
#include <QString>
#include "TestUtils.h"
#include "GeoUriParser.h"

namespace Marble
{

class GeoUriParserTest : public QObject
{
    Q_OBJECT

 private slots:
    void testGeoUri_data();
    void testGeoUri();
};

void GeoUriParserTest::testGeoUri_data()
{
    QTest::addColumn<QString>( "uri" );
    QTest::addColumn<bool>( "valid" );
    QTest::addColumn<double>( "lat" );
    QTest::addColumn<double>( "lon" );
    QTest::addColumn<double>( "alt" );

    addRow() << "geo:-25.0064,153.359,250" << true << -25.0064 << 153.359 << 250.00;
    addRow() << "geo:-25.0064153.359250" << false << 0.00 << 0.00 << 0.00;
    addRow() << "geo-25.0064,153.359,250" << false << 0.00 << 0.00 << 0.00;
    addRow() << "geo:25.0064,-153.359,250" << true << 25.0064 << -153.359 << 250.00;
    addRow() << "25.0064,-153.359,250" << false << 0.00 << 0.00 << 0.00;
    addRow() << "geo:25.0064,-153.359" << true << 25.0064 << -153.359 << 00.00;
    addRow() << "geo:37.786971,-122.399677;u=35" << true << 37.786971 << -122.399677 << 0.00;
    addRow() << "geo:37.786971,-122.399677;crs=Moon-2011" << true << 37.786971 << -122.399677 << 0.00;
    addRow() << "geo:37.786971,-122.399677;crs=Moon-2011,u=25" << true <<  37.786971 << -122.399677 << 0.00;
}

void GeoUriParserTest::testGeoUri()
{
    QFETCH( QString, uri );
    QFETCH( bool, valid );
    QFETCH( double, lat );
    QFETCH( double, lon );
    QFETCH( double, alt );

    GeoUriParser parser( uri );
    bool ret = parser.parse();

    GeoDataCoordinates coords = parser.coordinates();

    double cLat = 0.0, cLon = 0.0, cAlt = 0.0;
    coords.geoCoordinates(cLon, cLat, cAlt, GeoDataCoordinates::Degree);

    QCOMPARE(ret, valid);
    if ( ret == valid ) {
        QCOMPARE(cLat, lat );
        QCOMPARE(cLon, lon);
        QCOMPARE(cAlt, alt );
    }
}


}

QTEST_MAIN( Marble::GeoUriParserTest )

#include "GeoUriParserTest.moc"

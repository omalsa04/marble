//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2009      Andrew Manson <g.real.ate@gmail.com>
// Copyright 2013      Thibaut Gridel <tgridel@free.fr>
//

#ifndef SCENEGRAPHICSITEM_H
#define SCENEGRAPHICSITEM_H

#include <QMouseEvent>

#include "GeoGraphicsItem.h"


namespace Marble
{

class GeoDataPlacemark;

/**
 * @brief This is the base class for all scene graphics included within the
 * annotate plugin. It is not instantiated by itself but it is always used
 * as a part of a derived object.
 */
class SceneGraphicsItem : public GeoGraphicsItem
{
public:
    explicit SceneGraphicsItem( GeoDataPlacemark *placemark );
    ~SceneGraphicsItem();

    enum ActionState {
        // General action states
        Editing,

        // Polygon specific
        DrawingPolygon,
        AddingPolygonHole,
        MergingPolygonNodes,
        AddingPolygonNodes,

        // Placemark specific
        AddingPlacemark,

        // Ground Overlays specific
        AddingOverlay
    };

    /**
     * @brief Some events may lead to particular requests to the widget, so it is the
     * AnnotatePlugin which has to test whether there is any request from this polygon.
     */
    enum MarbleWidgetRequest {
        NoRequest,

        // Polygon specific
        OuterInnerMergingWarning,
        InnerInnerMergingWarning,
        InvalidShapeWarning,
        ShowPolygonRmbMenu,
        ShowNodeRmbMenu,
        StartAnimation,
        RemovePolygonRequest,

        // Placemark specific
        ShowPlacemarkRmbMenu
    };

    /**
     * @brief Pure virtual method which is implemented by concrete scene graphic items
     * and returns true if the item contains the @p eventPos.
     */
    virtual bool containsPoint( const QPoint &eventPos ) const = 0;

    /**
     * @brief Pure virtual method which is implemented by concrete scene graphic items
     * and deals with changes that occur when this item is no longer the item we interact
     * with (by means of mouse events - so far).
     */
    virtual void dealWithItemChange( const SceneGraphicsItem *other ) = 0;

    /**
     * @brief Returns the current state.
     */
    ActionState state() const;

    /**
     * @brief Sets the ActionState of this item. This also calls dealWithStateChange() with
     * a parameter: the previous state.
     */
    void setState( ActionState state );

    /**
     * @brief Returns the widget request.
     */
    MarbleWidgetRequest request() const;

    /**
     * @brief Sets the widget request.
     */
    void setRequest( MarbleWidgetRequest request );

    /**
     * @brief SceneGraphicItem class, when called from one of its derived classes'
     * constructors, takes as a parameter a pointer to the placemark of the graphic
     * element.
     * @return The pointer to the placemark mentioned above.
     */
    const GeoDataPlacemark *placemark() const;

    GeoDataPlacemark *placemark();

    /**
     * @brief This function is used to call the event distributer and makes use of
     * the re-implemented virtual functions which handle the mouse events.
     */
    bool sceneEvent( QEvent *event );

    /**
     * @brief It is used for downcasting a SceneGraphicItem. It returns a const char
     * which is the name of the element's class and is defined within the
     * SceneGraphicsTypes namespace.
     */
    virtual const char *graphicType() const = 0;

protected:
    /**
     * @brief Pure virtual functions which handle the mouse events, all of which are
     * re-implemented in every SceneGraphicItem derived classes.
     */
    virtual bool mousePressEvent( QMouseEvent *event ) = 0;
    virtual bool mouseMoveEvent( QMouseEvent *event ) = 0;
    virtual bool mouseReleaseEvent( QMouseEvent *event ) = 0;

    virtual void dealWithStateChange( SceneGraphicsItem::ActionState previousState ) = 0;

private:
    ActionState         m_state;
    MarbleWidgetRequest m_request;
    GeoDataPlacemark   *m_placemark;
};

}

#endif // SCENEGRAPHICSITEM_H

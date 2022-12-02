/*
 * Copyright (C) 2016 EPFL
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

/**
 * @file CelluloSVGUtil.h
 * @brief Header for SVG file parsing and dumping utils
 * @author Ayberk Özgür
 * @date 2016-07-26
 */

#ifndef CELLULOSVGUTIL_H
#define CELLULOSVGUTIL_H

#include <QObject>
#include <QString>
#include "../../zones/CelluloZone.h"
#include "../../zones/CelluloZonePoint.h"
#include "../../zones/CelluloZoneLine.h"
#include "../../zones/CelluloZoneRectangle.h"
#include "../../zones/CelluloZonePolygon.h"
#include "../../zones/CelluloZoneCircle.h"
#include "../../zones/CelluloZonePolyBezier.h"

namespace Cellulo{

/**
 * @addtogroup util
 * @{
 */

/**
 * @brief Utilities for converting SVG files into CelluloZone description files
 * @singleton
 */
class CelluloSVGUtil : public QObject {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloSVGUtil(QObject* parent = 0);
    ~CelluloSVGUtil();

    /** @endcond */

    //TODO remove choice of type for polybezier or tell also other types

    /**
     * @brief Parses the given SVG file and dumps all paths into the given JSON file in the CelluloZonePolyBezierClosestT type
     *
     * If optimize is true, tries to parsed into a more accurate shape and outputs in the distance type of the more accurately parsed zone.
     *
     * @param inSVGFile Full path to the local SVG file, can be in QUrl form
     * @param outJSONFile Full path to the local JSON file, will be overwritten, can be in QUrl form
     * @param type UNUSED
     * @param name Name prefix for all zones, consecutive zones are suffixed with "_1", "_2" etc
     * @param dpi Dots Per Inch to be used during conversion to millimeters (90 is the default for inkscape)
     * @param optimized Whether to try to extract as lines, points, circles, rectangles or polygons instead of polybezier zones, if possible (default, but EXPERIMENTAL). If disabled, all zones are converted into `POLBEZIERCLOSESTT`.
     * @return Human readable result message
     */
    Q_INVOKABLE static QString dumpAllPathsToJSON(
        const QString& inSVGFile,
        const QString& outJSONFile,
        const QString& type = "POLYBEZIERCLOSESTT",
        const QString& name = "SVG_DUMP_ZONE",
        float dpi = 90.0f,
        bool optimized = true
        );

    /**
     * @brief Parses the given SVG file into zones
     *
     * @param inSVGFile Full path to the local SVG file, can be in QUrl form
     * @param name Name prefix for all zones
     * @param dpi Dots Per Inch to be used during conversion to millimeters (90 is the default for inkscape)
     * @param optimized Whether to try to extract as lines, points, circles, rectangles or polygons instead of polybezier zones, if possible
     * @return Parsed zones (QML-compatible)
     */
    Q_INVOKABLE static QVariantList loadZonesQML(
        const QString& inSVGFile,
        const QString& name,
        float dpi = 90.0f,
        bool optimized = true
        );

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Parses the given SVG file into zones
     *
     * @param inSVGFile Full path to the local SVG file, can be in QUrl form
     * @param name Name prefix for all zones
     * @param dpi Dots Per Inch to be used during conversion to millimeters (90 is the default for inkscape)
     * @param optimized Whether to try to extract as lines, points, circles, rectangles or polygons instead of polybezier zones, if possible
     * @return Parsed zones (not QML-compatible)
     */
    Q_INVOKABLE static QList<Cellulo::CelluloZone*> loadZonesCPP(
        const QString& inSVGFile,
        const QString& name,
        float dpi = 90.0f,
        bool optimized = true
        );

    /** @endcond */

private:

    /**
     * @brief Get most accurate zone from given control points
     *
     * @param controlPoints controlPoints of the Bezier shape
     * @return Extracted zone
     */
    static CelluloZone* getMoreAccurateTypeFromBezier(QList<QVector2D> controlPoints);

    /**
     * @brief Get polygonal points of the Bezier shape
     *
     * @param controlPoints controlPoints of the polygonal Bezier shape (if not polygonal, lose information in the process)
     * @return Points of the polygon
     */
    static QList<QVector2D> getPolyPointsFromBezier(QList<QVector2D> controlPoints);

    /**
     * @brief Gets whether Bezier shape can be considered as a polygonal shape
     *
     * @param controlPoints controlPoints of the Bezier shape
     * @return Whether Bezier shape can be considered as a polygonal shape
     */
    static bool bezierIsPoly(QList<QVector2D> controlPoints);

    /**
     * @brief Gets whether polygon is small enough to be considered as a point
     *
     * @param polyPoints controlPoints of the Bezier shape
     * @return Whether polygon is small enough to be considered as a point
     */
    static bool polyIsSmallEnoughToBeAPoint(QList<QVector2D> polyPoints);

    /**
     * @brief Gets whether polygon can be considered as a point
     *
     * @param polyPoints points of the polygon
     * @return Whether polygon can be considered as a point
     */
    static bool polyIsPoint(QList<QVector2D> polyPoints);

    /**
     * @brief Gets whether polygon can be considered as a line
     *
     * @param polyPoints points of the polygon
     * @return Whether polygon can be considered as a line
     */
    static bool polyIsLine(QList<QVector2D> polyPoints);

    /**
     * @brief Gets whether polygon can be considered as a rectangle
     *
     * @param polyPoints points of the polygon
     * @return Whether polygon can be considered as a rectangle
     */
    static bool polyIsRectangle(QList<QVector2D> polyPoints);

    /**
     * @brief Gets whether polygon can be considered as a circle
     *
     * @param polyPoints points of the polygon
     * @return Whether polygon can be considered as a circle
     */
    static bool polyIsCircle(QList<QVector2D> polyPoints);

    /**
     * @brief Gets whether polygon can be considered as a regular polygon
     *
     * @param polyPoints points of the polygon
     * @return Whether polygon can be considered as a regular polygon
     */
    static bool polyIsRegular(QList<QVector2D> polyPoints);

    /**
     * @brief Construct point zone from the polygon points
     *
     * @param polyPoints points of the polygon
     * @return Constructed point zone
     */
    static CelluloZonePointDistance* getPointFromPolyPoints(QList<QVector2D> polyPoints);

    /**
     * @brief Construct line zone from the polygon points
     *
     * @param polyPoints points of the polygon
     * @return Constructed line zone
     */
    static CelluloZoneLineSegmentDistance* getLineFromPolyPoints(QList<QVector2D> polyPoints);

    /**
     * @brief Construct rectangle zone from the polygon points
     *
     * @param polyPoints points of the polygon
     * @return Constructed rectangle zone
     */
    static CelluloZoneRectangleDistance* getRectangleFromPolyPoints(QList<QVector2D> polyPoints);

    /**
     * @brief Construct circle zone from the polygon points
     *
     * @param polyPoints points of the polygon
     * @return Constructed circle zone
     */
    static CelluloZoneCircleDistance* getCircleFromPolyPoints(QList<QVector2D> polyPoints);

    /**
     * @brief Construct regular polygon zone from the polygon points
     *
     * @param polyPoints points of the polygon
     * @return Constructed regular polygon zone
     */
    static CelluloZoneRegularPolygonDistance* getRegularPolygonFromPolyPoints(QList<QVector2D> polyPoints);

    /**
     * @brief Construct point irregular polygon from the polygon points
     *
     * @param polyPoints points of the polygon
     * @return Constructed irregular polygon  zone
     */
    static CelluloZoneIrregularPolygonDistance* getIrregularPolygonFromPolyPoints(QList<QVector2D> polyPoints);

    /**
     * @brief Construct bezier zone from the control points
     *
     * @param controlPoints controlPoints of the Bezier shape
     * @return Constructed bezier zone
     */
    static CelluloZonePolyBezierClosestT* getBezierFromControlPoints(QList<QVector2D> controlPoints);
};

/** @} */

}

#endif // CELLULOSVGUTIL_H

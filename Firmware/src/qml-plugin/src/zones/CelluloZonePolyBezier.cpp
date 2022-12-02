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
 * @file CelluloZonePolyBezier.cpp
 * @brief Source for composite cubic Bézier curves
 * @author Ayberk Özgür
 * @date 2016-07-21
 */

#include "CelluloZonePolyBezier.h"

#include "../util/math/CelluloMathUtil.h"

#include <QPointF>
#include <QDebug>

namespace Cellulo{

CelluloZonePolyBezier::CelluloZonePolyBezier() : CelluloZone(){
    connect(&curve, SIGNAL(controlPointsChanged()), this, SIGNAL(controlPointsChanged()));
}

QVariantList CelluloZonePolyBezier::getControlPoints() const {
    return curve.getControlPoints();
}

void CelluloZonePolyBezier::setControlPoints(const QVariantList& newControlPoints){
    curve.setControlPoints(newControlPoints);
}

void CelluloZonePolyBezier::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    Q_UNUSED(color);
    Q_UNUSED(canvasWidth);
    Q_UNUSED(canvasHeight);
    Q_UNUSED(physicalWidth);
    Q_UNUSED(physicalHeight);
    painter->setRenderHint(QPainter::Antialiasing);
}

bool CelluloZonePolyBezier::isMouseInside(QVector2D  mousePosition, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    float mouseX = mousePosition.x()/canvasWidth*physicalWidth;
    float mouseY = mousePosition.y()/canvasHeight*physicalHeight;
    return curve.isInside(QVector2D(mouseX, mouseY));
}

void CelluloZonePolyBezier::write(QJsonObject& json){
    CelluloZone::write(json);

    QJsonArray controlPointsArray;
    QJsonObject controlPointObj;
    for(const QVariant& controlPoint : getControlPoints()){
        controlPointObj["x"] = controlPoint.value<QVector2D>().x();
        controlPointObj["y"] = controlPoint.value<QVector2D>().y();
        controlPointsArray.append(controlPointObj);
    }
    json["controlPoints"] = controlPointsArray;
}

void CelluloZonePolyBezier::read(const QJsonObject &json){
    CelluloZone::read(json);

    QVariantList newControlPoints;
    QJsonArray controlPointsArray = json["controlPoints"].toArray();
    for(const QJsonValue& controlPointValue : controlPointsArray){
        const QJsonObject& controlPointObj = controlPointValue.toObject();
        newControlPoints.append(QVariant(QVector2D(controlPointObj["x"].toDouble(), controlPointObj["y"].toDouble())));
    }
    setControlPoints(newControlPoints);
}

void CelluloZonePolyBezier::sendPathToRobot(CelluloBluetooth* robot) const {
    if(curve.isEmpty())
        qDebug() << "CelluloZonePolyBezier::sendPathToRobot(): Path is empty.";
    else{
        QVariantList controlPoints = getControlPoints();
        robot->polyBezierInit(controlPoints[0].value<QVector2D>());
        for(int i=1;i<controlPoints.size();i+=3)
            robot->polyBezierAppend(controlPoints[i].value<QVector2D>(), controlPoints[i + 1].value<QVector2D>(), controlPoints[i + 2].value<QVector2D>());
    }
}

QVector2D CelluloZonePolyBezier::getPoint(qreal t){
    return curve.getPoint(t);
}

QVector2D CelluloZonePolyBezier::getTangent(qreal t){
    return curve.getTangent(t);
}

QVector2D CelluloZonePolyBezier::getNormal(qreal t){
    QVector2D tangent = getTangent(t);
    return QVector2D(-tangent.y(), tangent.x());
}

/**
 * CelluloZonePolyBezierClosestT
 */

CelluloZonePolyBezierClosestT::CelluloZonePolyBezierClosestT() : CelluloZonePolyBezier(){
    type = CelluloZoneTypes::POLYBEZIERCLOSESTT;
}

float CelluloZonePolyBezierClosestT::calculate(float xRobot, float yRobot, float thetaRobot){
    Q_UNUSED(thetaRobot);
    QVector2D dummyv;
    qreal dummyf;
    return curve.getClosest(QVector2D(xRobot, yRobot), dummyv, dummyf);
}

void CelluloZonePolyBezierClosestT::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    CelluloZonePolyBezier::paint(painter, color, canvasWidth, canvasHeight, physicalWidth, physicalHeight);

    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(color), 2, Qt::DotLine));

    QVector2D scale(canvasWidth/physicalWidth, canvasHeight/physicalHeight);

    if(!curve.isEmpty()){
        QPainterPath path;

        QVector<CubicBezier> segments = curve.getSegments();
        path.moveTo((scale*segments[0].getControlPoint(0)).toPointF());
        for(auto const& segment : segments)
            path.cubicTo(
                (scale*segment.getControlPoint(1)).toPointF(),
                (scale*segment.getControlPoint(2)).toPointF(),
                (scale*segment.getControlPoint(3)).toPointF()
                );

        painter->drawPath(path);
    }
}

/**
 * CelluloZonePolyBezierXT
 */

CelluloZonePolyBezierXT::CelluloZonePolyBezierXT() : CelluloZonePolyBezierClosestT(){
    type = CelluloZoneTypes::POLYBEZIERXT;
}

float CelluloZonePolyBezierXT::calculate(float xRobot, float yRobot, float thetaRobot){
    Q_UNUSED(yRobot);
    Q_UNUSED(thetaRobot);

    QVector<CubicBezier> segments = curve.getSegments();

    qreal t = (qreal)segments.size()/2;
    qreal intervalSize = (qreal)segments.size()/4;

    //Assumption that curve is 1-to-1 on the x/y axes, i.e is a function of the form y = f(t), allows us to give an increasing/decreasing direction
    qreal incDecCoeff = segments.last().getPointX(1) >= segments.first().getPointX(0) ? 1 : -1;

    //Outside curve cases
    if(incDecCoeff > 0){
        if(xRobot >= segments.last().getPointX(1))
            return segments.size();
        else if(xRobot <= segments.first().getPointX(0))
            return 0;
    }
    else{
        if(xRobot <= segments.last().getPointX(1))
            return segments.size();
        else if(xRobot >= segments.first().getPointX(0))
            return 0;
    }

    //Binary search for t that gives x
    while(intervalSize >= GET_T_LIM_T){
        qreal localT = t;
        int i = curve.getSegmentIndex(localT);
        qreal currentX = segments[i].getPointX(localT);

        if(currentX > xRobot){
            if(currentX - xRobot <= GET_T_EPSILON)
                break;
            else
                t -= incDecCoeff*intervalSize;
        }
        else{
            if(xRobot - currentX <= GET_T_EPSILON)
                break;
            else
                t += incDecCoeff*intervalSize;
        }

        intervalSize /= 2;
    }

    return t;
}

void CelluloZonePolyBezierXT::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    CelluloZonePolyBezierClosestT::paint(painter, color, canvasWidth, canvasHeight, physicalWidth, physicalHeight);
}

/**
 * CelluloZonePolyBezierYT
 */

CelluloZonePolyBezierYT::CelluloZonePolyBezierYT() : CelluloZonePolyBezierClosestT(){
    type = CelluloZoneTypes::POLYBEZIERYT;
}

float CelluloZonePolyBezierYT::calculate(float xRobot, float yRobot, float thetaRobot){
    Q_UNUSED(xRobot);
    Q_UNUSED(thetaRobot);

    QVector<CubicBezier> segments = curve.getSegments();
    qreal t = (qreal)segments.size()/2;
    qreal intervalSize = (qreal)segments.size()/4;

    //Assumption that curve is 1-to-1 on the x/y axes, i.e is a function of the form y = f(t), allows us to give an increasing/decreasing direction
    qreal incDecCoeff = segments.last().getPointY(1) >= segments.first().getPointY(0) ? 1 : -1;

    //Outside curve cases
    if(incDecCoeff > 0){
        if(yRobot >= segments.last().getPointY(1))
            return segments.size();
        else if(yRobot <= segments.first().getPointY(0))
            return 0;
    }
    else{
        if(yRobot <= segments.last().getPointY(1))
            return segments.size();
        else if(yRobot >= segments.first().getPointY(0))
            return 0;
    }

    //Binary search for t that gives x
    while(intervalSize >= GET_T_LIM_T){
        qreal localT = t;
        int i = curve.getSegmentIndex(localT);
        qreal currentY = segments[i].getPointY(localT);

        if(currentY > xRobot){
            if(currentY - yRobot <= GET_T_EPSILON)
                break;
            else
                t -= incDecCoeff*intervalSize;
        }
        else{
            if(yRobot - currentY <= GET_T_EPSILON)
                break;
            else
                t += incDecCoeff*intervalSize;
        }

        intervalSize /= 2;
    }

    return t;
}

void CelluloZonePolyBezierYT::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    CelluloZonePolyBezierClosestT::paint(painter, color, canvasWidth, canvasHeight, physicalWidth, physicalHeight);
}

/**
 * CelluloZonePolyBezierDistance
 */

CelluloZonePolyBezierDistance::CelluloZonePolyBezierDistance() : CelluloZonePolyBezierClosestT(){
    type = CelluloZoneTypes::POLYBEZIERDISTANCE;
}

float CelluloZonePolyBezierDistance::calculate(float xRobot, float yRobot, float thetaRobot){
    Q_UNUSED(thetaRobot);
    QVector2D dummy;
    qreal dist;
    curve.getClosest(QVector2D(xRobot, yRobot), dummy, dist);
    return dist;
}

void CelluloZonePolyBezierDistance::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    CelluloZonePolyBezierClosestT::paint(painter, color, canvasWidth, canvasHeight, physicalWidth, physicalHeight);
}

/**
 * CelluloZonePolyBezierBorder
 */

CelluloZonePolyBezierBorder::CelluloZonePolyBezierBorder() : CelluloZonePolyBezier(){
    type = CelluloZoneTypes::POLYBEZIERBORDER;
    borderThickness = 0;
}

void CelluloZonePolyBezierBorder::setBorderThickness(qreal newThickness){
    if(borderThickness != newThickness){
        borderThickness = newThickness;
        emit borderThicknessChanged();
        updatePaintedItem();
    }
}

void CelluloZonePolyBezierBorder::write(QJsonObject &json){
    CelluloZonePolyBezier::write(json);

    json["borderThickness"] = borderThickness;
}

void CelluloZonePolyBezierBorder::read(const QJsonObject &json){
    CelluloZonePolyBezier::read(json);

    borderThickness = json["borderThickness"].toDouble();
}

float CelluloZonePolyBezierBorder::calculate(float xRobot, float yRobot, float thetaRobot){
    Q_UNUSED(thetaRobot);
    QVector2D dummy;
    qreal dist;
    curve.getClosest(QVector2D(xRobot, yRobot), dummy, dist);
    return dist <= borderThickness/2 ? 1.0f : 0.0f;
}

void CelluloZonePolyBezierBorder::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    CelluloZonePolyBezier::paint(painter, color, canvasWidth, canvasHeight, physicalWidth, physicalHeight);

    QVector2D scale(canvasWidth/physicalWidth, canvasHeight/physicalHeight);

    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QColor(color), borderThickness*(scale.x() + scale.y())/2, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));

    //Draw segments separately to ensure round caps
    if(!curve.isEmpty()){
        QPainterPath path;

        QVector<CubicBezier> segments = curve.getSegments();
        path.moveTo((scale*segments[0].getControlPoint(0)).toPointF());
        for(auto const& segment : segments)
            path.cubicTo(
                (scale*segment.getControlPoint(1)).toPointF(),
                (scale*segment.getControlPoint(2)).toPointF(),
                (scale*segment.getControlPoint(3)).toPointF()
                );

        painter->drawPath(path);
    }
}

/**
 * CelluloZonePolyBezierInner
 */

CelluloZonePolyBezierInner::CelluloZonePolyBezierInner() : CelluloZonePolyBezier(){
    type = CelluloZoneTypes::POLYBEZIERINNER;
}

float CelluloZonePolyBezierInner::calculate(float xRobot, float yRobot, float thetaRobot){
    Q_UNUSED(thetaRobot);

    return curve.isInside(QVector2D(xRobot, yRobot)) ? 1 : 0;
}

void CelluloZonePolyBezierInner::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    CelluloZonePolyBezier::paint(painter, color, canvasWidth, canvasHeight, physicalWidth, physicalHeight);

    painter->setBrush(QBrush(color));
    painter->setPen(Qt::NoPen);

    QVector2D scale(canvasWidth/physicalWidth, canvasHeight/physicalHeight);

    if(!curve.isEmpty()){
        QPainterPath path;

        QVector<CubicBezier> segments = curve.getSegments();
        path.moveTo((scale*segments[0].getControlPoint(0)).toPointF());
        for(auto const& segment : segments)
            path.cubicTo(
                (scale*segment.getControlPoint(1)).toPointF(),
                (scale*segment.getControlPoint(2)).toPointF(),
                (scale*segment.getControlPoint(3)).toPointF()
                );

        painter->drawPath(path);
    }
}

}

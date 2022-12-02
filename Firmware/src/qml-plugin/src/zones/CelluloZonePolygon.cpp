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
 * @file CelluloZonePolygon.cpp
 * @brief Source for the polygon zone
 * @author Joanna Salathé
 * @date 2016-03-04
 */

#include "CelluloZonePolygon.h"

#include <QPolygon>
#if defined(Q_OS_WIN)
	#define _USE_MATH_DEFINES
	#include <math.h>
#else
	#include <cmath>
#endif

#include "../util/math/CelluloMathUtil.h"

namespace Cellulo{

/**
 * CelluloZonePolygon
 */

CelluloZonePolygon::CelluloZonePolygon() : CelluloZone(){
    maxX = std::numeric_limits<qreal>::min();
    maxY = std::numeric_limits<qreal>::min();
    minX = std::numeric_limits<qreal>::max();
    minY = std::numeric_limits<qreal>::max();
}

void CelluloZonePolygon::updateBounds(){
    for(const QVector2D& vertex : vertices){
        if(vertex.x() < minX)
            minX = vertex.x();
        if(vertex.x() > maxX)
            maxX = vertex.x();
        if(vertex.y() < minY)
            minY = vertex.y();
        if(vertex.y() > maxY)
            maxY = vertex.y();
    }
}

QList<QVector2D> CelluloZonePolygon::convertQVariantToQVector2D(QList<QVariant> newVerticesAsVariants){
    QList<QVector2D> newVertices;
    for (int i = 0; i < newVerticesAsVariants.size(); ++i) {
        QVector2D newVertice = qvariant_cast<QVector2D>(newVerticesAsVariants.at(i));
        newVertices.append(newVertice);
    }
    return newVertices;
}

void CelluloZonePolygon::setVertices(const QList<QVector2D> &newVertices){
    if(newVertices != vertices){
        vertices = newVertices;
        emit verticesChanged();
        updateBounds();
        updatePaintedItem();
    }
}

void CelluloZonePolygon::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    Q_UNUSED(color);
    Q_UNUSED(canvasWidth);
    Q_UNUSED(canvasHeight);
    Q_UNUSED(physicalWidth);
    Q_UNUSED(physicalHeight);
    painter->setRenderHint(QPainter::Antialiasing);
}

bool CelluloZonePolygon::isMouseInside(QVector2D  mousePosition, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    float mouseX = mousePosition.x()/canvasWidth*physicalWidth;
    float mouseY = mousePosition.y()/canvasHeight*physicalHeight;
    return isPointInside(mouseX,mouseY);
}

bool CelluloZonePolygon::isPointInside(float pointX, float pointY){
    if(minX <= pointX && pointX <= maxX && minY <= pointY && pointY <= maxY)
        return CelluloMathUtil::pointInPoly(QVector2D(pointX, pointY), vertices) ? 1 : 0;
    else
        return 0;
}

/**
 * CelluloZoneIrregularPolygon
 */

void CelluloZoneIrregularPolygon::write(QJsonObject& json){
    CelluloZone::write(json);

    QJsonArray verticesArray;
    for(const QVector2D& vertex : vertices){
        QJsonObject vertexObj;
        vertexObj["x"] = vertex.x();
        vertexObj["y"] = vertex.y();
        verticesArray.append(vertexObj);
    }
    json["vertices"] = verticesArray;
}

void CelluloZoneIrregularPolygon::read(const QJsonObject &json){
    CelluloZone::read(json);

    QList<QVector2D> newVertices;
    QJsonArray verticesArray = json["vertices"].toArray();
    for(const QJsonValue& vertexValue : verticesArray){
        const QJsonObject& vertexObj = vertexValue.toObject();
        newVertices.append(QVector2D(vertexObj["x"].toDouble(), vertexObj["y"].toDouble()));
    }
    setVertices(newVertices);
}

void CelluloZoneIrregularPolygon::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    CelluloZonePolygon::paint(painter, color, canvasWidth, canvasHeight, physicalWidth, physicalHeight);
}

/**
 * CelluloZoneIrregularPolygonInner
 */

CelluloZoneIrregularPolygonInner::CelluloZoneIrregularPolygonInner() : CelluloZoneIrregularPolygon(){
    type = CelluloZoneTypes::IRPOLYGONINNER;
}

float CelluloZoneIrregularPolygonInner::calculate(float xRobot, float yRobot, float thetaRobot){
    Q_UNUSED(thetaRobot);

    return isPointInside(xRobot, yRobot) ? 1 : 0;
}

void CelluloZoneIrregularPolygonInner::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    CelluloZoneIrregularPolygon::paint(painter, color, canvasWidth, canvasHeight, physicalWidth, physicalHeight);

    painter->setBrush(QBrush(color));
    painter->setPen(Qt::NoPen);

    qreal horizontalScaleCoeff = canvasWidth/physicalWidth;
    qreal verticalScaleCoeff = canvasHeight/physicalHeight;

    QPolygonF poly;
    for(const QVector2D& vertex : vertices)
        poly.append(QPointF(vertex.x()*horizontalScaleCoeff, vertex.y()*verticalScaleCoeff));

    painter->drawPolygon(poly);
}

/**
 * CelluloZoneIrregularPolygonBorder
 */

CelluloZoneIrregularPolygonBorder::CelluloZoneIrregularPolygonBorder() : CelluloZoneIrregularPolygon(){
    type = CelluloZoneTypes::IRPOLYGONBORDER;
}

void CelluloZoneIrregularPolygonBorder::setBorderThickness(qreal newThickness){
    if(borderThickness != newThickness){
        borderThickness = newThickness;
        emit borderThicknessChanged();
        updatePaintedItem();
    }
}

void CelluloZoneIrregularPolygonBorder::write(QJsonObject &json){
    CelluloZoneIrregularPolygon::write(json);

    json["borderThickness"] = borderThickness;
}

void CelluloZoneIrregularPolygonBorder::read(const QJsonObject &json){
    CelluloZoneIrregularPolygon::read(json);

    borderThickness = json["borderThickness"].toDouble();
}

float CelluloZoneIrregularPolygonBorder::calculate(float xRobot, float yRobot, float thetaRobot){
    Q_UNUSED(thetaRobot);

    if(minX - borderThickness/2 <= xRobot && xRobot <= maxX + borderThickness/2 && minY - borderThickness/2 <= yRobot && yRobot <= maxY + borderThickness/2)
        return CelluloMathUtil::pointToPolyBorderDist(QVector2D(xRobot, yRobot), vertices) <= borderThickness/2 ? 1 : 0;
    else
        return 0;
}

void CelluloZoneIrregularPolygonBorder::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    CelluloZoneIrregularPolygon::paint(painter, color, canvasWidth, canvasHeight, physicalWidth, physicalHeight);

    qreal horizontalScaleCoeff = canvasWidth/physicalWidth;
    qreal verticalScaleCoeff = canvasHeight/physicalHeight;

    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(color), borderThickness*(horizontalScaleCoeff + verticalScaleCoeff)/2));

    QPolygonF poly;
    for(const QVector2D& vertex : vertices)
        poly.append(QPointF(vertex.x()*horizontalScaleCoeff, vertex.y()*verticalScaleCoeff));

    painter->drawPolygon(poly);
}

/**
 * CelluloZoneIrregularPolygonDistance
 */

CelluloZoneIrregularPolygonDistance::CelluloZoneIrregularPolygonDistance() : CelluloZoneIrregularPolygon(){
    type = CelluloZoneTypes::IRPOLYGONDISTANCE;
}

float CelluloZoneIrregularPolygonDistance::calculate(float xRobot, float yRobot, float thetaRobot){
    Q_UNUSED(thetaRobot);
    QVector2D robot(xRobot, yRobot);
    return CelluloMathUtil::pointInPoly(robot, vertices) ? 0 : CelluloMathUtil::pointToPolyBorderDist(robot, vertices);
}

void CelluloZoneIrregularPolygonDistance::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    CelluloZoneIrregularPolygon::paint(painter, color, canvasWidth, canvasHeight, physicalWidth, physicalHeight);

    painter->setBrush(QBrush(color, Qt::Dense5Pattern));
    painter->setPen(Qt::NoPen);

    qreal horizontalScaleCoeff = canvasWidth/physicalWidth;
    qreal verticalScaleCoeff = canvasHeight/physicalHeight;

    QPolygonF poly;
    for(const QVector2D& vertex : vertices)
        poly.append(QPointF(vertex.x()*horizontalScaleCoeff, vertex.y()*verticalScaleCoeff));

    painter->drawPolygon(poly);
}

/**
 * CelluloZoneRegularPolygon
 */

CelluloZoneRegularPolygon::CelluloZoneRegularPolygon() : CelluloZonePolygon(){
    x = 0;
    y = 0;
    r = 0;
    numEdges = 3;
    rotAngle = 0;
}

void CelluloZoneRegularPolygon::setVertices(const QList<QVector2D>& newVertices){
    Q_UNUSED(newVertices);
    qDebug() << "CelluloZoneRegularPolygon::setVertices(): Setting the vertices of a regular polygon directly is not allowed.";
}

void CelluloZoneRegularPolygon::setNumEdges(int newNumEdge) {
    if(newNumEdge > 2 && newNumEdge != numEdges){
        numEdges = newNumEdge;
        emit numEdgesChanged();
        updateVerticesFromRegularPolyParams();
    }
}

void CelluloZoneRegularPolygon::setX(float newX) {
    if(newX != x){
        x = newX;
        emit xChanged();
        updateVerticesFromRegularPolyParams();
    }
}

void CelluloZoneRegularPolygon::setY(float newY) {
    if(newY != y){
        y = newY;
        emit yChanged();
        updateVerticesFromRegularPolyParams();
    }
}

void CelluloZoneRegularPolygon::setR(float newR) {
    if(newR != r){
        r = newR;
        emit rChanged();
        updateVerticesFromRegularPolyParams();
    }
}

void CelluloZoneRegularPolygon::setRotAngle(float newRotAngle) {
    if(newRotAngle != rotAngle){
        rotAngle = newRotAngle;
        emit rotAngleChanged();
        updateVerticesFromRegularPolyParams();
    }
}

void CelluloZoneRegularPolygon::write(QJsonObject &json){
    CelluloZone::write(json);

    json["numEdges"] = numEdges;
    json["x"] = x;
    json["y"] = y;
    json["r"] = r;
    json["rotAngle"] = rotAngle;
}

void CelluloZoneRegularPolygon::read(const QJsonObject &json){
    CelluloZone::read(json);

    numEdges = json["numEdges"].toDouble();
    x = json["x"].toDouble();
    y = json["y"].toDouble();
    r = json["r"].toDouble();
    rotAngle = json["rotAngle"].toDouble();

    updateVerticesFromRegularPolyParams();
}

void CelluloZoneRegularPolygon::updateVerticesFromRegularPolyParams(){
    QList<QVector2D> newVertices;
    qreal rotAngleRadian = rotAngle*M_PI/180;
    for(int i=0; i<numEdges; i++)
        newVertices.append(QVector2D(x + r*cos(2*M_PI*i/numEdges + rotAngleRadian), y + r*sin(2*M_PI*i/numEdges + rotAngleRadian)));
    CelluloZonePolygon::setVertices(newVertices);
}

void CelluloZoneRegularPolygon::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    CelluloZonePolygon::paint(painter, color, canvasWidth, canvasHeight, physicalWidth, physicalHeight);
}

/**
 * CelluloZoneRegularPolygonInner
 */

CelluloZoneRegularPolygonInner::CelluloZoneRegularPolygonInner() : CelluloZoneRegularPolygon(){
    type = CelluloZoneTypes::RPOLYGONINNER;
}

float CelluloZoneRegularPolygonInner::calculate(float xRobot, float yRobot, float thetaRobot){
    Q_UNUSED(thetaRobot);

    return isPointInside(xRobot, yRobot) ? 1 : 0;
}

void CelluloZoneRegularPolygonInner::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    CelluloZoneRegularPolygon::paint(painter, color, canvasWidth, canvasHeight, physicalWidth, physicalHeight);

    painter->setBrush(QBrush(color));
    painter->setPen(Qt::NoPen);

    qreal horizontalScaleCoeff = canvasWidth/physicalWidth;
    qreal verticalScaleCoeff = canvasHeight/physicalHeight;

    QPolygonF poly;
    for(const QVector2D& vertex : vertices)
        poly.append(QPointF(vertex.x()*horizontalScaleCoeff, vertex.y()*verticalScaleCoeff));

    painter->drawPolygon(poly);
}

/**
 * CelluloZoneRegularPolygonBorder
 */

CelluloZoneRegularPolygonBorder::CelluloZoneRegularPolygonBorder() : CelluloZoneRegularPolygon(){
    type = CelluloZoneTypes::RPOLYGONBORDER;
}

void CelluloZoneRegularPolygonBorder::setBorderThickness(qreal newThickness){
    if(borderThickness != newThickness){
        borderThickness = newThickness;
        emit borderThicknessChanged();
        updatePaintedItem();
    }
}

void CelluloZoneRegularPolygonBorder::write(QJsonObject &json){
    CelluloZoneRegularPolygon::write(json);

    json["borderThickness"] = borderThickness;
}

void CelluloZoneRegularPolygonBorder::read(const QJsonObject &json){
    CelluloZoneRegularPolygon::read(json);

    borderThickness = json["borderThickness"].toDouble();
}

float CelluloZoneRegularPolygonBorder::calculate(float xRobot, float yRobot, float thetaRobot){
    Q_UNUSED(thetaRobot);
    if(minX - borderThickness/2 <= xRobot && xRobot <= maxX + borderThickness/2 && minY - borderThickness/2 <= yRobot && yRobot <= maxY + borderThickness/2)
        return CelluloMathUtil::pointToPolyBorderDist(QVector2D(xRobot, yRobot), vertices) <= borderThickness/2 ? 1 : 0;
    else
        return 0;
}

void CelluloZoneRegularPolygonBorder::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    CelluloZoneRegularPolygon::paint(painter, color, canvasWidth, canvasHeight, physicalWidth, physicalHeight);

    qreal horizontalScaleCoeff = canvasWidth/physicalWidth;
    qreal verticalScaleCoeff = canvasHeight/physicalHeight;

    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(color), borderThickness*(horizontalScaleCoeff + verticalScaleCoeff)/2));

    QPolygonF poly;
    for(const QVector2D& vertex : vertices)
        poly.append(QPointF(vertex.x()*horizontalScaleCoeff, vertex.y()*verticalScaleCoeff));

    painter->drawPolygon(poly);
}

/**
 * CelluloZoneRegularPolygonDistance
 */

CelluloZoneRegularPolygonDistance::CelluloZoneRegularPolygonDistance() : CelluloZoneRegularPolygon(){
    type = CelluloZoneTypes::RPOLYGONDISTANCE;
}

float CelluloZoneRegularPolygonDistance::calculate(float xRobot, float yRobot, float thetaRobot){
    Q_UNUSED(thetaRobot);
    QVector2D robot(xRobot, yRobot);
    return CelluloMathUtil::pointInPoly(robot, vertices) ? 0 : CelluloMathUtil::pointToPolyBorderDist(robot, vertices);
}

void CelluloZoneRegularPolygonDistance::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    CelluloZoneRegularPolygon::paint(painter, color, canvasWidth, canvasHeight, physicalWidth, physicalHeight);

    painter->setBrush(QBrush(color, Qt::Dense5Pattern));
    painter->setPen(Qt::NoPen);

    qreal horizontalScaleCoeff = canvasWidth/physicalWidth;
    qreal verticalScaleCoeff = canvasHeight/physicalHeight;

    QPolygonF poly;
    for(const QVector2D& vertex : vertices)
        poly.append(QPointF(vertex.x()*horizontalScaleCoeff, vertex.y()*verticalScaleCoeff));

    painter->drawPolygon(poly);
}

}

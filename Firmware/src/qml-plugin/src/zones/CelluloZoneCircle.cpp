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
 * @file CelluloZoneCircle.cpp
 * @brief Header for circle zone
 * @author Joanna Salathé
 * @date 2016-03-04
 */

#include "CelluloZoneCircle.h"
#include <QQmlEngine>

#include <cmath>

namespace Cellulo{

/**
 * CelluloZoneCircle
 */

CelluloZoneCircle::CelluloZoneCircle() : CelluloZone(){
    x = 0;
    y = 0;
    r = 0;
}

void CelluloZoneCircle::write(QJsonObject &json){
    CelluloZone::write(json);
    json["x"] = x;
    json["y"] = y;
    json["r"] = r;
}

void CelluloZoneCircle::read(const QJsonObject &json){
    CelluloZone::read(json);
    x = json["x"].toDouble();
    y = json["y"].toDouble();
    r = json["r"].toDouble();
}

void CelluloZoneCircle::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    Q_UNUSED(color);
    Q_UNUSED(canvasWidth);
    Q_UNUSED(canvasHeight);
    Q_UNUSED(physicalWidth);
    Q_UNUSED(physicalHeight);
    painter->setRenderHint(QPainter::Antialiasing);
}

bool CelluloZoneCircle::isMouseInside(QVector2D  mousePosition, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    float mouseX = mousePosition.x()/canvasWidth*physicalWidth;
    float mouseY = mousePosition.y()/canvasHeight*physicalHeight;
    return isPointInside(mouseX,mouseY);
}

bool CelluloZoneCircle::isPointInside(float pointX, float pointY){
    return ((pointX-x)*(pointX-x) + (pointY -y)*(pointY -y) <= r*r) ? 1 : 0;
}

void CelluloZoneCircle::setX(float newX){
    if(newX != x){
        x = newX;
        emit(xChanged());
        updatePaintedItem();
    }
}

void CelluloZoneCircle::setY(float newY){
    if(newY != y){
        y = newY;
        emit(yChanged());
        updatePaintedItem();
    }
}

void CelluloZoneCircle::setR(float newR){
    if(newR != r){
        r = newR;
        emit(rChanged());
        updatePaintedItem();
    }
}

/**
 * CelluloZoneCircleInner
 */

CelluloZoneCircleInner::CelluloZoneCircleInner() : CelluloZoneCircle(){
    type = CelluloZoneTypes::CIRCLEINNER;
}

float CelluloZoneCircleInner::calculate(float xRobot, float yRobot, float thetaRobot){
    Q_UNUSED(thetaRobot);
    return isPointInside(xRobot, yRobot) ? 1 : 0;
}

void CelluloZoneCircleInner::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    CelluloZoneCircle::paint(painter, color, canvasWidth, canvasHeight, physicalWidth, physicalHeight);

    painter->setBrush(QBrush(color));
    painter->setPen(Qt::NoPen);

    qreal horizontalScaleCoeff = canvasWidth/physicalWidth;
    qreal verticalScaleCoeff = canvasHeight/physicalHeight;

    painter->drawEllipse(QPointF(x*horizontalScaleCoeff, y*verticalScaleCoeff), r*horizontalScaleCoeff, r*verticalScaleCoeff);
}

/**
 * CelluloZoneCircleBorder
 */

CelluloZoneCircleBorder::CelluloZoneCircleBorder() : CelluloZoneCircle(){
    type = CelluloZoneTypes::CIRCLEBORDER;
}

void CelluloZoneCircleBorder::setBorderThickness(qreal newThickness){
    if(borderThickness != newThickness){
        borderThickness = newThickness;
        emit borderThicknessChanged();
        updatePaintedItem();
    }
}

void CelluloZoneCircleBorder::write(QJsonObject &json){
    CelluloZoneCircle::write(json);

    json["borderThickness"] = borderThickness;
}

void CelluloZoneCircleBorder::read(const QJsonObject &json){
    CelluloZoneCircle::read(json);

    borderThickness = json["borderThickness"].toDouble();
}

float CelluloZoneCircleBorder::calculate(float xRobot, float yRobot, float thetaRobot){
    Q_UNUSED(thetaRobot);
    float robotRSquared = (xRobot - x)*(xRobot - x) + (yRobot - y)*(yRobot - y);
    return ((r - borderThickness/2)*(r - borderThickness/2) <= robotRSquared && robotRSquared <= (r + borderThickness/2)*(r + borderThickness/2)) ? 1 : 0;
}

void CelluloZoneCircleBorder::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    CelluloZoneCircle::paint(painter, color, canvasWidth, canvasHeight, physicalWidth, physicalHeight);

    qreal horizontalScaleCoeff = canvasWidth/physicalWidth;
    qreal verticalScaleCoeff = canvasHeight/physicalHeight;

    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QColor(color), borderThickness*(horizontalScaleCoeff + verticalScaleCoeff)/2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));

    painter->drawEllipse(QPointF(x*horizontalScaleCoeff, y*verticalScaleCoeff), r*horizontalScaleCoeff, r*verticalScaleCoeff);
}

/**
 * CelluloZoneCircleDistance
 */

CelluloZoneCircleDistance::CelluloZoneCircleDistance() : CelluloZoneCircle(){
    type = CelluloZoneTypes::CIRCLEDISTANCE;
}

float CelluloZoneCircleDistance::calculate(float xRobot, float yRobot, float thetaRobot){
    Q_UNUSED(thetaRobot);
    float dist = sqrt((xRobot - x)*(xRobot - x) + (yRobot - y)*(yRobot - y)) - r;
    return dist >= 0 ? dist : 0;
}

void CelluloZoneCircleDistance::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    CelluloZoneCircle::paint(painter, color, canvasWidth, canvasHeight, physicalWidth, physicalHeight);

    painter->setBrush(QBrush(color, Qt::Dense5Pattern));
    painter->setPen(Qt::NoPen);

    qreal horizontalScaleCoeff = canvasWidth/physicalWidth;
    qreal verticalScaleCoeff = canvasHeight/physicalHeight;

    painter->drawEllipse(QPointF(x*horizontalScaleCoeff, y*verticalScaleCoeff), r*horizontalScaleCoeff, r*verticalScaleCoeff);
}

}

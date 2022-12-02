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
 * @file CelluloZoneRectangle.cpp
 * @brief Source for the rectangular zone
 * @author Joanna Salathé
 * @date 2016-03-04
 */

#include "CelluloZoneRectangle.h"

#include "../util/math/CelluloMathUtil.h"

namespace Cellulo{

/**
 * CelluloZoneRectangle
 */

CelluloZoneRectangle::CelluloZoneRectangle() : CelluloZone(){
    x = 0;
    y = 0;
    height = 0;
    width = 0;
}

void CelluloZoneRectangle::setX(float newX) {
    if(newX!=x){
        x = newX;
        emit(xChanged());
        updatePaintedItem();
    }
}

void CelluloZoneRectangle::setY(float newY) {
    if(newY!=y){
        y = newY;
        emit(yChanged());
        updatePaintedItem();
    }
}

void CelluloZoneRectangle::setHeight(float newHeight) {
    if(newHeight!=height){
        height = newHeight;
        emit(heightChanged());
        updatePaintedItem();
    }
}

void CelluloZoneRectangle::setWidth(float newWidth) {
    if(newWidth!=width){
        width = newWidth;
        emit(widthChanged());
        updatePaintedItem();
    }
}

void CelluloZoneRectangle::write(QJsonObject &json){
    CelluloZone::write(json);

    json["x"] = x;
    json["y"] = y;
    json["width"] = width;
    json["height"] = height;
}

void CelluloZoneRectangle::read(const QJsonObject &json){
    CelluloZone::read(json);

    x = json["x"].toDouble();
    y = json["y"].toDouble();
    width = json["width"].toDouble();
    height = json["height"].toDouble();
}

void CelluloZoneRectangle::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    Q_UNUSED(color);
    Q_UNUSED(canvasWidth);
    Q_UNUSED(canvasHeight);
    Q_UNUSED(physicalWidth);
    Q_UNUSED(physicalHeight);
    painter->setRenderHint(QPainter::Antialiasing);
}

bool CelluloZoneRectangle::isMouseInside(QVector2D  mousePosition, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    float mouseX = mousePosition.x()/canvasWidth*physicalWidth;
    float mouseY = mousePosition.y()/canvasHeight*physicalHeight;
    return isPointInside(mouseX,mouseY);
}

bool CelluloZoneRectangle::isPointInside(float pointX, float pointY){
    return x <= pointX && x + width >= pointX && y <= pointY && y + height >= pointY ? true : false;
}

/**
 * CelluloZoneRectangleInner
 */

CelluloZoneRectangleInner::CelluloZoneRectangleInner() : CelluloZoneRectangle(){
    type = CelluloZoneTypes::RECTANGLEINNER;
}

float CelluloZoneRectangleInner::calculate(float xRobot, float yRobot, float thetaRobot){
    Q_UNUSED(thetaRobot);
    return isPointInside(xRobot, yRobot) ? 1 : 0;
}

void CelluloZoneRectangleInner::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    CelluloZoneRectangle::paint(painter, color, canvasWidth, canvasHeight, physicalWidth, physicalHeight);

    painter->setBrush(QBrush(color));
    painter->setPen(Qt::NoPen);

    qreal horizontalScaleCoeff = canvasWidth/physicalWidth;
    qreal verticalScaleCoeff = canvasHeight/physicalHeight;

    painter->drawRect(x*horizontalScaleCoeff, y*verticalScaleCoeff, width*horizontalScaleCoeff, height*verticalScaleCoeff);
}

/**
 * CelluloZoneRectangleBorder
 */

CelluloZoneRectangleBorder::CelluloZoneRectangleBorder() : CelluloZoneRectangle(){
    type = CelluloZoneTypes::RECTANGLEBORDER;
    borderThickness = 0;
}

void CelluloZoneRectangleBorder::setBorderThickness(qreal newThickness){
    if(borderThickness != newThickness){
        borderThickness = newThickness;
        emit borderThicknessChanged();
        updatePaintedItem();
    }
}

void CelluloZoneRectangleBorder::write(QJsonObject &json){
    CelluloZoneRectangle::write(json);

    json["borderThickness"] = borderThickness;
}

void CelluloZoneRectangleBorder::read(const QJsonObject &json){
    CelluloZoneRectangle::read(json);

    borderThickness = json["borderThickness"].toDouble();
}

float CelluloZoneRectangleBorder::calculate(float xRobot, float yRobot, float thetaRobot){
    Q_UNUSED(thetaRobot);
    return
        (x - borderThickness/2 <= xRobot && xRobot <= x + width + borderThickness/2 && y - borderThickness/2 <= yRobot && yRobot <= y + height + borderThickness/2) &&
        !(x + borderThickness/2 <= xRobot && xRobot <= x + width - borderThickness/2 && y + borderThickness/2 <= yRobot && yRobot <= y + height - borderThickness/2);
}

void CelluloZoneRectangleBorder::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    CelluloZoneRectangle::paint(painter, color, canvasWidth, canvasHeight, physicalWidth, physicalHeight);

    qreal horizontalScaleCoeff = canvasWidth/physicalWidth;
    qreal verticalScaleCoeff = canvasHeight/physicalHeight;

    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QColor(color), borderThickness*(horizontalScaleCoeff + verticalScaleCoeff)/2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));

    painter->drawRect(x*horizontalScaleCoeff, y*verticalScaleCoeff, width*horizontalScaleCoeff, height*verticalScaleCoeff);
}

/**
 * CelluloZoneRectangleDistance
 */

CelluloZoneRectangleDistance::CelluloZoneRectangleDistance() : CelluloZoneRectangle(){
    type = CelluloZoneTypes::RECTANGLEDISTANCE;
}

float CelluloZoneRectangleDistance::calculate(float xRobot, float yRobot, float thetaRobot){
    Q_UNUSED(thetaRobot);
    if(x <= xRobot && x + width >= xRobot && y <= yRobot && y + height >= yRobot)
        return 0;
    else{
        qreal distances[4] = {
            CelluloMathUtil::pointToSegmentDist(
                QVector2D(xRobot, yRobot), QVector2D(x, y),                     QVector2D(width + x, y)),
            CelluloMathUtil::pointToSegmentDist(
                QVector2D(xRobot, yRobot), QVector2D(width + x, y),             QVector2D(width + x, height + y)),
            CelluloMathUtil::pointToSegmentDist(
                QVector2D(xRobot, yRobot), QVector2D(width + x, height + y),    QVector2D(x, height + y)),
            CelluloMathUtil::pointToSegmentDist(
                QVector2D(xRobot, yRobot), QVector2D(x, height + y),            QVector2D(x, y))
        };

        qreal min = std::numeric_limits<qreal>::max();
        for(int i=0; i<4; i++)
            if(distances[i] < min)
                min = distances[i];
        return min;
    }
}

void CelluloZoneRectangleDistance::paint(QPainter* painter, QColor color, qreal canvasWidth, qreal canvasHeight, qreal physicalWidth, qreal physicalHeight){
    CelluloZoneRectangle::paint(painter, color, canvasWidth, canvasHeight, physicalWidth, physicalHeight);

    painter->setBrush(QBrush(color, Qt::Dense5Pattern));
    painter->setPen(Qt::NoPen);

    qreal horizontalScaleCoeff = canvasWidth/physicalWidth;
    qreal verticalScaleCoeff = canvasHeight/physicalHeight;

    painter->drawRect(x*horizontalScaleCoeff, y*verticalScaleCoeff, width*horizontalScaleCoeff, height*verticalScaleCoeff);
}

}

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
 * @file CoordSpaceConverter.cpp
 * @brief Source for any space to any space converter
 * @author Ayberk Özgür
 * @date 2018-05-14
 */

#include "CoordSpaceConverter.h"

namespace Cellulo{

CoordSpaceConverter::CoordSpaceConverter(QQuickItem* parent) : QQuickItem(parent){
    inputOrigin = QVector2D(0,0);
    inputSize = QVector2D(1,1);
    outputOrigin = QVector2D(0,0);
    outputSize = QVector2D(1,1);
}

CoordSpaceConverter::~CoordSpaceConverter(){}

void CoordSpaceConverter::setInputOrigin(QVector2D const& inputOrigin){
    this->inputOrigin = inputOrigin;
    emit inputOriginChanged();
}

void CoordSpaceConverter::setInputSize(QVector2D const& inputSize){
    if(inputSize.x() > 0 && inputSize.y() > 0){
        this->inputSize = inputSize;
        emit inputSizeChanged();
    }
    else
        qCritical() << "CoordSpaceConverter::setInputSize(): Space size must have positive width and height!";
}

void CoordSpaceConverter::setOutputOrigin(QVector2D const& outputOrigin){
    this->outputOrigin = outputOrigin;
    emit outputOriginChanged();
}

void CoordSpaceConverter::setOutputSize(QVector2D const& outputSize){
    if(outputSize.x() > 0 && outputSize.y() > 0){
        this->outputSize = outputSize;
        emit outputSizeChanged();
    }
    else
        qCritical() << "CoordSpaceConverter::setOutputSize(): Space size must have positive width and height!";
}

QVector2D CoordSpaceConverter::convert(QVector2D const& input) const{
    return (input - inputOrigin)/inputSize*outputSize + outputOrigin;
}

}

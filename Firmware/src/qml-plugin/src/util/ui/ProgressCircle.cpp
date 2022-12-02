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
 * @file HexTile.cpp
 * @brief Source for a hexagonal tile definition
 * @author Ayberk Özgür
 * @date 2018-04-27
 */

#include "ProgressCircle.h"

#include<QColor>
#include<QSGGeometry>
#include<QSGGeometryNode>
#include<QSGFlatColorMaterial>

#if defined(Q_OS_WIN)
	#define _USE_MATH_DEFINES
	#include <math.h>
#else
	#include <cmath>
#endif

namespace Cellulo{

ProgressCircle::ProgressCircle(QQuickItem* parent) : QQuickItem(parent){
    setWidth(50);
    setHeight(50);

    setFlag(QQuickItem::ItemHasContents, true);

    progress = 0.0;
    color = QColor(66, 179, 232);
    backgroundColor = QColor(212, 212, 212);
    thickness = 0.3;

    connect(this, SIGNAL(progressChanged()),        this, SLOT(update()));
    connect(this, SIGNAL(colorChanged()),           this, SLOT(update()));
    connect(this, SIGNAL(backgroundColorChanged()), this, SLOT(update()));
    connect(this, SIGNAL(thicknessChanged()),       this, SLOT(update()));
}

ProgressCircle::~ProgressCircle(){}

QSGNode* ProgressCircle::updatePaintNode(QSGNode* oldRoot, UpdatePaintNodeData* updatePaintNodeData){
    Q_UNUSED(updatePaintNodeData);

    qreal w = width();
    qreal h = height();

    QSGNode* root = nullptr;
    QSGGeometryNode* foregroundFill = nullptr;
    QSGGeometryNode* backgroundFill = nullptr;
    QSGGeometry* foregroundGeo = nullptr;
    QSGGeometry* backgroundGeo = nullptr;

    //First render
    if(!oldRoot){
        root = new QSGNode();
        foregroundFill = new QSGGeometryNode();
        backgroundFill = new QSGGeometryNode();
        root->appendChildNode(foregroundFill);
        root->appendChildNode(backgroundFill);

        //Background
        backgroundGeo = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 2*(RESOLUTION + 1));
        backgroundGeo->setLineWidth(0);
        backgroundGeo->setDrawingMode(QSGGeometry::DrawTriangleStrip);
        backgroundFill->setGeometry(backgroundGeo);
        backgroundFill->setFlag(QSGNode::OwnsGeometry);
        QSGFlatColorMaterial* backgroundMat = new QSGFlatColorMaterial();
        backgroundMat->setColor(backgroundColor);
        backgroundFill->setMaterial(backgroundMat);
        backgroundFill->setFlag(QSGNode::OwnsMaterial);

        //Foreground
        foregroundGeo = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 2*(RESOLUTION + 1));
        foregroundGeo->setLineWidth(0);
        foregroundGeo->setDrawingMode(QSGGeometry::DrawTriangleStrip);
        foregroundFill->setGeometry(foregroundGeo);
        foregroundFill->setFlag(QSGNode::OwnsGeometry);
        QSGFlatColorMaterial* foregroundMat = new QSGFlatColorMaterial();
        foregroundMat->setColor(color);
        foregroundFill->setMaterial(foregroundMat);
        foregroundFill->setFlag(QSGNode::OwnsMaterial);
    }

    //Not the first render
    else{
        root = oldRoot;
        backgroundFill = static_cast<QSGGeometryNode*>(root->childAtIndex(0));
        foregroundFill = static_cast<QSGGeometryNode*>(root->childAtIndex(1));

        //Background
        backgroundGeo = backgroundFill->geometry();
        backgroundGeo->allocate(2*(RESOLUTION + 1));
        QSGFlatColorMaterial* backgroundMat = static_cast<QSGFlatColorMaterial*>(backgroundFill->material());
        backgroundMat->setColor(backgroundColor);

        //Foreground
        foregroundGeo = foregroundFill->geometry();
        foregroundGeo->allocate(2*(RESOLUTION + 1));
        QSGFlatColorMaterial* foregroundMat = static_cast<QSGFlatColorMaterial*>(foregroundFill->material());
        foregroundMat->setColor(color);
    }

    //Vertices
    QSGGeometry::Point2D* backgroundVertices = backgroundGeo->vertexDataAsPoint2D();
    QSGGeometry::Point2D* foregroundVertices = foregroundGeo->vertexDataAsPoint2D();
    qreal finalBorderX = 0, finalBorderY = 0, finalInnerX = 0, finalInnerY = 0;
    for(int i=0;i<RESOLUTION+1;i++){
        qreal currentProgress = ((qreal)i)/RESOLUTION;
        qreal angle = 2*M_PI*currentProgress - M_PI/2;
        qreal borderX = 0.5*w + 0.5*w*cos(angle);
        qreal borderY = 0.5*h + 0.5*h*sin(angle);
        qreal innerX = 0.5*w + (1.0 - thickness)*0.5*w*cos(angle);
        qreal innerY = 0.5*h + (1.0 - thickness)*0.5*h*sin(angle);

        backgroundVertices[2*i].set(borderX, borderY);
        backgroundVertices[2*i + 1].set(innerX, innerY);

        if(currentProgress < progress){
            foregroundVertices[2*i].set(borderX, borderY);
            foregroundVertices[2*i + 1].set(innerX, innerY);
            finalBorderX = borderX;
            finalBorderY = borderY;
            finalInnerX = innerX;
            finalInnerY = innerY;
        }
        else{
            foregroundVertices[2*i].set(finalBorderX, finalBorderY);
            foregroundVertices[2*i + 1].set(finalInnerX, finalInnerY);
        }
    }

    backgroundFill->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
    foregroundFill->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
    root->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
    return root;
}

}

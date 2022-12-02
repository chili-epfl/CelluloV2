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

#include "HexTile.h"

#include<QDebug>
#include<QColor>
#include<QSGGeometry>
#include<QSGGeometryNode>
#include<QSGFlatColorMaterial>
#include<QMetaObject>
#include<QMetaProperty>
#include<QQmlProperty>

#include<cmath>

#include"HexTileMap.h"

namespace Cellulo{

HexTile::HexTile(QQuickItem* parent) : QQuickItem(parent){
    coords = new AxialHexCoords(this);

    connect(coords, SIGNAL(physicalWidthChanged()),   this, SLOT(recalculateScreenCoords()));
    connect(coords, SIGNAL(qChanged()),               this, SLOT(recalculateScreenCoords()));
    connect(coords, SIGNAL(rChanged()),               this, SLOT(recalculateScreenCoords()));

    sourceLeft = 0;
    sourceTop = 0;
    sourceRight = 0;
    sourceBottom = 0;
    sourceCenterX = 0;
    sourceCenterY = 0;

    stdSourceCoords = nullptr;

    connect(this, SIGNAL(stdSourceCoordsChanged()), this, SLOT(updateFromStdSourceCoords()), Qt::DirectConnection); //Update immediately

    setFlag(QQuickItem::ItemHasContents, true);
    fillColor = QColor(250, 250, 250);
    borderColor = QColor(50, 50, 50);
    borderSize = 0.05;
    connect(this, SIGNAL(fillColorChanged()),   this, SLOT(update()));
    connect(this, SIGNAL(borderColorChanged()), this, SLOT(update()));
    connect(this, SIGNAL(borderSizeChanged()),  this, SLOT(update()));

    setZ(-1); //Tiles go underneath other stuff by default

    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
}

HexTile::~HexTile(){
    delete coords;
}

bool HexTile::sourceContains(QVector2D const& point){
    return
        sourceLeft < point.x() && point.x() <= sourceRight &&
        sourceTop  < point.y() && point.y() <= sourceBottom;
}

QVector2D HexTile::sourceCoordinates(QVector2D const& point){
    return point - QVector2D(sourceLeft + sourceCenterX, sourceTop + sourceCenterY);
}

void HexTile::setStdSourceCoords(Cellulo::HexTileStdSourceCoords* stdSourceCoords){
    if(stdSourceCoords != this->stdSourceCoords){
        if(this->stdSourceCoords)
            this->stdSourceCoords->disconnectHexTileChangedSignals(this);
        this->stdSourceCoords = stdSourceCoords;
        if(this->stdSourceCoords)
            this->stdSourceCoords->connectHexTileChangedSignals(this);
        emit stdSourceCoordsChanged();
    }
}

void HexTile::updateFromStdSourceCoords(){
    if(stdSourceCoords){
        float sourceLeft = stdSourceCoords->getI()*210.0f;
        float sourceRight = sourceLeft;
        float sourceTop = stdSourceCoords->getJ()*260.0f;
        float sourceBottom = sourceTop;
        float sourceCenterX;
        float sourceCenterY;

        if(stdSourceCoords->getU() == 0){
            sourceLeft += 0.0f;
            sourceRight += 105.7f;
            sourceCenterX = 54.3f;
        }
        else{ // == 1
            sourceLeft += 105.7f;
            sourceRight += 210.0f;
            sourceCenterX = 50.0f;
        }

        if(stdSourceCoords->getV() == 0){
            sourceTop += 0.0f;
            sourceBottom += 130.0f;
            sourceCenterY = 67.0f;
        }
        else{ // == 1
            sourceTop += 130.0f;
            sourceBottom += 260.0f;
            sourceCenterY = 67.0f;
        }

        if(this->sourceLeft != sourceLeft){
            this->sourceLeft = sourceLeft;
            emit sourceLeftChanged();
        }
        if(this->sourceRight != sourceRight){
            this->sourceRight = sourceRight;
            emit sourceRightChanged();
        }
        if(this->sourceTop != sourceTop){
            this->sourceTop = sourceTop;
            emit sourceTopChanged();
        }
        if(this->sourceBottom != sourceBottom){
            this->sourceBottom = sourceBottom;
            emit sourceBottomChanged();
        }
        if(this->sourceCenterX != sourceCenterX){
            this->sourceCenterX = sourceCenterX;
            emit sourceCenterXChanged();
        }
        if(this->sourceCenterY != sourceCenterY){
            this->sourceCenterY = sourceCenterY;
            emit sourceCenterYChanged();
        }
    }
}

void HexTile::recalculateScreenCoords(){
    HexTileMap* tileMap = qobject_cast<HexTileMap*>(parentItem());
    if(tileMap){
        QVector2D physicalSize = QVector2D(coords->getPhysicalWidth(), coords->getPhysicalHeight());
        QVector2D screenSize = tileMap->getToScreenSize()->convert(physicalSize);
        setWidth(screenSize.x());
        setHeight(screenSize.y());
        emit widthChanged();
        emit heightChanged();

        QVector2D screenTopLeft = tileMap->getToScreenCoords()->convert(coords->hexOffset() - 0.5f*physicalSize);
        setX(screenTopLeft.x());
        setY(screenTopLeft.y());
        emit xChanged();
        emit yChanged();
    }
}

void HexTile::itemChange(ItemChange change, const ItemChangeData& value){
    if(change == ItemChange::ItemParentHasChanged){
        HexTileMap* tileMap = qobject_cast<HexTileMap*>(parentItem());
        if(tileMap){
            recalculateScreenCoords();
            connect(tileMap, SIGNAL(markedDirty()), this, SLOT(recalculateScreenCoords()));
        }
    }
    QQuickItem::itemChange(change, value);
}

QSGNode* HexTile::updatePaintNode(QSGNode* oldRoot, UpdatePaintNodeData* updatePaintNodeData){
    Q_UNUSED(updatePaintNodeData);

    qreal w = width();
    qreal h = height();

    QSGNode* root = nullptr;
    QSGGeometryNode* hexFill = nullptr;
    QSGGeometry* hexFillGeo = nullptr;
    QSGGeometryNode* hexBorder = nullptr;
    QSGGeometry* hexBorderGeo = nullptr;
    qreal borderWidth = w*borderSize;

    //First render
    if(!oldRoot){
        root = new QSGNode();
        hexFill = new QSGGeometryNode();
        hexBorder = new QSGGeometryNode();
        root->appendChildNode(hexFill);
        root->appendChildNode(hexBorder);

        //Hex
        hexFillGeo = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 6);
        hexFillGeo->setLineWidth(0);
        hexFillGeo->setDrawingMode(QSGGeometry::DrawTriangleFan);
        hexFill->setGeometry(hexFillGeo);
        hexFill->setFlag(QSGNode::OwnsGeometry);
        QSGFlatColorMaterial* hexFillMat = new QSGFlatColorMaterial();
        hexFillMat->setColor(fillColor);
        hexFill->setMaterial(hexFillMat);
        hexFill->setFlag(QSGNode::OwnsMaterial);

        //Border
        hexBorderGeo = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 14);
        hexBorderGeo->setLineWidth(0);
        hexBorderGeo->setDrawingMode(QSGGeometry::DrawTriangleStrip);
        hexBorder->setGeometry(hexBorderGeo);
        hexBorder->setFlag(QSGNode::OwnsGeometry);
        QSGFlatColorMaterial* hexBorderMat = new QSGFlatColorMaterial();
        hexBorderMat->setColor(borderColor);
        hexBorder->setMaterial(hexBorderMat);
        hexBorder->setFlag(QSGNode::OwnsMaterial);
    }

    //Not the first render
    else{
        root = oldRoot;
        hexFill = static_cast<QSGGeometryNode*>(root->childAtIndex(0));
        hexBorder = static_cast<QSGGeometryNode*>(root->childAtIndex(1));

        //Hex polygon in the background
        hexFillGeo = hexFill->geometry();
        hexFillGeo->allocate(6);
        QSGFlatColorMaterial* hexFillMat = static_cast<QSGFlatColorMaterial*>(hexFill->material());
        hexFillMat->setColor(fillColor);

        //Hex border
        hexBorderGeo = hexBorder->geometry();
        hexBorderGeo->allocate(14);
        QSGFlatColorMaterial* hexBorderMat = static_cast<QSGFlatColorMaterial*>(hexBorder->material());
        hexBorderMat->setColor(borderColor);
    }

    //Hex fill (6 vertices are still enough with triangle fan...)
    QSGGeometry::Point2D* vertices = hexFillGeo->vertexDataAsPoint2D();
    vertices[0].set(0.5*w,              1.15470053837925152902*borderWidth);
    vertices[1].set(w - borderWidth,    0.25*h + 0.57735026918962576451*borderWidth);
    vertices[2].set(w - borderWidth,    0.75*h - 0.57735026918962576451*borderWidth);
    vertices[3].set(0.5*w,              h - 1.15470053837925152902*borderWidth);
    vertices[4].set(borderWidth,        0.75*h - 0.57735026918962576451*borderWidth);
    vertices[5].set(borderWidth,        0.25*h + 0.57735026918962576451*borderWidth);

    //Hex border
    vertices = hexBorderGeo->vertexDataAsPoint2D();
    vertices[0].set(0.5*w,              0);
    vertices[1].set(0.5*w,              1.15470053837925152902*borderWidth);
    vertices[2].set(w,                  0.25*h);
    vertices[3].set(w - borderWidth,    0.25*h + 0.57735026918962576451*borderWidth);
    vertices[4].set(w,                  0.75*h);
    vertices[5].set(w - borderWidth,    0.75*h - 0.57735026918962576451*borderWidth);
    vertices[6].set(0.5*w,  h);
    vertices[7].set(0.5*w,              h - 1.15470053837925152902*borderWidth);
    vertices[8].set(0,      0.75*h);
    vertices[9].set(borderWidth,        0.75*h - 0.57735026918962576451*borderWidth);
    vertices[10].set(0,      0.25*h);
    vertices[11].set(borderWidth,       0.25*h + 0.57735026918962576451*borderWidth);
    vertices[12].set(0.5*w,             0);
    vertices[13].set(0.5*w,             1.15470053837925152902*borderWidth);

    hexFill->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
    hexBorder->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
    root->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
    return root;
}

QJsonObject HexTile::dumpToJSON() const {
    QJsonObject json;

    //Basic HexTile properties
    json["physicalWidth"] = coords->getPhysicalWidth();
    json["q"] = coords->getQ();
    json["r"] = coords->getR();
    if(stdSourceCoords){
        json["stdSourceCoordsI"] = stdSourceCoords->getI();
        json["stdSourceCoordsJ"] = stdSourceCoords->getJ();
        json["stdSourceCoordsU"] = stdSourceCoords->getU();
        json["stdSourceCoordsV"] = stdSourceCoords->getV();
    }
    else{
        json["sourceLeft"] = sourceLeft;
        json["sourceTop"] = sourceTop;
        json["sourceRight"] = sourceRight;
        json["sourceBottom"] = sourceBottom;
        json["sourceCenterX"] = sourceCenterX;
        json["sourceCenterY"] = sourceCenterY;
    }
    json["fillColor"] = fillColor.name();
    json["borderColor"] = borderColor.name();
    json["borderSize"] = borderSize;

    //Custom properties defined by the user
    const QMetaObject* thisMeta = metaObject();
    QSet<QString> customProperties;
    for (int i=0;i<thisMeta->propertyCount();i++){
        QMetaProperty p = thisMeta->property(i);
        if(p.isReadable() && p.isWritable() && !p.isConstant())
            customProperties << QString(p.name());
    }
    HexTile basicTile;
    const QMetaObject* basicMeta = basicTile.metaObject();
    for(int i=0;i<basicMeta->propertyCount();i++)
        customProperties.remove(QString(basicMeta->property(i).name()));
    for(QString p : customProperties){
        QVariant var = QQmlProperty::read(this, p);
        if(var.canConvert<QVariantList>())
            json[p] = QJsonValue::fromVariant(var.value<QVariantList>());
        else
            json[p] = QJsonValue::fromVariant(var);
    }

    return json;
}

void HexTile::loadFromJSON(QJsonObject const& json){

    //Basic HexTile properties
    coords->setPhysicalWidth(json["physicalWidth"].toDouble());
    coords->setQ(json["q"].toInt());
    coords->setR(json["r"].toInt());
    if(json.contains("stdSourceCoordsI") && json.contains("stdSourceCoordsJ") && json.contains("stdSourceCoordsU") && json.contains("stdSourceCoordsV")){
        HexTileStdSourceCoords* std = new HexTileStdSourceCoords();
        std->setI(json["stdSourceCoordsI"].toInt());
        std->setJ(json["stdSourceCoordsJ"].toInt());
        std->setU(json["stdSourceCoordsU"].toInt());
        std->setV(json["stdSourceCoordsV"].toInt());
        setStdSourceCoords(std);
    }
    else{
        sourceLeft      = json["sourceLeft"     ].toDouble();
        sourceTop       = json["sourceTop"      ].toDouble();
        sourceRight     = json["sourceRight"    ].toDouble();
        sourceBottom    = json["sourceBottom"   ].toDouble();
        sourceCenterX   = json["sourceCenterX"  ].toDouble();
        sourceCenterY   = json["sourceCenterY"  ].toDouble();
    }
    fillColor = QColor(json["fillColor"].toString());
    borderColor = QColor(json["borderColor"].toString());
    borderSize = json["borderSize"].toDouble();

    //Custom properties defined by the user
    const QMetaObject* thisMeta = metaObject();
    QSet<QString> customProperties;
    for (int i=0;i<thisMeta->propertyCount();i++){
        QMetaProperty p = thisMeta->property(i);
        if(p.isReadable() && p.isWritable() && !p.isConstant())
            customProperties << QString(p.name());
    }
    HexTile basicTile;
    const QMetaObject* basicMeta = basicTile.metaObject();
    for(int i=0;i<basicMeta->propertyCount();i++)
        customProperties.remove(QString(basicMeta->property(i).name()));
    for(QString p : customProperties){
        QJsonValue val = json[p];
        if(val != QJsonValue::Undefined)
            QQmlProperty::write(this, p, val.toVariant());
    }
}

void HexTile::mousePressEvent(QMouseEvent* event){
    QVector2D screenSize(width(), height());
    QVector2D screenCoords(event->localPos());
    QVector2D physicalSize(coords->getPhysicalWidth(), coords->getPhysicalHeight());
    QVector2D tileCoords = screenCoords/screenSize*physicalSize - 0.5f*physicalSize;

    //Need to check whether click is in the hexagon
    bool accepted =
        !CelluloMathUtil::pointAboveLine(tileCoords, physicalSize*QVector2D(-0.50f, -0.25f), physicalSize*QVector2D(0.00f, -0.50f)) &&
        !CelluloMathUtil::pointAboveLine(tileCoords, physicalSize*QVector2D( 0.00f, -0.50f), physicalSize*QVector2D(0.50f, -0.25f)) &&
         CelluloMathUtil::pointAboveLine(tileCoords, physicalSize*QVector2D(-0.50f,  0.25f), physicalSize*QVector2D(0.00f,  0.50f)) &&
         CelluloMathUtil::pointAboveLine(tileCoords, physicalSize*QVector2D( 0.00f,  0.50f), physicalSize*QVector2D(0.50f,  0.25f));
    if(accepted)
        emit clicked(tileCoords + coords->hexOffset());

    event->setAccepted(accepted);
    QQuickItem::mousePressEvent(event);
}

}

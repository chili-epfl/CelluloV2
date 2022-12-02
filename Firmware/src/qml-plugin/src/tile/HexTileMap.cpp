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
 * @file HexTileMap.cpp
 * @brief Source for a hexagonal multi-tile map
 * @author Ayberk Özgür
 * @date 2018-04-27
 */

#include "HexTileMap.h"

#include "HexTile.h"
#include "../util/math/CelluloMathUtil.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QQmlEngine>

#include <limits>

namespace Cellulo{

HexTileMap::HexTileMap(QQuickItem* parent) : PoseRemapper(parent),
    toScreenSize(this),
    toScreenCoords(this)
{
    autoResize = true;
    physicalTopLeft = QVector2D(-50.0f, -57.73502691896257645092f);
    physicalSize = QVector2D(100.0f, 115.47005383792515290183f);
    connect(this, SIGNAL(tileAdded(HexTile*)),      this, SLOT(fitPhysicalCoordsToTiles()));
    connect(this, SIGNAL(tileRemoved(int, int)),    this, SLOT(fitPhysicalCoordsToTiles()));

    connect(this, SIGNAL(widthChanged()),           this, SLOT(updateToScreenSize()));
    connect(this, SIGNAL(heightChanged()),          this, SLOT(updateToScreenSize()));
    connect(this, SIGNAL(physicalSizeChanged()),    this, SLOT(updateToScreenSize()));

    connect(this, SIGNAL(widthChanged()),           this, SLOT(updateToScreenCoords()));
    connect(this, SIGNAL(heightChanged()),          this, SLOT(updateToScreenCoords()));
    connect(this, SIGNAL(physicalSizeChanged()),    this, SLOT(updateToScreenCoords()));
    connect(this, SIGNAL(physicalTopLeftChanged()), this, SLOT(updateToScreenCoords()));

    connect(this, SIGNAL(toScreenSizeChanged()),    this, SIGNAL(markedDirty()));
    connect(this, SIGNAL(toScreenCoordsChanged()),  this, SIGNAL(markedDirty()));

    autoBuild = false;

    tileComponent = nullptr;
}

HexTileMap::~HexTileMap(){ }

void HexTileMap::setAutoResize(bool autoResize){
    this->autoResize = autoResize;
    if(autoResize){
        connect(this, SIGNAL(tileAdded(HexTile*)),      this, SLOT(fitPhysicalCoordsToTiles()));
        connect(this, SIGNAL(tileRemoved(int, int)),    this, SLOT(fitPhysicalCoordsToTiles()));
    }
    else{
        disconnect(this, SIGNAL(tileAdded(HexTile*)),   this, SLOT(fitPhysicalCoordsToTiles()));
        disconnect(this, SIGNAL(tileRemoved(int, int)), this, SLOT(fitPhysicalCoordsToTiles()));
    }
    emit autoResizeChanged();
}

void HexTileMap::fitPhysicalCoordsToTiles(){
    QVector2D newTopLeft(-50.0f, -57.73502691896257645092f);
    QVector2D newBottomRight(newTopLeft + QVector2D(100.0f, 115.47005383792515290183f));
    float left, right, top, bottom;

    for(auto const& tileVariant : tiles){
        HexTile* tile = tileVariant.value<HexTile*>();
        if(tile){
            tile->getCoords()->limits(left, right, top, bottom);
            if(top < newTopLeft.y())
                newTopLeft.setY(top);
            if(bottom > newBottomRight.y())
                newBottomRight.setY(bottom);
            if(left < newTopLeft.x())
                    newTopLeft.setX(left);
            if(right > newBottomRight.x())
                    newBottomRight.setX(right);
        }
        else
            qCritical() << "HexTileMap::remapPose(): tiles can only contain HexTile type!";
    }

    setPhysicalTopLeft(newTopLeft);
    setPhysicalSize(newBottomRight - newTopLeft);
}

void HexTileMap::setPhysicalSize(QVector2D const& physicalSize){
    if(physicalSize.x() > 0 && physicalSize.y() > 0){
        this->physicalSize = physicalSize;
        emit physicalSizeChanged();
    }
    else
        qCritical() << "HexTileMap::setPhysicalSize(): Physical size must have positive width and height!";
}

void HexTileMap::setPhysicalTopLeft(QVector2D const& physicalTopLeft){
    this->physicalTopLeft = physicalTopLeft;
    emit physicalTopLeftChanged();
}

void HexTileMap::setAutoBuild(bool autoBuild){
    if(this->autoBuild != autoBuild){
        this->autoBuild = autoBuild;
        if(autoBuild)
            resetAutoBuilders();
        emit autoBuildChanged();
    }
}

void HexTileMap::setTileComponent(QQmlComponent* tileComponent){
    if(this->tileComponent != tileComponent){
        if(this->tileComponent)
            this->tileComponent->deleteLater();
        this->tileComponent = tileComponent;
        if(tileComponent){
            QQmlEngine::setObjectOwnership(tileComponent, QQmlEngine::CppOwnership);
            tileComponent->setParent(this);
        }
        emit tileComponentChanged();
    }
}

void HexTileMap::processTileClick(QVector2D physicalPos){
    HexTile* tile = qobject_cast<HexTile*>(sender());
    if(tile)
        emit tileClicked(tile, physicalPos);
    else
        qCritical() << "HexTileMap::processTileClick(): Cannot be invoked by other than HexTile!";
}

void HexTileMap::itemChange(ItemChange change, const ItemChangeData& value){
    if(change == ItemChange::ItemChildAddedChange){
        HexTile* newTile = qobject_cast<HexTile*>(value.item);
        if(newTile)
            addTile(newTile);
    }

    QQuickItem::itemChange(change, value);
}

void HexTileMap::updateToScreenSize(){
    toScreenSize.setInputSize(physicalSize);
    toScreenSize.setOutputSize(QVector2D(width(), height()));
    emit toScreenSizeChanged();
}

void HexTileMap::updateToScreenCoords(){
    toScreenCoords.setInputSize(physicalSize);
    toScreenCoords.setInputOrigin(physicalTopLeft);
    toScreenCoords.setOutputSize(QVector2D(width(), height()));
    toScreenCoords.setOutputOrigin(QVector2D(0, 0));
    emit toScreenCoordsChanged();
}

QVector3D HexTileMap::remapPose(QVector3D const& pose, QObject* sender){
    QVector2D sourcePosition = pose.toVector2D();
    HexTile* tile = getTileBySourceCoords(sourcePosition);
    HexTileMapAutoBuilder* builder = autoBuilders.value(sender, nullptr);
    if(!builder){
        builder = new HexTileMapAutoBuilder(this);
        connect(sender, SIGNAL(kidnappedChanged()), builder, SLOT(reset()));
        autoBuilders.insert(sender, builder);
    }

    //Position is on a known tile
    if(tile){
        QVector2D resultPosition = tile->sourceCoordinates(sourcePosition) + tile->getCoords()->hexOffset();
        builder->processKnownTile(resultPosition, tile->getCoords());

        QVector3D resultPose = resultPosition.toVector3D();
        resultPose.setZ(pose.z());
        return resultPose;
    }

    //Unknown tile encountered
    else
        return builder->processUnknownTile(pose, autoBuild, tiles.isEmpty(), this);
}

HexTile* HexTileMap::getTile(int q, int r){
    //TODO: More efficient lookup by axial coords possible?
    for(auto const& tileVariant : tiles){
        HexTile* tile = tileVariant.value<HexTile*>();
        if(tile){
            if(tile->getCoords()->getQ() == q && tile->getCoords()->getR() == r)
                return tile;
        }
        else
            qCritical() << "HexTileMap::remapPose(): tiles can only contain HexTile type!";
    }
    return nullptr;
}

HexTile* HexTileMap::getTile(QVector2D const& position){
    HexTile* result = nullptr;
    float minDist = std::numeric_limits<float>::infinity();

    //TODO: More efficient lookup by hex coords (q,r) !!!
    for(auto const& tileVariant : tiles){
        HexTile* tile = tileVariant.value<HexTile*>();
        if(tile){
            float dist = (position - tile->getCoords()->hexOffset()).lengthSquared();
            if(dist < minDist){
                minDist = dist;
                result = tile;
            }
        }
        else
            qCritical() << "HexTileMap::getTile(): tiles can only contain HexTile type!";
    }
    return result;
}

HexTile* HexTileMap::getTileBySourceCoords(QVector2D const& position){
    //TODO: More efficient lookup by standard source coords!!!
    for(auto const& tileVariant : tiles){
        HexTile* tile = tileVariant.value<HexTile*>();
        if(tile){
            if(tile->sourceContains(position))
                return tile;
        }
        else
            qCritical() << "HexTileMap::remapPose(): tiles can only contain HexTile type!";
    }
    return nullptr;
}

void HexTileMap::addTile(HexTile* newTile){
    if(!tiles.contains(QVariant::fromValue(newTile))){
        removeTile(newTile->getCoords()->getQ(), newTile->getCoords()->getR());
        tiles.append(QVariant::fromValue(newTile));
        connect(newTile, SIGNAL(clicked(QVector2D)), this, SLOT(processTileClick(QVector2D)));
        emit tileAdded(newTile);
    }
    else
        qInfo() << "HexTileMap::addNewTile(): Not adding already existing identical tile.";
}

bool HexTileMap::removeTile(HexTile* oldTile){
    //TODO: More efficient lookup by axial coords possible?
    for(QVariantList::iterator tileVariantIt = tiles.begin(); tileVariantIt != tiles.end(); tileVariantIt++){
        HexTile* tile = (*tileVariantIt).value<HexTile*>();
        if(tile){
            if(tile == oldTile){
                tiles.erase(tileVariantIt);
                emit tileRemoved(tile->getCoords()->getQ(), tile->getCoords()->getR());
                tile->deleteLater();
                resetAutoBuilders(); //Too expensive to check whether this tile was involved with autobuild...
                return true;
            }
        }
        else
            qCritical() << "HexTileMap::removeTile(): tiles can only contain HexTile type!";
    }
    return false;
}

bool HexTileMap::removeTile(int q, int r){
    //TODO: More efficient lookup by axial coords possible?
    for(QVariantList::iterator tileVariantIt =  tiles.begin(); tileVariantIt != tiles.end(); tileVariantIt++){
        HexTile* tile = (*tileVariantIt).value<HexTile*>();
        if(tile){
            if(tile->getCoords()->getQ() == q && tile->getCoords()->getR() == r){
                tiles.erase(tileVariantIt);
                emit tileRemoved(q, r);
                tile->deleteLater();
                resetAutoBuilders(); //Too expensive to check whether this tile was involved with autobuild...
                return true;
            }
        }
        else
            qCritical() << "HexTileMap::removeTile(): tiles can only contain HexTile type!";
    }
    return false;
}

void HexTileMap::clearTiles(){
    for(QVariantList::iterator tileVariantIt = tiles.begin(); tileVariantIt != tiles.end(); tileVariantIt++){
        HexTile* tile = (*tileVariantIt).value<HexTile*>();
        if(tile){
            tiles.erase(tileVariantIt);
            emit tileRemoved(tile->getCoords()->getQ(), tile->getCoords()->getR());
            tile->deleteLater();
        }
        else
            qCritical() << "HexTileMap::removeTile(): tiles can only contain HexTile type!";
    }
    resetAutoBuilders();
}

void HexTileMap::resetAutoBuilders(){
    for(HexTileMapAutoBuilder* builder : autoBuilders)
        builder->reset();
}

void HexTileMap::dumpTilesToJSON(QString const& filename){
    QFile saveFile(QUrl(filename).toLocalFile());
    if(!saveFile.open(QIODevice::WriteOnly)){
        qCritical() << "HexTileMap::dumpTilesToJSON(): Couldn't write into file.";
        return;
    }

    QJsonArray jsonTiles;
    for(auto const& tileVariant : tiles){
        HexTile* tile = tileVariant.value<HexTile*>();
        if(tile)
            jsonTiles.append(tile->dumpToJSON());
        else
            qCritical() << "HexTileMap::dumpTilesToJSON(): tiles can only contain HexTile type!";
    }
    saveFile.write(QJsonDocument(jsonTiles).toJson());
}

void HexTileMap::loadTilesFromJSON(QString const& filename){
    QFile loadFile(QUrl(filename).toLocalFile());
    if(!loadFile.open(QIODevice::ReadOnly)){
        qCritical("HexTileMap::loadTilesFromJSON(): Couldn't open file.");
        return;
    }

    QJsonArray jsonTiles = QJsonDocument::fromJson(loadFile.readAll()).array();
    for(const QJsonValue& jsonTile : jsonTiles){
        HexTile* tile = nullptr;

        //Create tile from component if there
        if(tileComponent){
            tile = qobject_cast<HexTile*>(tileComponent->create());
            if(!tile)
                qCritical() << "HexTileMap::loadTilesFromJSON(): tileComponent must encapsulate a HexTile-derived object!";
            else
                QQmlEngine::setObjectOwnership(tile, QQmlEngine::JavaScriptOwnership);
        }
        if(!tile)
            tile = new HexTile();

        tile->loadFromJSON(jsonTile.toObject());
        tile->setParent(this);
        tile->setParentItem(this); //Child added detection will call addTile(), no need to call here
    }
}

}

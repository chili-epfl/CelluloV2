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
 * @file HexTileMapAutoBuilder.cpp
 * @brief Source for a hexagonal tile autobuilder
 * @author Ayberk Özgür
 * @date 2018-05-18
 */

#include "HexTileMapAutoBuilder.h"

#include "../util/math/CelluloMathUtil.h"

#include <QQmlEngine>

namespace Cellulo{

HexTileMapAutoBuilder::HexTileMapAutoBuilder(QObject* parent) : QObject(parent){
    knownCoordsExist = false;
    unknownStdCoords = nullptr;
}

HexTileMapAutoBuilder::~HexTileMapAutoBuilder(){}

void HexTileMapAutoBuilder::reset(){
    knownHistory.clear();
    unknownHistory.clear();
    knownCoordsExist = false;
    if(unknownStdCoords){
        delete unknownStdCoords;
        unknownStdCoords = nullptr;
    }
}

void HexTileMapAutoBuilder::processKnownTile(QVector2D const& position, AxialHexCoords* coords){
    if(knownCoordsExist){
        if(knownCoords.getQ() != coords->getQ() || knownCoords.getR() != coords->getR()){
            knownCoords.copyFrom(*coords);
            knownHistory.clear();
        }
    }
    else{
        knownCoordsExist = true;
        knownCoords.copyFrom(*coords);
    }
    knownHistory.append(position);
    if(knownHistory.size() > knownHistorySize)
        knownHistory.removeFirst();
}

QVector3D HexTileMapAutoBuilder::processUnknownTile(QVector3D const& sourcePose, bool addNewTile, bool mapEmpty, Cellulo::HexTileMap* tileMap){
    QVector2D sourcePosition = sourcePose.toVector2D();
    QVector3D result(0,0,0);

    //Get the standard tile coords of the unknown position
    HexTileStdSourceCoords* newUnknownStdCoords = new HexTileStdSourceCoords();
    if(newUnknownStdCoords->estimateFromCoords(sourcePosition)){

        //Update old std coords if necessary and position history
        if(unknownStdCoords){
            if(unknownStdCoords->equals(*newUnknownStdCoords)){
                delete newUnknownStdCoords;
                unknownHistory.append(sourcePosition);
                if(unknownHistory.size() > unknownHistorySize)
                    unknownHistory.removeFirst();
            }
            else{
                delete unknownStdCoords;
                unknownStdCoords = newUnknownStdCoords;
                unknownHistory.clear();
                unknownHistory.append(sourcePosition);
            }
        }
        else{
            unknownStdCoords = newUnknownStdCoords;
            unknownHistory.clear();
            unknownHistory.append(sourcePosition);
        }

        //Build imaginary tile, create tile from component if there
        HexTile* imaginaryTile = nullptr;
        QQmlComponent* tileComponent = tileMap->getTileComponent();
        if(tileComponent){
            imaginaryTile = qobject_cast<HexTile*>(tileComponent->create());
            if(!imaginaryTile)
                qCritical() << "HexTileMapAutoBuilder::processUnknownTile(): tileComponent must encapsulate a HexTile-derived object!";
            else
                QQmlEngine::setObjectOwnership(imaginaryTile, QQmlEngine::JavaScriptOwnership);
        }
        if(!imaginaryTile)
            imaginaryTile = new HexTile(); //No need for screen rendering now, no need for parent
        imaginaryTile->setStdSourceCoords(unknownStdCoords);

        //This is the first tile ever, assume q,r = 0,0
        if(!knownCoordsExist){

            //If map is empty, we can try to get coordinates
            if(mapEmpty){
                result = (imaginaryTile->sourceCoordinates(sourcePosition) + imaginaryTile->getCoords()->hexOffset()).toVector3D();
                result.setZ(sourcePose.z());
            }

            //Add this tile
            if(mapEmpty && addNewTile && unknownHistory.size() >= unknownHistorySize){
                unknownStdCoords = nullptr; //Detach std coords from the map, rests only with imaginaryTile
                imaginaryTile->setParent(tileMap);
                imaginaryTile->setParentItem(tileMap); //Child added detection will call addTile(), no need to call here















                reset();
            }
            else
                delete imaginaryTile;
        }

        //There are existing tiles
        else{
            QVector<QVector2D> corners;
            knownCoords.cornerList(corners);
            int deltaQ = 0;
            int deltaR = 0;
            static QVector<int> deltaQs = {0,   -1, -1, 0,  1,  1};
            static QVector<int> deltaRs = {-1,  0,  1,  1,  0,  -1};

            if(knownHistory.length() >= 2){

                //Detect raycast exit to estimate unknown coordinates for a short while
                //Order: top-left, left, bottom-left, bottom-right, right, top-right
                QVector2D origin(0,0);
                for(QVector2D const& vec : knownHistory)
                    origin += vec;
                origin /= knownHistory.length();
                QVector2D mainVec = knownHistory.last() - knownHistory.first();
                for(int i=0;i<6;i++)
                    if(CelluloMathUtil::rayCrossesLineSeg(origin, mainVec, corners[i], corners[(i + 1) % 6])){
                        deltaQ = deltaQs[i];
                        deltaR = deltaRs[i];
                        break;
                    }
                imaginaryTile->getCoords()->setQ(knownCoords.getQ() + deltaQ);
                imaginaryTile->getCoords()->setR(knownCoords.getR() + deltaR);
                result = (imaginaryTile->sourceCoordinates(sourcePosition) + imaginaryTile->getCoords()->hexOffset()).toVector3D();
                result.setZ(sourcePose.z());
            }

            //Enough samples on the known and unknown tiles, pick best direction
            if(addNewTile && unknownHistory.size() >= unknownHistorySize && knownHistory.size() >= knownHistoryMinSize){

                //Directions on the known tile
                QList<QVector2D> knownVecs;
                for(auto it = knownHistory.begin() + 1; it != knownHistory.end(); it++){
                    QVector2D vec = *it - *(it - 1);
                    if(vec.length() > minVecSize)
                        knownVecs.append(vec);
                }

                //Directions on the unknown tile calculated from the raw source coordinates, will result in the same if calculated from remapped coordinates
                QList<QVector2D> unknownVecs;
                for(auto it = unknownHistory.begin() + 1; it != unknownHistory.end(); it++){
                    QVector2D vec = *it - *(it - 1);
                    if(vec.length() > minVecSize)
                        unknownVecs.append(vec);
                }

                float bestScore = std::numeric_limits<float>::infinity();
                deltaQ = 0;
                deltaR = 0;
                for(int i=0;i<6;i++){
                    float hypotDeltaQ = deltaQs[i];
                    float hypotDeltaR = deltaRs[i];
                    imaginaryTile->getCoords()->setQ(knownCoords.getQ() + hypotDeltaQ);
                    imaginaryTile->getCoords()->setR(knownCoords.getR() + hypotDeltaR);

                    //Passage direction from the known tile to the unknown tile
                    QVector2D fromKnownToUnknownDir =
                        (imaginaryTile->sourceCoordinates(unknownHistory.first()) + imaginaryTile->getCoords()->hexOffset()) - knownHistory.last();

                    //Score is the sum of angles between all direction pairs, minimum score wins
                    //We don't add known-known, known-unknown and unknown-unknown pairs to the score as they are the same in all 6 cases
                    float score = 0;
                    for(auto it = knownVecs.begin(); it != knownVecs.end(); it++)
                        score += CelluloMathUtil::angleBetween(*it, fromKnownToUnknownDir);
                    for(auto it = unknownVecs.begin(); it != unknownVecs.end(); it++)
                        score += CelluloMathUtil::angleBetween(*it, fromKnownToUnknownDir);
                    if(score < bestScore){
                        bestScore = score;
                        deltaQ = hypotDeltaQ;
                        deltaR = hypotDeltaR;
                    }
                }

                //Add tile
                if(deltaQ != 0 || deltaR != 0){
                    imaginaryTile->getCoords()->setQ(knownCoords.getQ() + deltaQ);
                    imaginaryTile->getCoords()->setR(knownCoords.getR() + deltaR);
                    result = (imaginaryTile->sourceCoordinates(sourcePosition) + imaginaryTile->getCoords()->hexOffset()).toVector3D();
                    result.setZ(sourcePose.z());
                    unknownStdCoords = nullptr; //Detach std coords from the map, rests only with imaginaryTile
                    imaginaryTile->setParent(tileMap);
                    imaginaryTile->setParentItem(tileMap); //Child added detection will call addTile(), no need to call here

                    //Transfer unknown history to known history
                    knownHistory.clear();
                    for(QVector2D const& pos : unknownHistory)
                        knownHistory.append(imaginaryTile->sourceCoordinates(pos) + imaginaryTile->getCoords()->hexOffset());
                    unknownHistory.clear();
                    knownCoords.copyFrom(*imaginaryTile->getCoords());
                }
                else{
                    delete imaginaryTile;
                    reset();
                }
            }
            else
                delete imaginaryTile;
        }
    }

    //Estimate somehow failed
    else{
        delete newUnknownStdCoords;
        qCritical() << "HexTileMapAutoBuilder::processUnknownTile(): Standard coordinate estimate somehow failed.";
    }

    return result;
}

}

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
 * @file CelluloZonePaintedItem.cpp
 * @brief Source for a QML compatible QQuickPaintedItem for Cellulo zones
 * @author Ayberk Özgür
 * @date 2016-06-28
 */

#include "CelluloZonePaintedItem.h"

#include <QVariant>

namespace Cellulo{

CelluloZonePaintedItem::CelluloZonePaintedItem(QQuickItem* parent) : QQuickPaintedItem(parent){
    associatedZone = NULL;
    color = QColor(128,128,128,128);

    if(parent)
        updateParentWidthHeightBinding(parent);
    else
        qWarning() << "CelluloZonePaintedItem::CelluloZonePaintedItem(): CelluloZonePaintedItem must have a visual parent to function properly.";
}

void CelluloZonePaintedItem::itemChange(ItemChange change, const ItemChangeData& value){
    if(change == ItemChange::ItemParentHasChanged){
        QQuickItem* newParent = qobject_cast<QQuickItem*>(value.item);
        if(newParent){
            updateParentWidthHeightBinding(newParent);
            qDebug() << "CelluloZonePaintedItem::itemChange(): Parent changed, binding width/height automatically.";
        }
    }

    QQuickItem::itemChange(change, value);
}

void CelluloZonePaintedItem::setWidthToParent(){
    setWidth(parentItem()->width());
}

void CelluloZonePaintedItem::setHeightToParent(){
    setHeight(parentItem()->height());
}

void CelluloZonePaintedItem::updateParentWidthHeightBinding(QQuickItem* newParent){
    disconnect(parentWidthConnection);
    disconnect(parentHeightConnection);
    parentWidthConnection = connect(newParent, SIGNAL(widthChanged()), this, SLOT(setWidthToParent()));
    parentHeightConnection = connect(newParent, SIGNAL(heightChanged()), this, SLOT(setHeightToParent()));
    setWidth(newParent->width());
    setHeight(newParent->height());
}

void CelluloZonePaintedItem::setColor(QColor newColor){
    if(newColor != color){
        color = newColor;
        emit colorChanged();
        update();
    }
}

void CelluloZonePaintedItem::setAssociatedZone(CelluloZone* zone){
    if(associatedZone != zone){
        associatedZone = zone;
        associatedZone->setPaintedItem(this);
        emit associatedZoneChanged();
        update();
    }
}

void CelluloZonePaintedItem::setPhysicalPlaygroundWidth(qreal newWidth){
    if(newWidth != physicalPlaygroundWidth){
        physicalPlaygroundWidth = newWidth;
        emit physicalPlaygroundWidthChanged();
        update();
    }
}

void CelluloZonePaintedItem::setPhysicalPlaygroundHeight(qreal newHeight){
    if(newHeight != physicalPlaygroundHeight){
        physicalPlaygroundHeight = newHeight;
        emit physicalPlaygroundHeightChanged();
        update();
    }
}

void CelluloZonePaintedItem::paint(QPainter* painter){
    if(associatedZone){
        associatedZone->paint(painter, color, width(), height(), physicalPlaygroundWidth, physicalPlaygroundHeight);
    }
    else
        qWarning() << "CelluloZonePaintedItem::paint(): No associatedZone set.";
}

}

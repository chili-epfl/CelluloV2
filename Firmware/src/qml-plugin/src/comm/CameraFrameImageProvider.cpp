/*
 * Copyright (C) 2015 EPFL
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
 * @file CameraFrameImageProvider.cpp
 * @brief Source for converting the camera image in QByteBuffer to QImage
 * @author Ayberk Özgür
 * @date 2015-05-21
 */

#include"CameraFrameImageProvider.h"

#include<QImage>

namespace Cellulo{

CameraFrameImageProvider::CameraFrameImageProvider():
    QQuickImageProvider(QQuickImageProvider::Image){ }

QImage CameraFrameImageProvider::requestImage(QString const& id, QSize* size, QSize const& requestedSize){
    Q_UNUSED(id)
    Q_UNUSED(requestedSize)

    if(size != NULL)
        *size = QSize(IMG_WIDTH_SHARED, IMG_HEIGHT_SHARED);

    int* frameCharBufferPtr = (int*)frameCharBuffer;
    unsigned char pixel;
    for(int i=0;i<IMG_WIDTH_SHARED*IMG_HEIGHT_SHARED;i++){
        pixel = CelluloBluetooth::frameBuffer[i];
        frameCharBufferPtr[i] = (255 << 24) + (pixel << 16) + (pixel << 8) + pixel;
    }

    return QImage(frameCharBuffer, IMG_WIDTH_SHARED, IMG_HEIGHT_SHARED, QImage::Format_ARGB32);
}

}

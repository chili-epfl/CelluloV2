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
 * @file CameraFrameImageProvider.h
 * @brief Header for converting the camera image in QByteBuffer to QImage
 * @author Ayberk Özgür
 * @date 2015-05-21
 */

#ifndef CAMERAFRAMEIMAGEPROVIDER_H
#define CAMERAFRAMEIMAGEPROVIDER_H

#include"CelluloBluetoothSharedDefs.h"
#include"CelluloBluetooth.h"

#include<QQuickImageProvider>

namespace Cellulo{

/** @cond DO_NOT_DOCUMENT */

/**
 * @brief QImage provider from QByteArray
 */
class CameraFrameImageProvider : public QQuickImageProvider {

public:

    CameraFrameImageProvider();

    QImage requestImage(QString const& id, QSize* size, QSize const& requestedSize);

private:

    unsigned char frameCharBuffer[IMG_WIDTH_SHARED*IMG_HEIGHT_SHARED*4];

};

/** @endcond */

}

#endif // CAMERAFRAMEIMAGEPROVIDER_H

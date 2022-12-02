/*
 * Copyright (C) 2018 EPFL
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
 * @file PolyBezierTrackerConstVel.h
 * @brief Makes CelluloRobots track PolyBezier curves with constant velocity
 * @author Ayberk Özgür
 * @date 2018-07-13
 */

#ifndef POLYBEZIERTRACKERCONSTVEL_H
#define POLYBEZIERTRACKERCONSTVEL_H

#include "PolyBezierTracker.h"

namespace Cellulo {

/**
 * @addtogroup robot
 * @{
 */

/**
 * @brief Makes a CelluloRobot track a PolyBezier curve with constant velocity
 */
class PolyBezierTrackerConstVel : public PolyBezierTracker {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @brief Linear tracking velocity, must be between 0 mm/s and 185 mm/s, default is 50 mm/s */
    Q_PROPERTY(qreal trackingVelocity READ getTrackingVelocity WRITE setTrackingVelocity NOTIFY trackingVelocityChanged)

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new PolyBezier tracker with constant velocity
     *
     * @param parent The QML parent
     */
    PolyBezierTrackerConstVel(QQuickItem* parent = nullptr);

    /**
     * @brief Destroys the tracker
     */
    virtual ~PolyBezierTrackerConstVel();

    /**
     * @brief Gets the linear tracking velocity
     *
     * @return Tracking velocity in mm/s
     */
    qreal getTrackingVelocity() const { return trackingVelocity; }

    /**
     * @brief Set the linear tracking velocity
     *
     * @param trackingVelocity Linear tracking velocity
     */
    void setTrackingVelocity(qreal trackingVelocity);

    /** @endcond */

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when the tracking velocity changes
     */
    void trackingVelocityChanged();

    /** @endcond */

private:

    /**
     * @brief Sets the robot velocities so that the curve is tracked with constant linear velocity
     *
     * @param deltaTime Unused
     */
    virtual void spinLoop(qreal deltaTime) override;

    qreal trackingVelocity = 50.0; ///< Linear tracking velocity

};

/** @} */

}

#endif // POLYBEZIERTRACKERCONSTVEL_H

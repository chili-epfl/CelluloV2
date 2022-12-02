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
 * @file PolyBezierTrackerConstAccel.h
 * @brief Makes CelluloRobots track PolyBezier curves with constant velocity and finite constant acceleration
 * @author Ayberk Özgür
 * @date 2018-07-13
 */

#ifndef POLYBEZIERTRACKERCONSTACCEL_H
#define POLYBEZIERTRACKERCONSTACCEL_H

#include "PolyBezierTracker.h"

namespace Cellulo {

/**
 * @addtogroup robot
 * @{
 */

/**
 * @brief Makes a CelluloRobot track a PolyBezier curve with constant velocity and finite constant acceleration
 */
class PolyBezierTrackerConstAccel : public PolyBezierTracker {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @brief Linear tracking velocity to reach, must be between 0 mm/s and 185 mm/s, default is 75 mm/s */
    Q_PROPERTY(qreal trackingVelocity READ getTrackingVelocity WRITE setTrackingVelocity NOTIFY trackingVelocityChanged)

    /** @brief Linear acceleration, must be above 0 mm/s^2, default is 100 mm/s */
    Q_PROPERTY(qreal trackingAcceleration READ getTrackingAcceleration WRITE setTrackingAcceleration NOTIFY trackingAccelerationChanged)

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new PolyBezier tracker with constant velocity
     *
     * @param parent The QML parent
     */
    PolyBezierTrackerConstAccel(QQuickItem* parent = nullptr);

    /**
     * @brief Destroys the tracker
     */
    virtual ~PolyBezierTrackerConstAccel();

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

    /**
     * @brief Gets the linear tracking acceleration
     *
     * @return Tracking acceleration in mm/s^2
     */
    qreal getTrackingAcceleration() const { return trackingAcceleration; }

    /**
     * @brief Set the linear tracking acceleration
     *
     * @param trackingAcceleration Linear tracking acceleration
     */
    void setTrackingAcceleration(qreal trackingAcceleration);

    /** @endcond */

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when the tracking velocity changes
     */
    void trackingVelocityChanged();

    /**
     * @brief Emitted when the tracking acceleration changes
     */
    void trackingAccelerationChanged();

    /** @endcond */

private:

    /**
     * @brief Sets the robot velocities so that the curve is tracked with constant linear velocity
     *
     * @param deltaTime Unused
     */
    virtual void spinLoop(qreal deltaTime) override;

    qreal currentVelocity = 0.0;        ///< Actual linear tracking velocity
    qreal trackingVelocity = 75.0;      ///< Linear tracking velocity goal
    qreal trackingAcceleration = 100.0; ///< Linear tracking acceleration

};

/** @} */

}

#endif // POLYBEZIERTRACKERCONSTACCEL_H

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
 * @file PolyBezierTrackerAdaptiveVel.h
 * @brief Makes CelluloRobots track PolyBezier curves with adaptive velocity, i.e decreases velocity when turning sharp corners
 * @author Ayberk Özgür
 * @date 2018-07-13
 */

#ifndef POLYBEZIERTRACKERADAPTIVEVEL_H
#define POLYBEZIERTRACKERADAPTIVEVEL_H

#include "PolyBezierTracker.h"

namespace Cellulo {

/**
 * @addtogroup robot
 * @{
 */

/**
 * @brief Makes a CelluloRobot track a PolyBezier curve with adaptive velocity, i.e decreases velocity when turning sharp corners
 */
class PolyBezierTrackerAdaptiveVel : public PolyBezierTracker {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @brief Maximum linear tracking velocity, adopted on straight parts of the path, must be between 0 mm/s and 185 mm/s, default is 100 mm/s */
    Q_PROPERTY(qreal maxTrackingVelocity READ getMaxTrackingVelocity WRITE setMaxTrackingVelocity NOTIFY maxTrackingVelocityChanged)

    /** @brief Minimum linear tracking velocity, adopted on sharp turning parts of the path, must be between 0 mm/s and 185 mm/s, default is 10 mm/s */
    Q_PROPERTY(qreal minTrackingVelocity READ getMinTrackingVelocity WRITE setMinTrackingVelocity NOTIFY minTrackingVelocityChanged)

    /** @brief Current linear tracking velocity, close to max. velocity if path is straight, or to min. velocity if the path is sharp, read-only */
    Q_PROPERTY(qreal currentTrackingVelocity READ getCurrentTrackingVelocity NOTIFY currentTrackingVelocityChanged)

    /** @brief Value to determine how much the velocity is decreased towards minTrackingVelocity when turning sharp corners, default 10.0 */
    Q_PROPERTY(qreal attenuationPower MEMBER attenuationPower)

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new PolyBezier tracker with adaptive velocity
     *
     * @param parent The QML parent
     */
    PolyBezierTrackerAdaptiveVel(QQuickItem* parent = nullptr);

    /**
     * @brief Destroys the tracker
     */
    virtual ~PolyBezierTrackerAdaptiveVel();

    /**
     * @brief Gets the maximum linear tracking velocity
     *
     * @return Maximum tracking velocity in mm/s
     */
    qreal getMaxTrackingVelocity() const { return maxTrackingVelocity; }

    /**
     * @brief Gets the minimum linear tracking velocity
     *
     * @return Minimum tracking velocity in mm/s
     */
    qreal getMinTrackingVelocity() const { return minTrackingVelocity; }

    /**
     * @brief Set the maximum linear tracking velocity
     *
     * @param maxTrackingVelocity Maximum linear tracking velocity
     */
    void setMaxTrackingVelocity(qreal maxTrackingVelocity);

    /**
     * @brief Set the minimum linear tracking velocity
     *
     * @param maxTrackingVelocity Minimum linear tracking velocity
     */
    void setMinTrackingVelocity(qreal minTrackingVelocity);

    /**
     * @brief Gets the current linear tracking velocity
     *
     * @return Current tracking velocity in mm/s
     */
    qreal getCurrentTrackingVelocity() const { return currentTrackingVelocity; }

    /** @endcond */

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when the maximum tracking velocity changes
     */
    void maxTrackingVelocityChanged();

    /**
     * @brief Emitted when the minimum tracking velocity changes
     */
    void minTrackingVelocityChanged();

    /**
     * @brief Emitted when the current tracking velocity changes
     */
    void currentTrackingVelocityChanged();

    /** @endcond */

private:

    /**
     * @brief Sets the robot velocities so that the curve is tracked with adaptive linear velocity
     *
     * @param deltaTime Unused
     */
    virtual void spinLoop(qreal deltaTime) override;

    qreal maxTrackingVelocity = 100.0;  ///< Maximum linear tracking velocity
    qreal minTrackingVelocity = 10.0;   ///< Maximum linear tracking velocity
    qreal currentTrackingVelocity;      ///< Current linear tracking velocity
    qreal attenuationPower = 10.0;      ///< Attenuation factor

};

/** @} */

}

#endif // POLYBEZIERTRACKERADAPTIVEVEL_H

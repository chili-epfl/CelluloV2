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
 * @file PolyBezierTrackerProfiledVel.h
 * @brief Makes CelluloRobots track PolyBezier curves with the desired velocity profile
 * @author Ayberk Özgür
 * @date 2018-07-13
 */

#ifndef POLYBEZIERTRACKERPROFILEDVEL_H
#define POLYBEZIERTRACKERPROFILEDVEL_H

#include "PolyBezierTracker.h"

namespace Cellulo {

/**
 * @addtogroup robot
 * @{
 */

/**
 * @brief Makes a CelluloRobot track a PolyBezier curve with the desired velocity profile
 */
class PolyBezierTrackerProfiledVel : public PolyBezierTracker {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @brief Linear tracking velocity profile; must be a list of velocities, each between 0 mm/s and 185 mm/s, that corresponds to the list of velocities to adpont at equidistant points on the curve; default is [0, 100, 100, 100, 100, 100, 100, 0] mm/s */
    Q_PROPERTY(QVariantList trackingVelocityProfile READ getTrackingVelocityProfile WRITE setTrackingVelocityProfile NOTIFY trackingVelocityProfileChanged)

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates a new PolyBezier tracker with profiled velocity
     *
     * @param parent The QML parent
     */
    PolyBezierTrackerProfiledVel(QQuickItem* parent = nullptr);

    /**
     * @brief Destroys the tracker
     */
    virtual ~PolyBezierTrackerProfiledVel();

    /**
     * @brief Gets the linear tracking velocity
     *
     * @return Tracking velocity in mm/s
     */
    QVariantList getTrackingVelocityProfile() const;

    /**
     * @brief Set the linear tracking velocity profile
     *
     * @param trackingVelocityProfile Linear tracking velocity profile
     */
    void setTrackingVelocityProfile(QVariantList const& trackingVelocityProfile);

    /** @endcond */

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when the tracking velocity changes
     */
    void trackingVelocityProfileChanged();

    /** @endcond */

private:

    /**
     * @brief Sets the robot velocities so that the curve is tracked with constant linear velocity
     *
     * @param deltaTime Unused
     */
    virtual void spinLoop(qreal deltaTime) override;

    /**
     * @brief Get tracking velocity from the profile that corresponds to the given arc length rate
     *
     * @param  r Arc length rate
     * @return   Velocity to be adopted at that point
     */
    qreal getTrackingVelocity(qreal r);

    QVector<qreal> trackingVelocityProfile = {10, 100, 100, 100, 100, 100, 100, 10}; ///< Linear tracking velocity profile

};

/** @} */

}

#endif // POLYBEZIERTRACKERPROFILEDVEL_H

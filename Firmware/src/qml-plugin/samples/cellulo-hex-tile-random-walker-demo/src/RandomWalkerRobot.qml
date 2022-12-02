import QtQuick 2.0

import Cellulo 1.0

CelluloRobot{
    property bool walk: false

    property int goalQ: 0
    property int goalR: 0
    property vector2d goalPos: Qt.vector2d(0,0)

    property HexTileMap hexMap: null

    poseRemapper: hexMap

    poseVelControlEnabled: true
    poseVelControlPeriod: 100
    poseVelControlKGoalPoseErr: Qt.vector3d(2,2,2)
    poseVelControlKGoalVel:     Qt.vector3d(0,0,0)
    poseVelControlKGoalVelErr:  Qt.vector3d(0,0,0)
    poseVelControlMaxLinearVel: 100.0
    poseVelControlMaxAngularVel: 7.5

    property var deltaQs: [0,   -1, -1, 0,  1,  1];
    property var deltaRs: [-1,  0,  1,  1,  0,  -1];

    onNextGoalPoseVelRequested: {
        if(walk){
            setGoalPoseAndVelocity(goalPos.x, goalPos.y, 30, 0, 0, 0, true, true, true);
            if(goalPos.minus(Qt.vector2d(x,y)).length() < 10){
                var newGoalQ;
                var newGoalR;
                var newTile;
                var ind = CelluloMathUtil.shuffle([0, 1, 2, 3, 4, 5]);
                for(var i=0;i<6;i++){
                    newGoalQ = goalQ + deltaQs[ind[i]];
                    newGoalR = goalR + deltaRs[ind[i]];
                    newTile = hexMap.getTile(newGoalQ, newGoalR);
                    if(newTile){
                        goalQ = newGoalQ;
                        goalR = newGoalR;
                        goalPos = newTile.coords.hexOffset();
                        return;
                    }
                }
            }
        }
        else
            setGoalPoseAndVelocity(0, 0, 0, 0, 0, 0, false, false, false);
    }
}

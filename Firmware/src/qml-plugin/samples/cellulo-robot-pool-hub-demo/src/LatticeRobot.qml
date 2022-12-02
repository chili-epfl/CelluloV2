import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.3
import QtBluetooth 5.2

import Cellulo 1.0

CelluloRobot{
    id: robot

    property var root: null

    //Internal properties
    property int index: 0

    /*
     * Private
     */

    poseVelControlPeriod: 100

    function init(){
        setCasualBackdriveAssistEnabled(false);
        clearHapticFeedback();
        clearTracking();
        setExposureTime(0);
        setGestureEnabled(false);
        setLocomotionInteractivityMode(CelluloBluetoothEnums.LocomotionInteractivityModeNormal);
        setVisualEffect(CelluloBluetoothEnums.VisualEffectConstAll,"#202020",0);
        setLEDResponseMode(CelluloBluetoothEnums.LEDResponseModeResponsiveIndividual);
    }

    onBootCompleted: init()

    onConnectionStatusChanged: {
        if(connectionStatus === CelluloBluetoothEnums.ConnectionStatusConnected)
            init();
    }

    onKeysTouchedChanged: root.recalcTouchedRobots()

    function anyKey(){
        for(var i=0;i<6;i++)
            if(keysTouched[i])
                return true;
        return false;
    }

    property vector3d goalXYTheta: Qt.vector3d(0,0,0)

    function calcLatticeGoal(){
        var myX = index % root.nearestSquareEdge;
        var myY = (index - myX)/root.nearestSquareEdge;
        var myVec = Qt.vector2d(myX*root.latticeDist, myY*root.latticeDist);

        var rotRad = root.latticePose.z*Math.PI/180;
        var rotMat = [
                    [Math.cos(rotRad), -Math.sin(rotRad)],
                    [Math.sin(rotRad), Math.cos(rotRad)]
                ];

        myVec = Qt.vector2d(rotMat[0][0]*myVec.x + rotMat[0][1]*myVec.y, rotMat[1][0]*myVec.x + rotMat[1][1]*myVec.y);

        goalXYTheta = Qt.vector3d(root.latticePose.x + myVec.x, root.latticePose.y + myVec.y, root.latticePose.z);
    }

    property bool reached: false

    function calcUserInput(){
        if(root.touchedRobot1 === robot){

            //Resize and rotate lattice according to the first and second held robot
            if(root.touchedRobot2 !== null)
                root.recalcResize();

            //Rotate according to the first held robot
            else
                root.latticePose.z = theta;

            //Move lattice according to the first held robot
            var myX = index % root.nearestSquareEdge;
            var myY = (index - myX)/root.nearestSquareEdge;

            var latticeCornerPos = Qt.vector2d(-myX*root.latticeDist, -myY*root.latticeDist);
            var rotRad = root.latticePose.z*Math.PI/180;
            var rotMat = [
                        [Math.cos(rotRad), -Math.sin(rotRad)],
                        [Math.sin(rotRad), Math.cos(rotRad)]
                    ];
            latticeCornerPos = Qt.vector2d(
                        rotMat[0][0]*latticeCornerPos.x + rotMat[0][1]*latticeCornerPos.y,
                        rotMat[1][0]*latticeCornerPos.x + rotMat[1][1]*latticeCornerPos.y
                        );

            root.latticePose.x = x + latticeCornerPos.x;
            root.latticePose.y = y + latticeCornerPos.y;
        }
    }

    function calcReached(){
        var xythetaDiff = goalXYTheta.minus(Qt.vector3d(x,y,theta));
        while(xythetaDiff.z > 180)
            xythetaDiff.z -= 360;
        while(xythetaDiff.z <= -180)
            xythetaDiff.z += 360;
        reached = (Qt.vector2d(xythetaDiff.x, xythetaDiff.y).length() < 5 && Math.abs(xythetaDiff.z) < 5);
    }

    onPoseChanged: {
        calcUserInput();
        calcLatticeGoal();
        calcReached();

        if(root.touchedRobot1 === robot || root.touchedRobot2 === robot){
            setGoalVelocity(0,0,0);
        }
        else if(root.goChecked){
            while(goalXYTheta.z >= 360)
                goalXYTheta.z -= 360;
            while(goalXYTheta.z < 0)
                goalXYTheta.z += 360;
            setGoalPose(goalXYTheta.x, goalXYTheta.y, goalXYTheta.z, 200, 10);
        }
        else
            setGoalVelocity(0,0,0);
    }
}

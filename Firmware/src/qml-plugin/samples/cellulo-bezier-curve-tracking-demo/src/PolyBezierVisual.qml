import QtQuick.Shapes 1.0

Shape{
    property real scaleCoeff: 1.0
    property var controlPoints: []

    ShapePath{
        id: shapePath
        strokeWidth: 2
        strokeColor: "black"
        fillColor: "transparent"

        function clear(){
            pathElements = [];
            visible = false;
        }

        function start(startPoint){
            startX = Qt.binding(function(){ return startPoint.x*scaleCoeff; });
            startY = Qt.binding(function(){ return startPoint.y*scaleCoeff; });
            visible = true;
        }

        function addCubic(cp1, cp2, end){
            var newCubic = Qt.createQmlObject('
                import QtQuick 2.0; \
                import QtQuick.Shapes 1.0; \
                \
                PathCubic{ \
                    control1X: ' + cp1.x + '*scaleCoeff; \
                    control1Y: ' + cp1.y + '*scaleCoeff; \
                    control2X: ' + cp2.x + '*scaleCoeff; \
                    control2Y: ' + cp2.y + '*scaleCoeff; \
                    x: ' + end.x + '*scaleCoeff; \
                    y: ' + end.y + '*scaleCoeff; \
                }',
                shapePath);
            var pathElements_ = []; //var pathElements_ = pathElements; doesn't work, not a plain Javascript array
            for(var i=0;i<pathElements.length;i++)
                pathElements_.push(pathElements[i]);
            pathElements_.push(newCubic);
            pathElements = pathElements_;
        }
    }

    onControlPointsChanged: {
        shapePath.clear();
        if(controlPoints.length > 0)
            shapePath.start(controlPoints[0]);
        for(var i=1;i<controlPoints.length;i+=3)
            shapePath.addCubic(controlPoints[i], controlPoints[i + 1], controlPoints[i + 2]);
    }
}

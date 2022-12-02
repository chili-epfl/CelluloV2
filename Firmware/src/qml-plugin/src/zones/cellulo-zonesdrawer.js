
var parent = null

function setVisualParent(newparent){
    parent = newparent
}

function drawNewCircle(color, rCircle, xCircle, yCircle, marginThickeness, stackingOrder, childNumber, playgroundWidth, playgroundHeight){
    xCircle = xCircle*playgroundWidth
    yCircle = yCircle*playgroundHeight
    rCircle = Math.sqrt(rCircle*playgroundWidth*playgroundHeight)
    marginThickeness = marginThickeness*playgroundWidth
    Qt.createQmlObject(
                'import QtQuick 2.2; \
                    Rectangle{id: child' + childNumber + '; \
                        z: + '+stackingOrder+'; \
                        x:'+ xCircle +'- ('+ rCircle +' + ' + marginThickeness/2 + '); \
                        y: '+ yCircle +'- ('+ rCircle +' + ' + marginThickeness/2 + '); \
                        height: ('+ rCircle +' + ' + marginThickeness/2 + ') * 2; \
                        width: ('+ rCircle +' + ' + marginThickeness/2 + ') * 2; \
                        color: ('+marginThickeness +' === 0 && "'+ color +'" != "black")? "'+ color +'" : "#00000000"; \
                        border.color:  Qt.darker("'+ color +'", 1.3); \
                        border.width: ('+marginThickeness +' === 0)? 1: '+ marginThickeness +'; \
                        radius: ('+ rCircle +' + ' + marginThickeness/2 + ');\
                    }',
                parent,'child' + childNumber)
}


function drawNewRectangle(color, xRectangle, yRectangle, heightRectangle, widthRectangle, marginThickeness, stackingOrder, childNumber, playgroundWidth, playgroundHeight){
    xRectangle= xRectangle*playgroundWidth
    yRectangle= yRectangle*playgroundHeight
    widthRectangle= widthRectangle*playgroundWidth
    heightRectangle= heightRectangle*playgroundHeight
    marginThickeness = marginThickeness*playgroundWidth

    Qt.createQmlObject(
                'import QtQuick 2.2; \
                    Rectangle{id: child' + childNumber + '; \
                        z: + '+stackingOrder+'; \
                        x:'+ xRectangle +'- ' + marginThickeness/2 + '; \
                        y: '+ yRectangle +'- ' + marginThickeness/2 + '; \
                        height: ('+ heightRectangle +' + ' + marginThickeness + ') ; \
                        width: ('+ widthRectangle +' + ' + marginThickeness + ') ; \
                        color: ('+marginThickeness +' === 0 && "'+ color +'" != "black")? "'+ color +'" : "#00000000"; \
                        border.color:  Qt.darker("'+ color +'", 1.3); \
                        border.width: ('+marginThickeness +' === 0)? 1: '+ marginThickeness +'; \
                    }',
                parent,'child' + childNumber)
}

function drawNewLine(x1Line, y1Line, x2Line, y2Line, stackingOrder, childNumber, playgroundWidth, playgroundHeight){
    x1Line= x1Line*playgroundWidth
    y1Line= y1Line*playgroundHeight
    x2Line= x2Line*playgroundWidth
    y2Line= y2Line*playgroundHeight

    Qt.createQmlObject(
                'import QtQuick 2.2;Canvas{id: child' + childNumber + ';z: + '+stackingOrder+';width: parent.width; height: parent.height;anchors.centerIn: parent;onPaint: {var ctx = child' + childNumber + '.getContext("2d");ctx.fillStyle="black";ctx.beginPath();ctx.moveTo(' + x1Line + ',' + y1Line + ');ctx.lineTo(' + x2Line + ',' + y2Line + ');ctx.stroke();ctx.closePath();}}',
                parent,'child' + childNumber)
}

function drawNewPoint(xPoint, yPoint, stackingOrder, childNumber, playgroundWidth, playgroundHeight){
    xPoint= xPoint*playgroundWidth
    yPoint= yPoint*playgroundHeight

    Qt.createQmlObject(
                'import QtQuick 2.2;Rectangle{id: child' + childNumber + ';z: + '+stackingOrder+';x:'+ xPoint + '-width/2;y: '+ yPoint + '- height/2 ;height: 5 ;width: 5 ;color: "black" ;border.color: "black";border.width:1;}',
                parent,'child' + childNumber)
}

function drawNewPolygon(color, vertices, margin, stackingOrder, childNumber, playgroundWidth, playgroundHeight){
    var temp = ""
    var  newVertices = []
    for(var i=0; i<vertices.length; i++){
        newVertices[i] = Qt.point(vertices[i].x * playgroundWidth, vertices[i].y*playgroundHeight)
    }
    vertices = newVertices
    margin = margin === 0? 0 : margin*playgroundWidth
    for(var i = 1; i < vertices.length; i++){
        temp += 'ctx.lineTo(' + vertices[i]["x"] + ', ' + vertices[i]["y"] + ');'
    }
    Qt.createQmlObject(
                'import QtQuick 2.2;Canvas{id: child' + childNumber + ';z: + '+stackingOrder+';width: parent.width; height: parent.height;anchors.centerIn: parent; \
                onPaint: {var ctx = child' + childNumber + '.getContext("2d");ctx.fillStyle="black"; \
                ctx.beginPath(); \
                ctx.moveTo( ' +  vertices[0]["x"] + ',' + vertices[0]["y"] + ');'
                + temp +
                'ctx.lineTo( ' + vertices[0]["x"] + ', ' + vertices[0]["y"] + '); \
                ctx.fillStyle = "'+ color +'"; \
                if("'+ color +'" != "black" && '+ margin +' === 0) {ctx.fill()}; \
                ctx.strokeStyle = Qt.darker("'+ color +'", 1.3); \
                ctx.lineWidth = '+ margin +'; \
                ctx.stroke(); \
                ctx.stroke();ctx.closePath();}}',
                parent,'child' + childNumber)
}




function drawNewZone(type, properties, childnumber, playgroundWidth, playgroundHeight){
    if(parent == null){
        console.info("You forgot to set a visual parent with setVisualParent or your parent is null")

    }else{

        console.info("AFTER SIGNAL: Type " + type + " Properties " + properties + " playground width " + playgroundWidth + " playgroundHeight " + playgroundHeight)

        for (var prop in properties) {
            console.info("Object item:", prop, "=", properties[prop])
        }

        switch(type){
        case CelluloZoneTypes.CIRCLEINNER:
            drawNewCircle("blue", properties["r"], properties["x"], properties["y"], 0, properties["stackingOrder"], childnumber, playgroundWidth, playgroundHeight)
            break;
        case CelluloZoneTypes.CIRCLEBORDER:
            drawNewCircle("blue", properties["r"], properties["x"], properties["y"], properties["marginThickeness"], properties["stackingOrder"], childnumber, playgroundWidth, playgroundHeight)
            break;
        case CelluloZoneTypes.CIRCLEDISTANCE:
            drawNewCircle("black", properties["r"], properties["x"], properties["y"], 0, properties["stackingOrder"], childnumber, playgroundWidth, playgroundHeight)
            break;
        case CelluloZoneTypes.RECTANGLEINNER:
            drawNewRectangle("yellow", properties["x"], properties["y"], properties["height"], properties["width"], 0, properties["stackingOrder"], childnumber, playgroundWidth, playgroundHeight)
            break;
        case CelluloZoneTypes.RECTANGLEBORDER:
            drawNewRectangle("yellow", properties["x"], properties["y"], properties["height"], properties["width"], properties["marginThickeness"], properties["stackingOrder"], childnumber, playgroundWidth, playgroundHeight)
            break;
        case CelluloZoneTypes.RECTANGLEDISTANCE:
            drawNewRectangle("black", properties["x"], properties["y"], properties["height"], properties["width"], 0, properties["stackingOrder"], childnumber, playgroundWidth, playgroundHeight)
            break;
        case CelluloZoneTypes.LINEDISTANCE:
            drawNewLine(properties["x1"], properties["y1"], properties["x2"], properties["y2"], properties["stackingOrder"], childnumber, playgroundWidth, playgroundHeight)
            break;
        case CelluloZoneTypes.POINTDISTANCE:
            drawNewPoint(properties["x"], properties["y"], properties["stackingOrder"], childnumber, playgroundWidth, playgroundHeight)
            break;
        case CelluloZoneTypes.RPOLYGONINNER:
            drawNewPolygon("green", properties["vertices"], 0, childnumber, properties["stackingOrder"], playgroundWidth, playgroundHeight)
            break;
        case CelluloZoneTypes.RPOLYGONBORDER:
            drawNewPolygon("green", properties["vertices"], properties["marginThickeness"], childnumber, properties["stackingOrder"], playgroundWidth, playgroundHeight)
            break;
        case CelluloZoneTypes.RPOLYGONDISTANCE:
            drawNewPolygon("black", properties["vertices"], 0, childnumber, properties["stackingOrder"], playgroundWidth, playgroundHeight)
            break;
        case CelluloZoneTypes.IRPOLYGONINNER:
            drawNewPolygon("purple", properties["vertices"], 0, childnumber, properties["stackingOrder"], playgroundWidth, playgroundHeight)
            break;
        case CelluloZoneTypes.IRPOLYGONBORDER:
            drawNewPolygon("purple", properties["vertices"], properties["marginThickeness"], childnumber, properties["stackingOrder"], playgroundWidth, playgroundHeight)
            break;
        case CelluloZoneTypes.IRPOLYGONDISTANCE:
            drawNewPolygon("black", properties["vertices"], 0, childnumber, properties["stackingOrder"], playgroundWidth, playgroundHeight)
            break;
        default:
            console.debug("Forgot to handle an enum case");
            break;
        }
    }
}

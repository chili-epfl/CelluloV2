#!/usr/bin/python

import sys
import os
from subprocess import call
import svgutils.transform as stf
import svgutils.compose as scp

I_COORD_SIZE = 210.0;
J_COORD_SIZE = 260.0;

def usage():
    print "Usage: generate-hex-tile-quad-a4.py -i SHEET_I_COORD -j SHEET_J_COORD";
    sys.exit(0);

#Get command line arguments
if len(sys.argv) <= 1:
    usage();
icoord = -1;
jcoord = -1;
c = 1;
while c < len(sys.argv):
    if sys.argv[c] == "--help" or sys.argv[c] == "-h":
        usage();
    elif sys.argv[c] == "-i":
        c += 1;
        if c < len(sys.argv):
            icoord = int(float(sys.argv[c]));
    elif sys.argv[c] == "-j":
        c += 1;
        if c < len(sys.argv):
            jcoord = int(float(sys.argv[c]));
    else:
        print "Invalid argument: " + sys.argv[c]
        usage();
    c += 1;

if icoord < 0:
    print "i coordinate not given or not a positive integer."
    usage();
if jcoord < 0:
    print "j coordinate not given or not a positive integer."
    usage();

print "Generating quad hex tile A4 sheet at i, j = " + str(icoord) + ", " + str(jcoord) + " ...";

#Generate QR codes
call("qrencode --background FFFFD0 -o _temp-qr00.svg -t SVG --size 3 --margin 4 -l H " + str(icoord) + "," + str(jcoord) + ",0,0", shell=True)
call("qrencode --background FFFFD0 -o _temp-qr10.svg -t SVG --size 3 --margin 4 -l H " + str(icoord) + "," + str(jcoord) + ",1,0", shell=True)
call("qrencode --background FFFFD0 -o _temp-qr01.svg -t SVG --size 3 --margin 4 -l H " + str(icoord) + "," + str(jcoord) + ",0,1", shell=True)
call("qrencode --background FFFFD0 -o _temp-qr11.svg -t SVG --size 3 --margin 4 -l H " + str(icoord) + "," + str(jcoord) + ",1,1", shell=True)

#Resize QR codes
qrwidth = 40 #mm
qrheight = 40 #mm
qrmarginleft = 5 #mm
qrtop = 255 #mm
qrtextwidth = 10 #mm
qrtextHeightToWidth = 0.64

qr00 = stf.fromfile('_temp-qr00.svg')
originalViewBox = qr00.root.get('viewBox').split()
qr00.set_size((originalViewBox[2] + "mm", originalViewBox[3] + "mm"))
qr00root = qr00.getroot()
qr00root.scale_xy(qrwidth/float(originalViewBox[2]), qrheight/float(originalViewBox[3]))
qr00root.moveto(qrmarginleft + 0*(qrtextwidth + qrwidth), qrtop)

qr10 = stf.fromfile('_temp-qr10.svg')
originalViewBox = qr10.root.get('viewBox').split()
qr10.set_size((originalViewBox[2] + "mm", originalViewBox[3] + "mm"))
qr10root = qr10.getroot()
qr10root.scale_xy(qrwidth/float(originalViewBox[2]), qrheight/float(originalViewBox[3]))
qr10root.moveto(qrmarginleft + 1*(qrtextwidth + qrwidth), qrtop)

qr01 = stf.fromfile('_temp-qr01.svg')
originalViewBox = qr01.root.get('viewBox').split()
qr01.set_size((originalViewBox[2] + "mm", originalViewBox[3] + "mm"))
qr01root = qr01.getroot()
qr01root.scale_xy(qrwidth/float(originalViewBox[2]), qrheight/float(originalViewBox[3]))
qr01root.moveto(qrmarginleft + 2*(qrtextwidth + qrwidth), qrtop)

qr11 = stf.fromfile('_temp-qr11.svg')
originalViewBox = qr11.root.get('viewBox').split()
qr11.set_size((originalViewBox[2] + "mm", originalViewBox[3] + "mm"))
qr11root = qr11.getroot()
qr11root.scale_xy(qrwidth/float(originalViewBox[2]), qrheight/float(originalViewBox[3]))
qr11root.moveto(qrmarginleft + 3*(qrtextwidth + qrwidth), qrtop)

#Human-readable texts
hstring = str(icoord) + "," + str(jcoord) + ",0,0";
estimatedHeight = len(hstring)*qrtextHeightToWidth*qrtextwidth
currenttextwidth = qrtextwidth
if estimatedHeight > qrheight:
    currenttextwidth = currenttextwidth/estimatedHeight*qrheight
qr00text = stf.TextElement(0, 0, hstring, size=currenttextwidth)
qr00text.rotate(90)
qr00text.moveto(qrmarginleft + 0*(qrtextwidth + qrwidth) + qrwidth, qrtop)

hstring = str(icoord) + "," + str(jcoord) + ",1,0";
estimatedHeight = len(hstring)*qrtextHeightToWidth*qrtextwidth
currenttextwidth = qrtextwidth
if estimatedHeight > qrheight:
    currenttextwidth = currenttextwidth/estimatedHeight*qrheight
qr10text = stf.TextElement(0, 0, hstring, size=currenttextwidth)
qr10text.rotate(90)
qr10text.moveto(qrmarginleft + 1*(qrtextwidth + qrwidth) + qrwidth, qrtop)

hstring = str(icoord) + "," + str(jcoord) + ",0,1";
estimatedHeight = len(hstring)*qrtextHeightToWidth*qrtextwidth
currenttextwidth = qrtextwidth
if estimatedHeight > qrheight:
    currenttextwidth = currenttextwidth/estimatedHeight*qrheight
qr01text = stf.TextElement(0, 0, hstring, size=currenttextwidth)
qr01text.rotate(90)
qr01text.moveto(qrmarginleft + 2*(qrtextwidth + qrwidth) + qrwidth, qrtop)

hstring = str(icoord) + "," + str(jcoord) + ",1,1";
estimatedHeight = len(hstring)*qrtextHeightToWidth*qrtextwidth
currenttextwidth = qrtextwidth
if estimatedHeight > qrheight:
    currenttextwidth = currenttextwidth/estimatedHeight*qrheight
qr11text = stf.TextElement(0, 0, hstring, size=currenttextwidth)
qr11text.rotate(90)
qr11text.moveto(qrmarginleft + 3*(qrtextwidth + qrwidth) + qrwidth, qrtop)

#Background with hex tile lines
background = stf.fromfile('hex-tile-quad-a4.svg').getroot()
background.moveto(0, 0)

#Overlay QR codes and human readable texts
labeledSvg = stf.SVGFigure("210mm", "297mm")
labeledSvg.append([background, qr00root, qr00text, qr10root, qr10text, qr01root, qr01text, qr11root, qr11text])
labeledSvg.root.set("viewBox", "0 0 210 297") #Must do this, otherwise it sets the viewbox to 210mm, 297mm which induces scaling
labeledSvg.save("_temp-labeled.svg")

call("rm _temp-qr00.svg _temp-qr10.svg _temp-qr01.svg _temp-qr11.svg", shell=True)
call("inkscape _temp-labeled.svg --export-pdf=_temp-labeled.pdf", shell=True)
call("rm _temp-labeled.svg", shell=True)
call("podofobox _temp-labeled.pdf _temp-labeled-boxed.pdf art 0 12472 59527 71716", shell=True) #Artbox (bottom left width height): 0mm 44mm 210mm 253mm
call("rm _temp-labeled.pdf", shell=True)
call("libdots-pdf-overlayer --thick -a -i _temp-labeled-boxed.pdf -o hex-tile-quad-a4-" + str(icoord) + "-" + str(jcoord) + ".pdf --xmm " + str(icoord*I_COORD_SIZE) + " --ymm " + str(jcoord*J_COORD_SIZE), shell=True)
call("rm _temp-labeled-boxed.pdf", shell=True)

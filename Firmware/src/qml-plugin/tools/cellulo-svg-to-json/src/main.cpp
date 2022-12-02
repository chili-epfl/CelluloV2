/*
 * Copyright (C) 2016 EPFL
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
 * @file main.cpp
 * @brief Entry point for the cellulo-svg-to-json tool
 * @author Ayberk Özgür
 * @date 2016-08-09
 */

#include <QTextStream>
#include <QString>
#include <CelluloSVGUtil.h>

using namespace Cellulo;

QTextStream& qPrint(){
    static QTextStream ts(stdout);
    return ts;
}

void showHelp(){
    qPrint() << "Usage: cellulo-svg-to-json [OPTIONS]\n";
    qPrint() << "    -i FILE        Name of the input SVG\n";
    qPrint() << "    -o FILE        Name of the output JSON (default is zones.json)\n";
    qPrint() << "    -z BOOL        Whether to try to optimize into more accurate zones (default is true)\n";
    qPrint() << "    -n NAME        Name prefix to all dumped zones (default is SVG_DUMP_ZONE)\n";
    qPrint() << "    -d FLOAT_NUM   DPI to use when converting to millimeters (default is 90, which is the Inkscape default)\n";
    qPrint() << "    -h,--help      Shows this help\n";
}

int main(int argc, char** argv){
    QString name("SVG_DUMP_ZONE");
    float dpi = 90.0f;
    QString inputFile("");
    QString outputFile("zones.json");
    bool optimize = true;

    for(int i=1; i<argc; i++){
        if(QString(argv[i]) == "-i")
            inputFile = argv[i + 1];
        else if(QString(argv[i]) == "-o")
            outputFile = argv[i + 1];
        else if(QString(argv[i]) == "-z"){
            QString optimizeStr(argv[i + 1]);
            optimizeStr = optimizeStr.toLower();
            if(optimizeStr == "true")
                optimize = true;
            else if(optimizeStr == "false")
                optimize = false;
            else{
                bool ok;
                int optimizeInt = (bool)optimizeStr.toInt(&ok);
                if(ok)
                    optimize = (bool)optimizeInt;
            }
        }
        else if(QString(argv[i]) == "-n")
            name = argv[i + 1];
        else if(QString(argv[i]) == "-d")
            dpi = QString(argv[i + 1]).toFloat();
        else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
            showHelp();
            return 0;
        }
    }

    qPrint() << "Input file: " << inputFile << "\n";
    qPrint() << "Output file: " << outputFile << "\n";
    qPrint() << "Optimize: " << optimize << "\n";
    qPrint() << "Zone name prefix: " << name << "\n";
    qPrint() << "DPI: " << dpi << "\n";

    QString returnMessage = CelluloSVGUtil::dumpAllPathsToJSON(inputFile, outputFile, "POLYBEZIERCLOSESTT", name, dpi, optimize);
    qPrint() << returnMessage.toLatin1().constData() << "\n";
    return 0;
}

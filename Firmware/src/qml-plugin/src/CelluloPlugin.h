/*
 * Copyright (C) 2015 EPFL
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
 * @file CelluloPlugin.h
 * @brief Header for exposing Cellulo objects as QML objects
 * @author Ayberk Özgür
 * @date 2016-06-26
 */

/**
 * @mainpage cellulo-qml-plugin
 *
 * This is a qml module for communicating with Cellulo robots over Bluetooth. See README.md for more information.
 *
 * Start with Modules on the left.
 */

#ifndef CELLULOPLUGIN_H
#define CELLULOPLUGIN_H

#include <QQmlExtensionPlugin>

namespace Cellulo{

/** @cond DO_NOT_DOCUMENT */

/**
 * @brief Object that exposes all relevant Cellulo objects as QML objects
 */
class CelluloPlugin : public QQmlExtensionPlugin {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
    void initializeEngine(QQmlEngine *engine, const char *uri);
};

/** @endcond */

}

#endif // CELLULOPLUGIN_H

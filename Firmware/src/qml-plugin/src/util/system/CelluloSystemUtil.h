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
 * @file CelluloSystemUtil.h
 * @brief System utilities
 * @author Ayberk Özgür
 * @date 2017-03-20
 */

#ifndef CELLULOSYSTEMUTIL_H
#define CELLULOSYSTEMUTIL_H

#include <QObject>

namespace Cellulo{

/**
 * @addtogroup util
 * @{
 */

/**
 * @brief Object that provides system utilities
 * @singleton
 */
class CelluloSystemUtil : public QObject {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

public:

    /** @cond DO_NOT_DOCUMENT */

    CelluloSystemUtil(QObject* parent = 0);
    ~CelluloSystemUtil();

    /** @endcond */

    /**
     * @brief Executes a system command
     *
     * @param command The command
     * @param arguments List of arguments
     * @return Return code of the executed command
     */
    Q_INVOKABLE static int exec(QString const& command, QStringList const& arguments);

    /**
     * @brief Gets a writable location specific to the app
     *
     * @return Location that is guaranteed to be writable
     */
    Q_INVOKABLE static QString getAppDataLocation();

};

/** @} */

}

#endif // CELLULOSYSTEMUTIL_H

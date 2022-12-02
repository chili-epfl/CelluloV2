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
 * @file CelluloSystemUtil.cpp
 * @brief System utilities
 * @author Ayberk Özgür
 * @date 2017-03-20
 */

#include "CelluloSystemUtil.h"

#if defined(Q_OS_WIN)
	#include <QDebug>
#else
	#include <QProcess>
#endif

#include <QStandardPaths>

namespace Cellulo{

CelluloSystemUtil::CelluloSystemUtil(QObject* parent) : QObject(parent){}

CelluloSystemUtil::~CelluloSystemUtil(){}

int CelluloSystemUtil::exec(QString const& command, QStringList const& arguments){
	#if defined(Q_OS_WIN)
        Q_UNUSED(command)
        Q_UNUSED(arguments)
		qWarning() << "CelluloSystemUtil::exec(): Not implemented on WinRT";
		return -1;
	#else
		QProcess proc;
		proc.start(command, arguments);
		proc.waitForFinished(-1);
		return proc.exitCode();
	#endif
}

QString CelluloSystemUtil::getAppDataLocation(){
	return QStandardPaths::writableLocation(QStandardPaths::StandardLocation::AppDataLocation);
}

}

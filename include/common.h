/*!
 * This file is a part of Backy project, a simple backup creation and
 * maintaining solution.
 * 
 * Copyright (C) 2018 Marek Felsoci <marek.felsoci@gmail.com> (Feldev)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 * \file common.h
 * \brief Provides various pre-processor macros used throughout the
 *        whole project and includes commonly used headers.
 * \author Marek Felsoci
 * \date 2018-27-06
 * \version 1.0
 */
#ifndef __COMMON_H
#define __COMMON_H

#include <QString>
#include <QDebug>
#include <QtGlobal>
#include <QStandardPaths>

#if defined Q_OS_LINUX
/*!
 * \def DEFAULT_LOGS_LOCATION
 * \brief Path to the default location where application's log files
 *        will be stored.
 * 
 * On Linux systems, a folder named '.backy' will be created in the
 * home directory. In the case of Windows(R), the application will
 * create a subfolder with the same name as the application in a folder
 * named after the application's developer. The latter will be located
 * in the user's configuration directory, e.g. 'Documents and Settings\
 * <user>\AppData\Local' on Windows(R) Vista(TM) and later.
 * 
 * \see Logger
 * \see main.cpp
 */
#define DEFAULT_LOGS_LOCATION QDir::homePath() + "/.backy"
#elif defined Q_OS_WIN
/*!
 * \def DEFAULT_LOGS_LOCATION
 * \brief Path to the default location where application's log files
 *        will be stored.
 * 
 * On Linux systems, a folder named '.backy' will be created in the
 * home directory. In the case of Windows(R), the application will
 * create a subfolder with the same name as the application in a folder
 * named after the application's developer. The latter will be located
 * in the user's configuration directory, e.g. 'Documents and Settings\
 * <user>\AppData\Local' on Windows(R) Vista(TM) and later.
 * 
 * \see Logger
 * \see main.cpp
 */
#define DEFAULT_LOGS_LOCATION \
        QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)
#else
#error Unsupported operating system!
#endif

/*!
 * \def KILOBYTE
 * \brief Represents the number of bytes corresponding to one kilobyte.
 */
#define KILOBYTE (qint64) 1024

/*!
 * \def MEGABYTE
 * \brief Represents the number of bytes corresponding to one megabyte.
 */
#define MEGABYTE (KILOBYTE * 1024)

/*!
 * \def GIGABYTE
 * \brief Represents the number of bytes corresponding to one gigabyte.
 */
#define GIGABYTE (MEGABYTE * 1024)

/*!
 * \def TERABYTE
 * \brief Represents the number of bytes corresponding to one terabyte.
 */
#define TERABYTE (GIGABYTE * 1024)

/*!
 * \def WORKER_EVENT_LOG_FILE_NAME
 * \brief File name of the application's event log.
 * 
 * Initial value is \c "events.log".
 */
#define WORKER_EVENT_LOG_FILE_NAME "events.log"

/*!
 * \def WORKER_ERROR_LOG_FILE_NAME
 * \brief File name of the application's error log.
 * 
 * Initial value is \c "errors.log".
 */
#define WORKER_ERROR_LOG_FILE_NAME "errors.log"

/*!
 * \def DEFAULT_COPY_BUFFER_SIZE
 * \brief Default size (in megabytes) of copy buffer used by file copy
 *        function which serves during backup process.
 * 
 * Initial value is \c 1.
 */
#define DEFAULT_COPY_BUFFER_SIZE 1

/*!
 * \def DEFAULT_SYNCHRONIZE
 * \brief Default value of boolean deciding whether backup source
 *        location should be only synchronized with a previously created
 *        backup in target location.
 * 
 * Initial value is \c false.
 */
#define DEFAULT_SYNCHRONIZE false

/*!
 * \def DEFAULT_KEEP_OBSOLETE
 * \brief Default value of boolean deciding whether any file in backup
 *        target location which does not exist in source location
 *        anymore should be kept or deleted.
 * 
 * Initial value is \c false.
 */
#define DEFAULT_KEEP_OBSOLETE false

/*!
 * \def DEFAULT_COMPARISON_CRITERION
 * \brief Default comparison criterion.
 * 
 * This is used to decide whether a file in backup source location is
 * superior to its version potentially present in target location.
 * 
 * Initial value is \c CRITERION_MORE_RECENT which means that the most
 * recent version of the file will be conserved.
 * \see Criterion
 */
#define DEFAULT_COMPARISON_CRITERION CRITERION_MORE_RECENT

/*!
 * \def DEFAULT_LOCALE_CODE
 * \brief Default code of the language to display the application's
 *        graphical user interface in.
 * 
 * Initial value is \c "sk" which corresponds to \e Slovak.
 */
#define DEFAULT_LOCALE_CODE "sk"

/*!
 * \def LOCALE_RELATIVE_PATH
 * \brief Path to the folder which contains application's translation
 *        files relatively to the application's executable location.
 * 
 * Initial value is \c "lang".
 */
#define LOCALE_RELATIVE_PATH "lang"

/*!
 * \def CEILD(dividend, divisor)
 * \brief Performs integer division of \a dividend by \a divider and if the
 *        latter does not divide the former it adds 1 to the result of their
 * division.
 */
#define CEILD(dividend, divisor) ( !(dividend % divisor) ? (dividend / divisor) : ((dividend / divisor) + 1) )

#endif // __COMMON_H

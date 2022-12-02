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
 * @file TranslationTool.h
 * @brief Header for QML-side dynamic translation
 * @author Ayberk Özgür
 * @date 2016-08-15
 */

#ifndef TRANSLATIONTOOL_H
#define TRANSLATIONTOOL_H

#include <QtGui>

namespace Cellulo{

/**
 * @addtogroup util
 * @{
 */

/**
 * @brief Object that provides a language translation for the entire app.
 *
 * To use, create one `TranslationTool` named e.g `tr`, and concatenate every user-readable string in the app with
 * `tr.estr`. For example, a text property in another object would look like `text: tr.estr + "Text that would
 * normally be here."`
 */
class TranslationTool : public QObject {
    /* *INDENT-OFF* */
    Q_OBJECT
    /* *INDENT-ON* */

    /** @brief List of languages (e.g `"en"`) versus their translation resource files loaded into the app */
    Q_PROPERTY(QVariantMap languageList READ getLanguageList WRITE setLanguageList NOTIFY languageListChanged)

    /** @brief An empty string (read-only) to be concatenated to all user-visible strings in the app to be translated if dynamic modification of the language during runtime is desired */
    Q_PROPERTY(QString estr READ getEmptyString NOTIFY languageChanged)

public:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Creates new translation tool and QTranslator and attaches it to this object
     */
    TranslationTool(QObject* parent = 0);

    /**
     * @brief Destroys the translation tool and the QTranslator
     */
    ~TranslationTool();

    /**
     * @brief Sets the language-translation file map
     *
     * @param newList New map from language (QString) to translation file (QString)
     */
    void setLanguageList(const QVariantMap& newList);

    /**
     * @brief Gets the language list
     *
     * @return Map from language (QString) to translation file (QString)
     */
    QVariantMap getLanguageList(){ return languageList; }

    /**
     * @brief Gets an empty string (hack to dynamically update texts in the UI)
     *
     * @return ""
     */
    QString getEmptyString(){ return ""; }

    /* @endcond */

signals:

    /** @cond DO_NOT_DOCUMENT */

    /**
     * @brief Emitted when the language to translation file list changes
     */
    void languageListChanged();

    /**
     * @brief Emitted when language has changed
     */
    void languageChanged();

    /** @endcond */

public slots:

    /**
     * @brief Chooses language
     *
     * @param language Language as provided to the translation list or "default" to uninstall translator
     */
    Q_INVOKABLE void selectLanguage(const QString& language);

private:

    QTranslator translator;   ///< Translator to be attached to the qApp
    QVariantMap languageList; ///< Language (QString) to translation file (QString) list

};

/** @} */

}

#endif

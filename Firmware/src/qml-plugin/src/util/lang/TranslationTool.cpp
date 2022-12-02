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
 * @brief Source for QML-side dynamic translation
 * @author Ayberk Özgür
 * @date 2016-08-15
 */

#include "TranslationTool.h"

namespace Cellulo{

TranslationTool::TranslationTool(QObject* parent) :
    QObject(parent),
    translator(this)
{}

TranslationTool::~TranslationTool(){}

void TranslationTool::setLanguageList(const QVariantMap& newList){
    languageList = newList;
    emit languageListChanged();
}

void TranslationTool::selectLanguage(const QString& language){
    if(language == QString("default") || language.isEmpty()){
        qApp->removeTranslator(&translator);
        emit languageChanged();
    }
    else{
        QVariant var = languageList.value(language);
        if(var.isValid()){
            if(translator.load(var.toString())){
                qApp->installTranslator(&translator);
                emit languageChanged();
            }
            else
                qWarning() << "TranslationTool::selectLanguage(): Language couldn't be loaded, file might be invalid.";
        }
        else
            qWarning() << "TranslationTool::selectLanguage(): Invalid language provided.";
    }
}

}

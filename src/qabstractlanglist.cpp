/*********************************************************************************************
    QAbstractLangList: An abstract class listing available languages. Can be Used for dynamic language loading. Designed for Qt3/4 and Motorola EZX
    Copyright (C) 2010 Wangbin <wbsecg1@gmail.com>
        (aka. nukin in ccmove & novesky in http://forum.motorolafans.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*********************************************************************************************/
/******************
***Last edited 2010-04-22
*********************/
/*!
  TODO:
        ZConfig for ezx
        Repalce QLocale::languageToString(..) by a language-names table Translated by different language.
        Translate "System" in different languages.
        QMap<QString,QString> langMap; 用于setConfigDir等刷新列表
*/

#include "qabstractlanglist.h"

#include <qapplication.h>
#include <qdir.h>
#include <qfile.h>
#ifdef QT_VERSION4
#include <QLocale>
#include <QSettings>
#include <QtDebug>
#include <QTranslator>
#else
//#include <qtranslate.h>
#include <qtextcodec.h>
#include <qtextstream.h>
#include <qlistbox.h>
#if CONFIG_EZX
#include <SetupSetManager.h>
#include <ZLanguage.h>
#endif //CONFIG_EZX
#endif //QT_VERSION4


static struct languageName {
    int index;
    const char* name;
    const char* translatedName;
    const char* sys;

} language_vec[]={
    {0, "US", "English", "System"},
    {1, "CN", "简体中文",  "系统"},
    {2, "HK", "繁体中文",  "系统"},
    {3, "JP", "Japanese", "System"},
    {4, "RU", "Russua",   "System"},
    {0, 0, 0, 0},
};

static QString find_lang(const QString& l) {
#ifdef QT_VERSION4
    QString lang = l.right(2).toUpper();
#else
    QString lang = l.right(2).upper();
#endif //QT_VERSION4
    for (int i = 0; language_vec[i].name; ++i) {
        if (lang == QString(language_vec[i].name))
            return QString::fromUtf8(language_vec[i].translatedName);
    }
    return l;
}

static QString find_syslang(const QString& l) {
#ifdef QT_VERSION4
    QString lang = l.right(2).toUpper();
#else
    QString lang = l.right(2).upper();
#endif //QT_VERSION4
    for (int i = 0; language_vec[i].name; ++i) {
        if (lang == QString(language_vec[i].name))
            return QString::fromUtf8(language_vec[i].sys);
    }
    return "System";
}


struct QAbstractLangListPrivate
{
    QAbstractLangListPrivate() {
        ts = new QTranslator(qApp);
#ifdef QT_VERSION4
        thisQmDir = ":/i18n";
#else
        thisQmDir = "i18n";
#endif
        haveSysLangCode = false;
        qApp->installTranslator(ts); //频繁的install会产生问题吗
    }

    //int qm_count;
    QTranslator *ts;
    QStringList qms;
    QString cfg;
    QString qmDir, thisQmDir, languageCode;
    bool haveSysLangCode;
    QString qmFilter;
    int langIndexOld;
};


QAbstractLangList::QAbstractLangList(const QString& langCode ,const QString& qmfilter,const QString &qmd,const QString &conf)
{
#ifdef QT_VERSION4
    Q_INIT_RESOURCE(resources);
#endif //QT_VERSION4
    d = new QAbstractLangListPrivate;
    d->cfg = conf;
    d->qmDir = qmd;
    d->languageCode = langCode;
    d->qmFilter = qmfilter;

    if (d->languageCode.isEmpty()) {
#if !CONFIG_EZX
        QSettings ss(d->cfg,QSettings::NativeFormat);
        int index = ss.value("Languages/LanguageIndex",0).toInt();
        d->languageCode = ss.value(QString("Languages/Language-%1").arg(index),"en").toString();
        //if(d->languageCode=="sys") d->languageCode=QLocale::system().name();  //"sys" is useful when set the default index

#else
        SETUP_SET_Manager mgr(d->cfg, false);
        int index;
        mgr.getSettingInfo("Languages","LanguageIndex",index,0);
        mgr.getSettingInfo("Languages",QString("Language-%1").arg(index),d->languageCode,"en",true);
                //if(d->languageCode=="sys") d->languageCode=ZLanguage::getSystemlanguageCode();
#endif //CONFIG_EZX
    }
    qDebug("langcode is %s", qPrintable(d->languageCode));
}

QAbstractLangList::~QAbstractLangList()
{
    qDebug("%s", __FUNCTION__);
    delete d;
}

void QAbstractLangList::setQmFilter(const QString& qf)
{
    d->qmFilter = qf;
    updateLangList();
}

int QAbstractLangList::langCount() const
{
    return d->qms.count();
}

void QAbstractLangList::clear()
{
    d->qms.clear();
}

void QAbstractLangList::setCurrentLangCode(const QString& langCode)
{
    int index = langIndex(langCode);
    d->languageCode = langCode;
    changeLangIndex(index);
}

#ifdef QT_VERSION4
void QAbstractLangList::setCurrentLang(QLocale::Language lang)
{
    QLocale l(lang);
    setCurrentLangCode(l.name().left(2));
}

#endif //QT_VERSION4

/*!
  change current index.
  if sytem language exists, then will update the translation of item "System" through changeSysText()
*/
bool QAbstractLangList::changeLangIndex(int index)
{
    bool changed = false;
    if (index != d->langIndexOld) {
        changed = true;
        d->langIndexOld = index;
        d->languageCode = langCode(d->langIndexOld);
        changeSysText();
        //qDebug("index %d", d->langIndexOld);
        //emit langChanged();
        if (d->ts->load(d->qms[index], d->qmDir) || d->ts->load(d->qms[index], d->thisQmDir))
            qDebug("%s loaded", qPrintable(d->qms[index]));
//if this is in a dialog and we canceled, don't change the settings
#if !CONFIG_EZX
        QSettings settings(d->cfg, QSettings::NativeFormat);
        settings.setValue("Languages/LanguageIndex", index);
#else
        SETUP_SET_Manager mgr(d->cfg, false);
        mgr.setSettingInfo("Languages", "LanguageIndex", index);
#endif //CONFIG_EZX
    }
    return changed;
}

void QAbstractLangList::setConfigFile(const QString& newCfg)
{
//handle error message
//#ifdef  QT_VERSION4
//    QFile::copy(d->cfg,newCfg);
    QFile::remove(d->cfg);
//    d->cfg=newCfg;
//#else
    d->cfg = newCfg;
    updateLangList();
//#endif // QT_VERSION4
}

void QAbstractLangList::setQmDir(const QString& dir)
{
    d->qmDir = dir;
    updateLangList();
}

void QAbstractLangList::setThisQmDir(const QString& dir)
{
    d->thisQmDir = dir;
    updateLangList();
}


void QAbstractLangList::updateLangList()
{
    clear();
    QString sysLang;
#ifdef QT_VERSION4
    sysLang = QLocale::system().name();
#else
# if CONFIG_EZX
    SETUP_SET_Manager mgr(d->cfg, false);
    sysLang = ZLanguage::getSystemLanguageCode();
# endif //CONFIG_EZX
#endif //QT_VERSION4
    qDebug("system language is: %s", qPrintable(sysLang));
    //QString qmPath=QDir::currentDirPath()+"/i18n";
    QDir dir(d->qmDir);//Path);

//filter
#ifdef QT_VERSION4
    d->qms = dir.entryList(QStringList(QString("%1-*.qm").arg(d->qmFilter)));
    d->qms << QDir(d->thisQmDir).entryList(QStringList(QString("%1-*.qm").arg(d->qmFilter)));
    //d->qms = d->qms.filter(QRegExp("^[^q][^l][^a][^n][^g][^c][^o][^m][^b][^o]"));
#else
# if CONFIG_EZX
    d->qms = dir.entryList(QString("%1-*.qm").arg(d->qmFilter));
# else
    d->qms = dir.entryList(QString("%1_*.qm").arg(d->qmFilter)); //zh_CN
    //d->qms = d->qms.grep(QRegExp("^[^q][^l][^a][^n][^g][^c][^o][^m][^b][^o]"));
# endif //CONFIG_EZX
#endif
    QString sysLangQm;
#if !CONFIG_EZX
    QSettings settings(d->cfg, QSettings::NativeFormat);
#endif //CONFIG_EZX
    int i = 0;
    // QTranslator translator(0);
    for (; i < (int)d->qms.count(); ++i) {
        //translator.load(d->qms[i], qmDir);//qmPath);
        QString locale = d->qms[i];

/*
         if(locale.contains("qabstractlanglist")) {
#ifdef QT_VERSION4
            d->qms.removeAt(i);
#else
            d->qms.remove(d->qms[i]);
#endif //QT_VERSION4
            --i;
            continue;
    }
*/
//how to set item text if QAbstractLangList have been built into a library?

#if !CONFIG_EZX
# ifdef QT_VERSION4
        locale.remove(0, locale.indexOf('-') + 1);
        locale.chop(3);

        //locale=lc_cc, e.g. zh_CN
        //if(translator.load("qabstractlanglist-"+locale,d->thisQmDir)) qDebug("load succesful");;
        //QString language = translator.translate("Languages","TranslateMe");
        //insertItem(i,tr("%1").arg(language));
# else
        locale = locale.mid(locale.find('-') + 1);
        locale.truncate(locale.find('.'));
# endif //QT_VERSION4

        settings.setValue(QString("Languages/Language-%1").arg(i), locale);
        QString language = QLocale::languageToString(QLocale(locale).language());

#else
        locale = locale.mid(locale.find('_') + 1);
        //locale=lc-cc, e.g. zh-cn
        locale.truncate(locale.find('.'));
        mgr.setSettingInfo("Languages", QString("Language-%1").arg(i),locale,true);
        QString language=locale;
        //if(translator.load("qabstractlanglist-"+locale,d->thisQmDir)) qDebug("load succesful");;
        //QTranslatorMessage message = translator.findMessage("Languages", "TranslateMe","");
        //QString language = message.translation();
#endif //CONFIG_EZX
        insertItem(i, find_lang(language), flag(locale));
        if (sysLang == locale) {
            d->haveSysLangCode = true;
            sysLangQm = d->qms[i];
        }
    }
    qDebug("qm total: %d\n ", d->qms.count());
    if (d->haveSysLangCode) {
        d->qms << sysLangQm;
#if !CONFIG_EZX
        settings.setValue(QString("Languages/Language-%1").arg(i),"sys");//sysLang);
#else
        mgr.setSettingInfo("Languages", QString("Language-%1").arg(i), "sys", true);//sysLang,true);
#endif //CONFIG_EZX
        insertItem(i, find_syslang(sysLang), flag(sysLang));
    }

//2010-04-17; we need set currentIndex after setQmDir()
    d->langIndexOld = langIndex(d->languageCode);
    //if(translator.load(d->qms[d->langIndexOld], qmDir)) qDebug("load succesful");;
    setCurrentIndex(d->langIndexOld);
}

int QAbstractLangList::langIndex(const QString& langCode)
{
    if (langCode == "sys")	{
        if (!d->haveSysLangCode)
            return d->langIndexOld;
//#if CONFIG_EZX
//		langCode=QLocale::system().name();
//#else
//	if(langCode=="sys")	langCode=ZLanguage::systemLocale();
//#endif //CONFIG_EZX
        return (int)d->qms.count() - 1;
    }

    for (int i = 0; i<(int)d->qms.count(); ++i)
        if (d->qms[i].contains(langCode))
            return i;
    return 0;
}

QString QAbstractLangList::langCode(int idx)
{
    if (d->haveSysLangCode && idx == (int)d->qms.count()-1)
        return "sys";
    QString locale=d->qms[idx];
#ifdef QT_VERSION4
    locale.remove(0, locale.indexOf('-') + 1);
    locale.chop(3);
#else
    locale = locale.mid(locale.find('-') + 1);
    locale.truncate(locale.find('.'));
#endif //QT_VERSION4
    return locale;
}


void QAbstractLangList::changeSysText()
{
    if(!d->haveSysLangCode)
        return;
   // QString sysText="System";
    QString locale = d->languageCode;
    if (locale == "sys")
#if !CONFIG_EZX
        locale = "qlangwidget-" + QLocale::system().name();
#else
        locale = "qlangwidget-" + ZLanguage::getSystemLanguageCode();
#endif //CONFIG_EZX
    setItemText(d->qms.count() - 1, find_syslang(locale));
}


QIcon QAbstractLangList::flag(const QString &lc) const
{
#ifdef QT_VERSION4
    return QIcon(":/flags/" + lc.right(2) + ".png");
#else
    return QIcon("flags/" + lc.right(2).upper() + ".png");
#endif //QT_VERSION4
}


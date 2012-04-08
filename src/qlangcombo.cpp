/*********************************************************************************************
    QLangCombo: An extended QComboBox listing available languages. Can be Used for dynamic language loading. Designed for Qt3/4 and Motorola EZX
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
    Usage:

read the configure file (default i18n.cfg) first. Get the language code. if the value is "sys", then you should get language code
by using QLocale::system().name(), otherwise, just use the value;

when startting up a program,
code:
        QSettings ss("i18n.cfg",QSettings::NativeFormat);
        int index=ss.value("Language/LanguageIndex",0).toInt();
        QString langCode=ss.value(QString("Language/Language-%1").arg(index),"en").toString();
        if(langCode=="sys") langCode=QLocale::system().name();
        translator.load(appname+"-"+langCode,"i18n");


qm file's name: app-zh_CN.qm
        {...
        languageMenu=new QLangCombo(this,"zh_CN","ProgrameName");//,"i18n","i18n.cfg");
        //languageMenu->setConfigFile(CFG);
        connect(languageMenu,SIGNAL(langChanged(int)),this,SLOT(retranslateUI()));
        //in qt4, we can use changeEvent instead
        ...
        }

        A::retranslateUI()
        {
                ...
                a->setText(tr("hello wangbin");
                ...
        }
*/

#include "qlangcombo.h"

#ifndef QT_VERSION4
//QComboBox
#define insertItem(index, text, data) insertItem(text, data, index)
#define setCurrentIndex(index) setCurrentItem(index)
#endif //QT_VERSION4

QLangCombo::QLangCombo(QWidget *parent, const QString &langCode,const QString& qmfilter, const QString &qmd, const QString &conf)
    :QComboBox(parent)
    ,QAbstractLangList(langCode, qmfilter, qmd, conf)
{
    setEditable(false);
    QComboBox::connect(this, SIGNAL(activated(int)), SLOT(setLangIndex(int)));

    updateLangList();
}

void QLangCombo::clear()
{
    QAbstractLangList::clear(); //不会自动调用?
    QComboBox::clear();
}

void QLangCombo::setItemText(int index, const QString &name)
{
#ifdef QT_VERSION4
    QComboBox::setItemText(index,name);
#else
    QComboBox::listBox()->changeItem(name, index);
#endif //QT_VERSION4
}

void QLangCombo::insertItem(int index, const QString &name,const QIcon& f)
{
    QComboBox::insertItem(index, f, name);
}

void QLangCombo::setCurrentIndex(int index)
{
    QComboBox::setCurrentIndex(index);
}

void QLangCombo::setLangIndex(int index)
{
    if (changeLangIndex(index))
        emit langChanged();
}

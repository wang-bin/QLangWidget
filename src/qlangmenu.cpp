/*********************************************************************************************
    QLangMenu: An extended QMenu listing available languages. Can be Used for dynamic language loading. Designed for Qt3/4 and Motorola EZX
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

#include "qlangmenu.h"
/******************
***Last edited 2010-04-22
*********************/
/*!
 *TODO: EZX support
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
        languageMenu=new QLangMenu(this,"zh_CN","ProgrameName");//,"i18n","i18n.cfg");
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
                ...
        }
*/
#ifdef QT_VERSION4
#include <QAction>
#include <QActionGroup>
#include <QMenu>
#endif //QT_VERSION4

QLangMenu::QLangMenu(QWidget *parent, const QString &langCode,const QString& qmfilter, const QString &qmd, const QString &conf)
    :QMenu(parent)
    ,QAbstractLangList(langCode, qmfilter, qmd, conf)
{
#ifdef QT_VERSION4
    setTitle(tr("&Language"));
    languageActionGroup = new QActionGroup(this);
    connect(languageActionGroup, SIGNAL(triggered(QAction *)), SLOT(switchLanguage(QAction *)));
#else
    connect(this, SIGNAL(activated(int)), SLOT(setLangId(int)));
#endif //QT_VERSION4
    updateLangList();
}

void QLangMenu::clear()
{
    QAbstractLangList::clear();
    QMenu::clear();
}

void QLangMenu::setItemText(int index, const QString &name)
{
#ifdef QT_VERSION4
    languageActionGroup->actions().at(index)->setText(name);
#else
    changeItem(index, name);
#endif //QT_VERSION4
}


void QLangMenu::insertItem(int index, const QString &name,const QIcon& ico)
{
#ifdef QT_VERSION4
    QAction *action = new QAction(name, this);
    action->setCheckable(true);
    action->setData(index);
    action->setIcon(ico);

    addAction(action);
    languageActionGroup->addAction(action);
#else
    //insertItem(ico,name,index);
    insertItem(index, name, ico);
#endif //QT_VERSION4
}

void QLangMenu::setCurrentIndex(int index)
{
#ifdef QT_VERSION4
    QList<QAction *> acts = languageActionGroup->actions();
    for (int i = 0; i<acts.size(); ++i)
        acts.at(i)->setChecked(i == index);
#else
    for (int i = 0; i<langCount(); ++i)
        setItemChecked(i, i == index);
#endif //QT_VERSION4
}

void QLangMenu::switchLanguage(QAction *action)
{
#ifdef QT_VERSION4
    int index = action->data().toInt();
    if (changeLangIndex(index))
        emit langChanged();
#endif //QT_VERSION4
}

void QLangMenu::setLangId(int id)
{
    if (changeLangIndex(id)) {
        setCurrentIndex(id);
        emit langChanged();
    }
}

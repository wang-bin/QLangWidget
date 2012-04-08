/******************************************************************************
    qabstractlanglist.h: description
    Copyright (C) 2012 Wang Bin <wbsecg1@gmail.com>
    
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
******************************************************************************/


#ifndef QABSTRACTLANGLIST_H
#define QABSTRACTLANGLIST_H

#include "qlangwidget_global.h"

#ifdef QT_VERSION4
#include <QLocale>
#include <QIcon>
#else
#include <qpixmap.h>
#define QIcon QPixmap
#endif //QT_VERSION4
#include <qstringlist.h>


class QWidget;
class QTranslator;
struct QAbstractLangListPrivate;

class QAbstractLangList
{
public:
    //first parameter langId should be improved. change to const QString langCode
    QAbstractLangList(const QString& langCode = QString(),const QString& qmfilter = "*",const QString &qmd = "i18n",const QString &conf = "i18n.cfg");
    virtual ~QAbstractLangList() =0;

    void setQmFilter(const QString& qf);
    void setConfigFile(const QString& file);
    void setQmDir(const QString& dir);
    void setThisQmDir(const QString& dir); //dir of qabstractlanglist-lc_cc.qm
    int langIndex(const QString& langCode);
    QString langCode(int);
    void setCurrentLangCode(const QString&);
#ifdef QT_VERSION4
    void setCurrentLang(QLocale::Language lang);
#endif //QT_VERSION4
    int langCount() const;

protected:
    void changeSysText();
    void updateLangList();
    QIcon flag(const QString& langCode) const;
    virtual void clear();
    virtual void insertItem(int ,const QString&,const QIcon& ) =0;
    virtual void setItemText(int ,const QString& ) =0;
    virtual void setCurrentIndex(int ) =0;
    //virtual findQms();
    virtual bool changeLangIndex(int);

private:
    QAbstractLangListPrivate* d;
};

#endif // QABSTRACTLANGLIST_H

/******************************************************************************
    qlangmenu.h: description
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

#ifndef QLANGMENU_H
#define QLANGMENU_H

#include "qabstractlanglist.h"

class QAction;
#ifdef QT_VERSION4
#include <QMenu>
class QActionGroup;
#else
#include <qpopupmenu.h>
#define QMenu QPopupMenu
#endif //QT_VERSION4

class QLangMenu : public QMenu, private QAbstractLangList
{
    Q_OBJECT
public:
    QLangMenu(QWidget* parent = 0, const QString& langCode = QString(), const QString& qmfilter = "*" \
            , const QString &qmd = "i18n",const QString &conf = "i18n.cfg");
    virtual ~QLangMenu() {}

signals:
    void langChanged(); //use changEvent in Qt4

public slots:
    void setLangId(int);
//#ifdef QT_VERSION4
    void switchLanguage(QAction *);
//#endif //QT_VERSION4  //qt-2.3.8 error in pp, qt4 doesn't know it's a slot ???

private:
    void clear();
    void insertItem(int index,const QString& name,const QIcon& f = QIcon());
    void setItemText(int index,const QString& name);
    void setCurrentIndex(int index);

private:
#ifdef QT_VERSION4
    QActionGroup *languageActionGroup;
#endif //QT_VERSION4

};

#endif // QLANGMENU_H

/******************************************************************************
    qlangwidget.h: description
    Copyright (C) 2011-2012 Wang Bin <wbsecg1@gmail.com>
    
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
#ifndef QLANGCOMBO_H
#define QLANGCOMBO_H

#include "qabstractlanglist.h"
#ifdef QT_VERSION4
#include <QComboBox>
#else
#include <qcombobox.h>
#include <qlistbox.h>
#endif //QT_VERSION4

class QLangCombo : public QComboBox, private QAbstractLangList
{
    Q_OBJECT
public:
    QLangCombo(QWidget* parent = 0,const QString& langCode = QString(),const QString& qmfilter = "*", const QString &qmd = "i18n",const QString &conf = "i18n.cfg");
    virtual ~QLangCombo() {}

signals:
    void langChanged(); //use changEvent in Qt4

public slots:
    void setLangIndex(int);

private:
    void clear();
    void insertItem(int index,const QString& name,const QIcon& f=QIcon());
    void setItemText(int index,const QString& name);
    void setCurrentIndex(int index);

};

#endif // QLANGCOMBO_H


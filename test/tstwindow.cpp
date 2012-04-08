/******************************************************************************
    tstwindow.cpp: description
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


#include "tstwindow.h"
#include <QtGui/QMenuBar>
#include <QtGui/QToolBar>
#include <QtGui/QLabel>

#include "qlangcombo.h"
#include "qlangmenu.h"

TstWindow::TstWindow(QWidget *parent) :
    QMainWindow(parent)
{
    lc = new QLangCombo(this);
    lm = new QLangMenu(this);

    connect(lc, SIGNAL(langChanged()), SLOT(retranslateUi()));
    connect(lm, SIGNAL(langChanged()), SLOT(retranslateUi()));

    menuBar()->addMenu(lm);
    QToolBar *tb = addToolBar("LangCombo");
    tb->addWidget(lc);
    QLabel *lb = new QLabel(this);
    setCentralWidget(lb);
    retranslateUi();
}

void TstWindow::retranslateUi()
{
    qDebug("%s", __FUNCTION__);
    QLabel *lb = static_cast<QLabel*>(centralWidget());
    lb->setText(tr("Dynamic language switching test"));
}

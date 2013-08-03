/***************************************************************************
    File                 : SettingsGeneralPage.h
    Project              : LabPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2008-2013 by Alexander Semke
    Email (use @ for *)  : alexander.semke*web.de
    Description          : general settings page
                           
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor,                    *
 *   Boston, MA  02110-1301  USA                                           *
 *                                                                         *
 ***************************************************************************/
#ifndef SETTINGSGENERALPAGE_H
#define SETTINGSGENERALPAGE_H

#include "SettingsPage.h"
#include "ui_settingsgeneralpage.h"

class SettingsGeneralPage : public SettingsPage {
    Q_OBJECT

public:
    SettingsGeneralPage(QWidget* parent);

    virtual void applySettings();
    virtual void restoreDefaults();

private:
	Ui::SettingsGeneralPage ui;
	bool m_changed;

	void loadSettings();
	void retranslateUi();

private slots:
	void interfaceChanged(int);
    void changed();

signals:
	void settingsChanged();
};

#endif

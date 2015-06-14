/***************************************************************************
    File                 : CantorWorksheet.h
    Project              : LabPlot
    Description          : Aspect providing a Cantor Worksheets for Multiple backends
    --------------------------------------------------------------------
    Copyright            : (C) 2015 Garvit Khatri (garvitdelhi@gmail.com)

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

#ifndef CANTORWORKSHEET_H
#define CANTORWORKSHEET_H

#include <backend/core/AbstractPart.h>
#include <cantor/session.h>
#include <KParts/ReadWritePart>
#include "backend/core/AbstractScriptingEngine.h"

class CantorWorksheet : public AbstractPart, public scripted{
    Q_OBJECT
    
    public:
	CantorWorksheet(AbstractScriptingEngine *engine, const QString &name);
	virtual QWidget* view() const;
	virtual QMenu *createContextMenu();
	QString BackendName();
	KParts::ReadWritePart* getPart();

    signals:
	void requestProjectContextMenu(QMenu*);
    
    private:
	KParts::ReadWritePart* part;
	QString backendName;
	
	void initialize();
};

#endif // CANTORWORKSHEET_H
/***************************************************************************
File                 : AbstractFileFilter.h
Project              : LabPlot
Description          : file I/O-filter related interface
--------------------------------------------------------------------
Copyright            : (C) 2009-2013 Alexander Semke (alexander.semke@web.de)
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

#ifndef ABSTRACTFILEFILTER_H
#define ABSTRACTFILEFILTER_H

#include <QObject>

class AbstractDataSource;
class XmlStreamReader;
class QXmlStreamWriter;

class AbstractFileFilter : public QObject {
	Q_OBJECT

	public:
        AbstractFileFilter() {}
		virtual ~AbstractFileFilter() {}
		enum ImportMode {Append, Prepend, Replace};
		
		virtual void read(const QString& fileName, AbstractDataSource* dataSource, ImportMode mode = Replace) = 0;
		virtual void write(const QString& fileName, AbstractDataSource* dataSource) = 0;

		virtual void loadFilterSettings(const QString& filterName) = 0;
		virtual void saveFilterSettings(const QString& filterName) const = 0;

		virtual void save(QXmlStreamWriter*) const = 0;
		virtual bool load(XmlStreamReader*) = 0;

	signals:
		void completed(int) const; //!< int ranging from 0 to 100 notifies about the status of a read/write process		
};

#endif

/***************************************************************************
    File                 : Base3D.cpp
    Project              : LabPlot
    Description          : Base class for 3D objects
    --------------------------------------------------------------------
    Copyright            : (C) 2015 by Minh Ngo (minh@fedoraproject.org)

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

#include "Base3D.h"
#include "Base3DPrivate.h"

#include <QDebug>

#include <KIcon>

#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkAssembly.h>
#include <vtkProp3DCollection.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>

Base3D::Base3D(Base3DPrivate* priv)
	: AbstractAspect(priv->name())
	, d_ptr(priv) {
	Q_D(Base3D);
	qDebug() << Q_FUNC_INFO << d;
	if (d->renderer)
		d->init();
}

Base3D::~Base3D() {
}

QIcon Base3D::icon() const {
	return KIcon("xy-curve");
}

void Base3D::setRenderer(vtkRenderer* renderer) {
	Q_D(Base3D);
	d->renderer = renderer;
	if (renderer)
		d->init();
}

void Base3D::setXScaling(Plot3D::Scaling scaling) {
	Q_D(Base3D);
	d->xScaling = scaling;
	// TODO: Separate update method for scaling
	update();
}

void Base3D::setYScaling(Plot3D::Scaling scaling) {
	Q_D(Base3D);
	d->yScaling = scaling;
	update();
}

void Base3D::setZScaling(Plot3D::Scaling scaling) {
	Q_D(Base3D);
	d->zScaling = scaling;
	update();
}

void Base3D::highlight(bool pred) {
	Q_D(Base3D);
	if (pred && !d->isHighlighted) {
		d->isHighlighted = true;
		vtkProperty *prop = d->getProperty();
		if (!d->isSelected)
			d->property->DeepCopy(prop);
		prop->SetColor(1.0, 1.0, 0.0);
		prop->SetDiffuse(1.0);
		prop->SetSpecular(0.0);
	} else if (d->isHighlighted && !pred) {
		d->isHighlighted = false;
		d->getProperty()->DeepCopy(d->property);
		if (d->isSelected) {
			d->isSelected = false;
			select(true);
		}
	}
}

void Base3D::select(bool pred) {
	Q_D(Base3D);
	if (pred && !d->isSelected) {
		if (!d->actor)
			return;
		d->isSelected = true;
		vtkProperty *prop = d->getProperty();
		if (!d->isHighlighted)
			d->property->DeepCopy(prop);
		prop->SetColor(1.0, 0.0, 0.0);
		prop->SetDiffuse(1.0);
		prop->SetSpecular(0.0);
	} else if (d->isSelected && !pred) {
		d->isSelected = false;
		d->isHighlighted = false;
		d->getProperty()->DeepCopy(d->property);
	}
}

bool Base3D::operator==(vtkProp* prop) const {
	Q_D(const Base3D);
	return dynamic_cast<vtkProp*>(d->actor.Get()) == prop;
}

bool Base3D::operator!=(vtkProp* prop) const {
	return !operator==(prop);
}

void Base3D::show(bool pred) {
	Q_D(Base3D);
	if (d->actor) {
		d->actor->SetVisibility(pred);
		emit parametersChanged();
	}
}

void Base3D::reset() {
	Q_D(Base3D);
	if (!d->renderer->HasViewProp(d->actor)) {
		d->renderer->AddActor(d->actor);
	}
}

bool Base3D::isVisible() const {
	Q_D(const Base3D);
	if (!d->actor)
		return false;
	return d->actor->GetVisibility() != 0;
}

void Base3D::update() {
	Q_D(Base3D);
	d->update();
}

void Base3D::remove() {
	Q_D(Base3D);
	d->hide();
	emit removed();
	AbstractAspect::remove();
}


////////////////////////////////////////////////////////////////////////////////

Base3DPrivate::Base3DPrivate(const QString& name, Base3D* baseParent)
	: baseParent(baseParent)
	, isHighlighted(false)
	, isSelected(false)
	, renderer(0)
	, property(vtkProperty::New())
	, objName(name) {
}

const QString& Base3DPrivate::name() const {
	return objName;
}

Base3DPrivate::~Base3DPrivate() {
}

void Base3DPrivate::init() {
	update();
}

vtkProperty* Base3DPrivate::getProperty() const {
	return actor.Get()->GetProperty();
}

void Base3DPrivate::update() {
	if (!renderer)
		return;

	hide();

	createActor();

	if (actor) {
		property->DeepCopy(getProperty());
		renderer->AddActor(actor);
		emit baseParent->parametersChanged();
		emit baseParent->visibilityChanged(true);
	}
}

void Base3DPrivate::hide() {
	if (actor && renderer) {
		baseParent->select(false);
		baseParent->highlight(false);
		renderer->RemoveActor(actor);
	}
}

void Base3DPrivate::scale(vtkPolyData* data) {
	const Plot3D::Scaling scaling[3] = {xScaling, yScaling, zScaling};

	for (int i = 0; i < 3; ++i) {
		if (scaling[i] == Plot3D::Scaling_Ln)
			scale(data, i, log);
		else if (scaling[i] == Plot3D::Scaling_Log10)
			scale(data, i, log10);
		else if (scaling[i] == Plot3D::Scaling_Log2)
			scale(data, i, log2);
	}
}

void Base3DPrivate::scale(vtkPolyData* data, int id, double (*scaleFunction)(double value)) {
	vtkPoints* points = data->GetPoints();
	double point[3];
	for (vtkIdType i = 0; i < points->GetNumberOfPoints(); ++i) {
		points->GetPoint(i, point);
		point[id] = scaleFunction(point[id]);
		points->SetPoint(i, point);
	}
}
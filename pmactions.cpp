/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2006 by Andreas Zehender
    email                : zehender@kde.org
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/

#include "pmactions.h"

#include <QSpinBox>
#include <QToolBar>


class PMSpinBoxAction::PMSpinBoxActionPrivate
{
public:
	PMSpinBoxActionPrivate()
	{
		m_value = 0;
	}

	~PMSpinBoxActionPrivate()
	{
	}

	QList<QSpinBox*> m_spinBoxes;
	int m_value;
};

PMSpinBoxAction::PMSpinBoxAction( QObject * parent )
		: QAction(parent)
		, d(new PMSpinBoxActionPrivate())
{
	init();
}

PMSpinBoxAction::PMSpinBoxAction( const QString & text, QObject * parent )
		: QAction(text, parent)
		, d(new PMSpinBoxActionPrivate())
{
	init();
}

PMSpinBoxAction::PMSpinBoxAction(const QIcon &icon, const QString & text, QObject * parent )
		: QAction(icon, text, parent)
		, d(new PMSpinBoxActionPrivate())
{
	init();
}

PMSpinBoxAction::~PMSpinBoxAction()
{
	delete d;
}

void PMSpinBoxAction::init()
{
}


void PMSpinBoxAction::setCurrentItem( int index )
{
	QList<QSpinBox*>::iterator it;
	for(it = d->m_spinBoxes.begin(); it != d->m_spinBoxes.end(); ++it){
		bool sb = (*it)->signalsBlocked();
		(*it)->blockSignals(true);
		(*it)->setValue(index);
		(*it)->blockSignals(sb);
	}
	d->m_value = index;
}

void PMSpinBoxAction::valueChanged(int value)
{
	setCurrentItem(value);
	emit triggered(value);
}

void PMSpinBoxAction::spinBoxDeleted(QObject* object)
{
	foreach (QSpinBox* spinBox, d->m_spinBoxes)
		if (object == spinBox) {
			d->m_spinBoxes.removeAll(static_cast<QSpinBox*>(object));
			break;
		}
}

QWidget * PMSpinBoxAction::createWidget( QWidget * parent )
{
	QSpinBox* spinBox = new QSpinBox(parent);
	spinBox->setValue(d->m_value);

	connect(spinBox, SIGNAL(destroyed(QObject*)), SLOT(spinBoxDeleted(QObject*)));
	connect(spinBox, SIGNAL(valueChanged(int)), SLOT(valueChanged(int)));
	d->m_spinBoxes.append(spinBox);

	return spinBox;
}

#include "pmactions.moc"

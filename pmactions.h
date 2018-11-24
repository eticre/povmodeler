//-*-C++-*-
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

#ifndef PMSPINBOXACTION_H
#define PMSPINBOXACTION_H

#include <QToolButton>

#include <QAction>

/**
 *  @short Action with spin box
 */
class PMSpinBoxAction : public QAction
{
	Q_OBJECT
public:
	/**
	 * Constructs an action with text; a shortcut may be specified by
	 * the ampersand character (e.g. "&amp;Option" creates a shortcut with key \e O )
	 *
	 * This is the most common QAction used when you do not have a
	 * corresponding icon (note that it won't appear in the current version
	 * of the "Edit ToolBar" dialog, because an action needs an icon to be
	 * plugged in a toolbar...).
	 *
	 * @param parent The action collection to contain this action.
	 */
	PMSpinBoxAction(QObject* parent);

        /**
	 * Constructs an action with text; a shortcut may be specified by
	 * the ampersand character (e.g. "&amp;Option" creates a shortcut with key \e O )
	 *
	 * This is the most common QAction used when you do not have a
	 * corresponding icon (note that it won't appear in the current version
	 * of the "Edit ToolBar" dialog, because an action needs an icon to be
	 * plugged in a toolbar...).
	 *
	 * @param text The text that will be displayed.
	 * @param parent The action collection to contain this action.
	 */
	PMSpinBoxAction(const QString& text, QObject* parent);

	/**
	 * Constructs an action with text and an icon; a shortcut may be specified by
	 * the ampersand character (e.g. "&amp;Option" creates a shortcut with key \e O )
	 *
	 * This is the other common QAction used.  Use it when you
	 * \e do have a corresponding icon.
	 *
	 * @param icon The icon to display.
	 * @param text The text that will be displayed.
	 * @param parent The action collection to contain this action.
	 */
    PMSpinBoxAction(const QIcon& icon, const QString& text, QObject* parent);

	/**
	 * Destructor
	 */
	virtual ~PMSpinBoxAction();

	/**
	 * Returns the current spinbox value.
	 */
	int currentItem() const;

	/**
	 * Sets the spinbox value
	 */
	void setCurrentItem(int index);

	virtual QWidget* createWidget(QWidget* parent);

Q_SIGNALS:
	/**
	 * This signal is emitted when an item is selected; @param index indicates
	 * the item selected.
	 */
	void triggered( int index );

protected Q_SLOTS:
   virtual void valueChanged(int);

private Q_SLOTS:
	void spinBoxDeleted(QObject* object);

private:
	void init();

	class PMSpinBoxActionPrivate;
	PMSpinBoxActionPrivate* const d;
};

#endif

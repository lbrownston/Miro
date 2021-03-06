// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2013
// Department of Neural Information Processing, University of Ulm
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
#define QT_ALTERNATE_QTSMANIP

// This module
#include "DocumentXML.h"
// This application
#include "miro/Exception.h"
// The Qt library
#include <qfile.h>
#include <qobject.h>
#include <QTextStream>
#include <QTreeWidget>
// The C++ Standard Library
#include <cassert>

//------------------------------------------------------------------------------
// public methods
//------------------------------------------------------------------------------

DocumentXML::DocumentXML(QDomDocument const& _document,
			 QTreeWidget * _treeWidget, 
			 QObject * _parent,
			 const char * _name) :
  Super(_document, _treeWidget, NULL, _parent, ""),
  document_(_document)
{}

DocumentXML::~DocumentXML()
{}

void
DocumentXML::init(QString const& _rootTag)
{
  clear();
  setName("");

  // init default document
  QDomElement root = document_.createElement(_rootTag);
  QDomNode n = document_.appendChild( root );
  assert(!n.isNull());

  // Label the root TreeWidgetItem
  treeWidgetItem()->setText(0, _rootTag);
  // Write the root node's type
  treeWidgetItem()->setText(2, document_.doctype().name());
  // Display the root node expanded
  treeWidgetItem()->setExpanded(true);

  const bool modified = false;
  const bool recurse =  true;
  setModified(modified, recurse);
}

void
DocumentXML::initXML(QString const& _xml)
{
  clear();
  setName("");

  // If a parse error occurred, the parser's error message
  QString error;
  // If a parse error occurred, the line number of the error
  int line;
  // If a parse error occurred, the column number of the error
  int column;
  if (!document_.setContent(_xml, &error, &line, &column)) {
    QString l, c;
    l.setNum(line);
    c.setNum(column);
    throw Miro::Exception(QString("XML parsing error!\n" + error + "in line " + l + ", column " + c));
  }

  const bool modified = false;
  const bool recurse = true;
  setModified(modified, recurse);
}

void
DocumentXML::clear()
{
  // Remove the root element
  document_.removeChild(document_.documentElement());
  // Notify that the document has been modified
  setModified(true);

  // Delete all children of the QObject
  if(!children().isEmpty()) {
    QObjectList childList = children();
    while(!childList.isEmpty()) {
      delete childList.takeFirst();
    }
  }
}

//------------------------------------------------------------------------------
// document methods

void
DocumentXML::setName(const QString& _name)
{
  if (name() != _name) {
    Super::setName(_name);
    setModified(true);
  }
}

void 
DocumentXML::loadXML(const QString& _name) 
{
  clear();
  setModified(false, true);

  QFile f(_name);
  if (!f.open(QIODevice::ReadOnly)) {
    throw Miro::Exception(QString("DocumentXML::loadXML: Error: file not found!"));
  }
  if (!document_.setContent(&f)) {
    throw Miro::Exception(QString("DocumentXML::loadXML: Error: XML parsing error!"));
  }
  Super::setName(_name);
}


void
DocumentXML::saveXML()
{
  QFile f(name() );
  if (!f.open(QIODevice::WriteOnly)) {
    throw Miro::Exception(QString("DocumentXML::loadXML: Error: file error!"));
  }
  QTextStream ts(&f);
  document_.save(ts, 2);
  setModified(false, true);
}

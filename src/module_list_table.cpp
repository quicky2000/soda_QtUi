/*
  This file is part of soda_QtUi, Graphical User Interface
  of SODA ( Smart Openstreetmap Diff Analyzer )
  Copyright (C) 2013  Julien Thevenon ( julien_thevenon at yahoo.fr )

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#include "module_list_table.h"
#include "simple_table_cell.h"
#include <cassert>

namespace soda_QtUi
{
  //----------------------------------------------------------------------------
  module_list_table::module_list_table(QWidget *parent):
    QTableWidget(0,3,parent)
  {
    QStringList l_h_header_list;
    l_h_header_list << tr("Name") << tr("Type")<< tr("Enabled") ;
    setHorizontalHeaderLabels(l_h_header_list);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setItemPrototype(new simple_table_cell(""));
  }

  //----------------------------------------------------------------------------
  void module_list_table::add_module(const std::string & p_name,
				     const std::string & p_type,
				     bool p_enabled)
  {
    setRowCount(rowCount()+1);
    update_row(rowCount()-1,p_name,p_type,p_enabled);
  }
  //----------------------------------------------------------------------------
  void module_list_table::update_row(unsigned int p_row,
				     const std::string & p_name,
				     const std::string & p_type,
				     bool p_enabled)
  {
    assert(p_row >= 0 && p_row < (unsigned int)rowCount());
    setItem(p_row,0,new simple_table_cell(QString(p_name.c_str())));
    setItem(p_row,1,new simple_table_cell(QString(p_type.c_str())));
    setItem(p_row,2,new simple_table_cell(QString((p_enabled ? tr("Yes"):tr("No")))));
    resizeColumnsToContents();
  }

}
//EOF

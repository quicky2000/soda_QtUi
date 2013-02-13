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
#include "module_parameter_list_table.h"
#include "simple_table_cell.h"
#include <cassert>
#include <iostream>

namespace soda_QtUi
{
  //------------------------------------------------------------------------
  module_parameter_list_table::module_parameter_list_table(QWidget *parent):
    QTableWidget(0,2,parent)
  {
    QStringList l_h_header_list;
    l_h_header_list << tr("Name") << tr("Value") ;
    setHorizontalHeaderLabels(l_h_header_list);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setItemPrototype(new simple_table_cell(""));
  }

  //------------------------------------------------------------------------
  void module_parameter_list_table::add_parameter(const std::string & p_name,
						  const std::string & p_value)
  {
    setRowCount(rowCount()+1);
    update_row(rowCount()-1,p_name,p_value);
  }
  //------------------------------------------------------------------------
  void module_parameter_list_table::update_row(unsigned int p_row,
					       const std::string & p_name,
					       const std::string & p_value)
  {
    assert(p_row >= 0 && p_row < (unsigned int)rowCount());
    setItem(p_row,0,new simple_table_cell(QString(p_name.c_str())));
    setItem(p_row,1,new simple_table_cell(QString(p_value.c_str())));
    resizeColumnsToContents();
  }

}

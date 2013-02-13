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
#include "domain_jump_table.h"
#include "simple_table_cell.h"

namespace soda_QtUi
{
  //----------------------------------------------------------------------------
  domain_jump_table::domain_jump_table(QWidget *parent):
    QTableWidget(0,4,parent)
  {
    QStringList l_h_header_list;
    l_h_header_list << tr("Old Domain") << tr("Old Sequence Number") << tr("Start Domain") << tr("Start Sequence Number") ;
    setHorizontalHeaderLabels(l_h_header_list);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setItemPrototype(new simple_table_cell(""));
  }

  //----------------------------------------------------------------------------
  void domain_jump_table::add(const std::string & p_start_domain,
			      const std::string & p_start_sequence_number,
			      const std::string & p_old_domain,
			      const std::string & p_old_sequence_number)
  {
    setRowCount(rowCount()+1);
    setItem(rowCount()-1,0,new simple_table_cell(QString(p_start_domain.c_str())));
    setItem(rowCount()-1,1,new simple_table_cell(QString(p_start_sequence_number.c_str())));
    setItem(rowCount()-1,2,new simple_table_cell(QString(p_old_domain.c_str())));
    setItem(rowCount()-1,3,new simple_table_cell(QString(p_old_sequence_number.c_str())));
    resizeColumnsToContents();
  }

  //----------------------------------------------------------------------------
  bool domain_jump_table::contains(const std::string & p_start_domain,
				   const std::string & p_start_sequence_number,
				   const std::string & p_old_domain,
				   const std::string & p_old_sequence_number)
  {
    bool l_found = false;
    for(int l_index = 0 ; l_index < rowCount() && ! l_found; ++l_index)
      {
	l_found = p_start_domain == item(l_index,0)->text().toStdString() && 
	  p_start_sequence_number == item(l_index,1)->text().toStdString() &&
	  p_old_domain == item(l_index,2)->text().toStdString() &&  
	  p_old_sequence_number == item(l_index,3)->text().toStdString();
      }
    return l_found;
  }


}
//EOF

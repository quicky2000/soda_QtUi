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
#ifndef DOMAIN_JUMP_TABLE_H
#define DOMAIN_JUMP_TABLE_H

#include <QTableWidget>

namespace soda_QtUi
{
    class domain_jump_table : public QTableWidget
    {
    public:
        domain_jump_table(QWidget *parent=NULL);
        void add(const std::string & p_start_domain,
                 const std::string & p_start_sequence_number,
                 const std::string & p_old_domain,
                 const std::string & p_old_sequence_number);
	bool contains(const std::string & p_start_domain,
		      const std::string & p_start_sequence_number,
		      const std::string & p_old_domain,
		      const std::string & p_old_sequence_number);

    private:
    };
}
#endif // DOMAIN_JUMP_TABLE_H

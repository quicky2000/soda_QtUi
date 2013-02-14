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
#ifndef DOMAIN_JUMP_CONFIGURATION_WIDGET_H
#define DOMAIN_JUMP_CONFIGURATION_WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

#include "domain_jump_table.h"
#include "replication_domain_jump.h"

namespace soda_QtUi
{

    class domain_jump_configuration_widget : public QWidget
    {
        Q_OBJECT
    public:
        domain_jump_configuration_widget(QWidget * parent = NULL);
        void clear(void);
        void add(const osm_diff_watcher::replication_domain_jump & p_domain);
        void get_domain_jumps(std::vector<osm_diff_watcher::replication_domain_jump> & p_domain_jumps);
    signals:
    private slots:
        void treat_field_modification_event();
	void treat_domain_selected_event(int p_row);
	void treat_domain_selection_changed_event();
	void treat_add_event();
	void treat_remove_event();
    private:
	void clean_fields(void);
        QLineEdit * m_start_domain;
        QLineEdit * m_start_seq_number;
        QLineEdit * m_old_domain;
        QLineEdit * m_old_seq_number;
        domain_jump_table *m_domain_jump_table;
        QPushButton * m_add_button;
        QPushButton * m_remove_button;
    };
}
#endif // DOMAIN_JUMP_CONFIGURATION_WIDGET_H

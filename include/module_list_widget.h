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
#ifndef MODULE_LIST_WIDGET_H
#define MODULE_LIST_WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include "module_list_table.h"
#include <map>

#include "module_configuration.h"

namespace soda_QtUi
{
    class module_list_widget: public QWidget
    {
        Q_OBJECT
    public:
        module_list_widget(QWidget * parent=NULL);
        osm_diff_analyzer_if::module_configuration & get_selection_configuration(void);
        void add(const osm_diff_analyzer_if::module_configuration & p_conf);
        void clear(void);
        const std::map<std::string,osm_diff_analyzer_if::module_configuration> & get_modules(void);
    signals:
        void no_more_selection(void);
        void module_selected(void);
    private slots:
        // Module list slots
        void treat_module_selected_event(int);
        void treat_module_selection_changed_event();
        void treat_module_name_modification_event();
        void treat_module_type_modification_event();
        void treat_module_enabled_state_changed_event();
        void treat_add_module_event();
        void treat_modify_module_event();
        void treat_remove_module_event();
    private:
        void treat_module_field_change_event(void);
        void clean_module_fields(void);

        QLineEdit * m_module_name_field;
        QLineEdit * m_module_type_field;
        QCheckBox * m_module_enabled_field;
        module_list_table * m_module_list_table;
        QPushButton * m_add_module_button;
        QPushButton * m_modify_module_button;
        QPushButton * m_remove_module_button;

        std::map<std::string,osm_diff_analyzer_if::module_configuration> m_module_configurations;
    };
}
#endif // MODULE_LIST_WIDGET_H

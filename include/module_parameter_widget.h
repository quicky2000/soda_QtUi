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
#ifndef MODULE_PARAMETER_WIDGET_H
#define MODULE_PARAMETER_WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "module_parameter_list_table.h"

#include <map>

namespace soda_QtUi
{
    class module_parameter_widget: public QWidget
    {
        Q_OBJECT
    public:
        module_parameter_widget(QWidget * parent = NULL);
        void display_module_parameters(const std::map<std::string,std::string> & p_parameters);
        void setEnabled(bool p_enabled);
    signals:
        void parameter_created(const QString & p_name,const QString & p_value);
        void parameter_modified(const QString & p_name,const QString & p_value);
        void parameter_removed(const QString & p_name);
    private slots:
        // Module parameter slots
        void treat_module_parameter_name_modification_event();
        void treat_module_parameter_value_modification_event();
        void treat_module_parameter_selected_event(int);
        void treat_module_parameter_selection_changed_event();
        void treat_add_module_parameter_event();
        void treat_modify_module_parameter_event();
        void treat_remove_module_parameter_event();
    private:
        void treat_module_parameter_field_change_event(void);

        QLineEdit * m_module_parameter_name_field;
        QLineEdit * m_module_parameter_value_field;
        module_parameter_list_table * m_module_parameter_list_table;
        QPushButton * m_add_module_parameter_button;
        QPushButton * m_modify_module_parameter_button;
        QPushButton * m_remove_module_parameter_button;

        const std::map<std::string,std::string> * m_parameters;
    };
}
#endif // MODULE_PARAMETER_WIDGET_H

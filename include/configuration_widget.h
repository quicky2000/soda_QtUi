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
#ifndef _CONFIGURATION_WIDGET_H_
#define _CONFIGURATION_WIDGET_H_

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <vector>
#include <string>
#include "library_list_table.h"
#include "module_parameter_widget.h"
#include "module_list_widget.h"
#include "domain_jump_configuration_widget.h"

namespace soda_QtUi
{
  class configuration_widget: public QWidget
  {
    Q_OBJECT
      public:
    configuration_widget(QWidget * p_parent=NULL);

    private slots:
    // Proxy slots
    void treat_proxy_conf_box_state_changed_event();
    // Start policy slots
    void treat_start_policy_selection_event();
    // Module libraries slots
    void treat_add_library_event();
    void treat_remove_library_event();
    void treat_library_selected_event(int);
    void treat_library_selection_changed_event();
    // Module list slot
    void treat_no_more_module_selection_event();
    void treat_module_selected_event();
    // Module parameters slot
    void treat_module_parameter_created_event(const QString & p_name,const QString & p_value);
    void treat_module_parameter_modified_event(const QString & p_name,const QString & p_value);
    void treat_module_parameter_removed_event(const QString & p_name);
  private:

    // Start configuration
    QComboBox * m_start_policy_field;
    QLineEdit * m_iteration_number_field;
    QLineEdit * m_start_sequence_number_field;
    QLineEdit * m_replication_domain_field;
    QLabel * m_start_sequence_number_label;
    QLabel * m_replication_domain_label;

    std::vector<std::string> m_start_policies;

    static const unsigned int m_start_policy_current_index;
    static const unsigned int m_start_policy_stored_index;
    static const unsigned int m_start_policy_user_defined_index;

    // Module libraries configuration
    library_list_table * m_library_list_table;
    QPushButton * m_add_library_button;
    QPushButton * m_remove_library_button;
    int m_selected_row;

    // Module configuration
    module_list_widget * m_module_list_widget;
    module_parameter_widget * m_module_parameter_widget;

    // Domain jump configuration
    domain_jump_configuration_widget * m_domain_jump_configuration_widget;

    // Proxy configuration
    QCheckBox * m_proxy_conf_box;
    QLabel * m_proxy_name_label; 
    QLineEdit * m_proxy_name_field; 
    QLineEdit * m_proxy_port_field; 
    QLabel * m_proxy_port_label; 
    QLineEdit * m_proxy_login_field; 
    QLabel * m_proxy_login_label; 
    QLabel * m_proxy_password_label; 
    QLineEdit * m_proxy_password_field; 
  };
}
#endif // _CONFIGURATION_WIDGET_H_
//EOF

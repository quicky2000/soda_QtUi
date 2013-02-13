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
#include "configuration_widget.h"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <iostream>
#include <cassert>
#include <fstream>

namespace soda_QtUi
{
  //----------------------------------------------------------------------------
  configuration_widget::configuration_widget(QWidget * p_parent):
    QWidget(p_parent),
      m_selected_row(-1)
  {
    QVBoxLayout * l_vertical_layout = new QVBoxLayout(this);

    QGroupBox * l_start_configuration = new QGroupBox(tr("Start configuration"));
    l_vertical_layout->addWidget(l_start_configuration);

    QVBoxLayout * l_start_layout = new QVBoxLayout(l_start_configuration);

    QHBoxLayout *l_start_horizontal_layout = new QHBoxLayout();
    l_start_layout->addLayout(l_start_horizontal_layout);

    l_start_horizontal_layout->addWidget(new QLabel(tr("start_policy")+" :"));
    m_start_policy_field = new QComboBox();
    m_start_policy_field->addItem(tr("current"));
    m_start_policy_field->addItem(tr("stored"));
    m_start_policy_field->addItem(tr("user_defined"));
    m_start_policy_field->setCurrentIndex(0);
    m_start_policies.push_back("current");
    m_start_policies.push_back("stored");
    m_start_policies.push_back("user_defined");
    m_start_policy_field->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    connect(m_start_policy_field,SIGNAL(activated (int)),this, SLOT(treat_start_policy_selection_event()));
    l_start_horizontal_layout->addWidget(m_start_policy_field);

    l_start_horizontal_layout->addWidget(new QLabel(tr("iteration_number")+" :"));
    m_iteration_number_field = new QLineEdit("");
    m_iteration_number_field->setInputMask("000000000000");
    l_start_horizontal_layout->addWidget(m_iteration_number_field);

    m_start_sequence_number_label = new QLabel(tr("start_sequence_number")+" :");
    l_start_horizontal_layout->addWidget(m_start_sequence_number_label);
    m_start_sequence_number_field = new QLineEdit("");
    m_start_sequence_number_field->setInputMask("000000000000");
    l_start_horizontal_layout->addWidget(m_start_sequence_number_field);

    m_replication_domain_label = new QLabel(tr("replication_domain")+" :");
    l_start_layout->addWidget(m_replication_domain_label);
    m_replication_domain_field = new QLineEdit("");
    l_start_layout->addWidget(m_replication_domain_field);


    // Module library configuration
    QGroupBox * l_module_libraries_configuration = new QGroupBox(tr("Module Libraries"));
    l_vertical_layout->addWidget(l_module_libraries_configuration);

    QVBoxLayout * l_module_libraries_layout = new QVBoxLayout(l_module_libraries_configuration);

    m_library_list_table = new library_list_table();
    l_module_libraries_layout->addWidget(m_library_list_table);
    connect(m_library_list_table,SIGNAL(cellClicked (int, int)),this, SLOT(treat_library_selected_event(int)));
    connect(m_library_list_table,SIGNAL(itemSelectionChanged()),this, SLOT(treat_library_selection_changed_event()));

    QHBoxLayout * l_library_button_layout = new QHBoxLayout();
    l_module_libraries_layout->addLayout(l_library_button_layout);

    m_add_library_button = new QPushButton(tr("Add"));
    l_library_button_layout->addWidget(m_add_library_button);
    connect(m_add_library_button,SIGNAL(clicked()),this,SLOT(treat_add_library_event()));

    m_remove_library_button = new QPushButton(tr("Remove"));
    l_library_button_layout->addWidget(m_remove_library_button);
    connect(m_remove_library_button,SIGNAL(clicked()),this,SLOT(treat_remove_library_event()));
    m_remove_library_button->setEnabled(false);

    // Modules configuration
    QGroupBox * l_module_configuration = new QGroupBox(tr("Module configuration"));
    l_vertical_layout->addWidget(l_module_configuration);

    QHBoxLayout * l_module_configuration_layout = new QHBoxLayout(l_module_configuration);

    m_module_list_widget = new module_list_widget();
    connect(m_module_list_widget,SIGNAL(no_more_selection()),this,SLOT(treat_no_more_module_selection_event()));
    connect(m_module_list_widget,SIGNAL(module_selected()),this,SLOT(treat_module_selected_event()));
    l_module_configuration_layout->addWidget(m_module_list_widget);
    
    // Module parameters
    m_module_parameter_widget = new module_parameter_widget();
    m_module_parameter_widget->setEnabled(false);
    connect(m_module_parameter_widget,SIGNAL(parameter_created(QString,QString)),this,SLOT(treat_module_parameter_created_event(QString,QString)));
    connect(m_module_parameter_widget,SIGNAL(parameter_modified(QString,QString)),this,SLOT(treat_module_parameter_modified_event(QString,QString)));
    connect(m_module_parameter_widget,SIGNAL(parameter_removed(QString)),this,SLOT(treat_module_parameter_removed_event(QString)));
    l_module_configuration_layout->addWidget(m_module_parameter_widget);

    // Domain jump configuration
    QGroupBox * l_domain_jump_configuration = new QGroupBox(tr("Domain Jump configuration"));
    l_vertical_layout->addWidget(l_domain_jump_configuration);

    QVBoxLayout * l_domain_jump_layout = new QVBoxLayout(l_domain_jump_configuration);

    m_domain_jump_configuration_widget = new domain_jump_configuration_widget();
    l_domain_jump_layout->addWidget(m_domain_jump_configuration_widget);

    // Proxy configuration
    QGroupBox * l_proxy_configuration = new QGroupBox(tr("Proxy configuration"));
    l_vertical_layout->addWidget(l_proxy_configuration);

    QVBoxLayout * l_proxy_layout = new QVBoxLayout(l_proxy_configuration);

    m_proxy_conf_box = new QCheckBox(tr("Behind a proxy"));
    connect(m_proxy_conf_box,SIGNAL(stateChanged(int)),this, SLOT(treat_proxy_conf_box_state_changed_event()));
    l_proxy_layout->addWidget(m_proxy_conf_box);

    QHBoxLayout * l_proxy_table_layout = new QHBoxLayout();
    l_proxy_layout->addLayout(l_proxy_table_layout);
    QVBoxLayout * l_proxy_left_field_layout = new QVBoxLayout();
    l_proxy_table_layout->addLayout(l_proxy_left_field_layout);
    QVBoxLayout * l_proxy_right_field_layout = new QVBoxLayout();
    l_proxy_table_layout->addLayout(l_proxy_right_field_layout);

    QHBoxLayout * l_proxy_name_layout = new QHBoxLayout();
    l_proxy_left_field_layout->addLayout(l_proxy_name_layout);
    m_proxy_name_label = new QLabel(tr("proxy_authentication.proxy_name")+" :");
    l_proxy_name_layout->addWidget(m_proxy_name_label);
    m_proxy_name_field = new QLineEdit("");
    l_proxy_name_layout->addWidget(m_proxy_name_field);

    QHBoxLayout * l_proxy_port_layout = new QHBoxLayout();
    l_proxy_right_field_layout->addLayout(l_proxy_port_layout);
    m_proxy_port_label = new QLabel(tr("proxy_authentication.proxy_port")+" :");
    l_proxy_port_layout->addWidget(m_proxy_port_label);
    m_proxy_port_field = new QLineEdit("");
    m_proxy_port_field->setInputMask("00000");
    l_proxy_port_layout->addWidget(m_proxy_port_field);


    QHBoxLayout * l_proxy_login_layout = new QHBoxLayout();
    l_proxy_left_field_layout->addLayout(l_proxy_login_layout);
    m_proxy_login_label = new QLabel(tr("proxy_authentication.proxy_login")+" :");
    l_proxy_login_layout->addWidget(m_proxy_login_label);
    m_proxy_login_field = new QLineEdit("");
    l_proxy_login_layout->addWidget(m_proxy_login_field);

    QHBoxLayout * l_proxy_password_layout = new QHBoxLayout();
    l_proxy_right_field_layout->addLayout(l_proxy_password_layout);
    m_proxy_password_label = new QLabel(tr("proxy_authentication.proxy_password")+" :");
    l_proxy_password_layout->addWidget(m_proxy_password_label);
    m_proxy_password_field = new QLineEdit("");
    l_proxy_password_layout->addWidget(m_proxy_password_field);

    treat_start_policy_selection_event();
    treat_proxy_conf_box_state_changed_event();
  }

  //----------------------------------------------------------------------------
  void configuration_widget::treat_proxy_conf_box_state_changed_event()
  {
    std::cout << "configuration_widget::received proxy_conf_box_state_changed Event" << std::endl ;
    m_proxy_name_label->setVisible(m_proxy_conf_box->isChecked());
    m_proxy_name_field->setVisible(m_proxy_conf_box->isChecked());
    m_proxy_port_label->setVisible(m_proxy_conf_box->isChecked());
    m_proxy_port_field->setVisible(m_proxy_conf_box->isChecked());
    m_proxy_login_label->setVisible(m_proxy_conf_box->isChecked());
    m_proxy_login_field->setVisible(m_proxy_conf_box->isChecked());
    m_proxy_password_label->setVisible(m_proxy_conf_box->isChecked());
    m_proxy_password_field->setVisible(m_proxy_conf_box->isChecked());
  }

  //----------------------------------------------------------------------------
  void configuration_widget::treat_start_policy_selection_event()
  {
    std::cout << "configuration_widget::received start_policy_selection_event" << std::endl ;
    std::cout << "Selected index : " << m_start_policy_field->currentIndex() << std::endl ;
    assert((unsigned int)m_start_policy_field->currentIndex() < m_start_policies.size());
    std::cout << "Selected \"" << m_start_policies[m_start_policy_field->currentIndex()] << "\"" << std::endl ;
    
    if((unsigned int)m_start_policy_field->currentIndex() == m_start_policy_stored_index)
      {
	m_start_sequence_number_label->setVisible(false);
	m_start_sequence_number_field->setVisible(false);
	m_replication_domain_label->setVisible(false);
	m_replication_domain_field->setVisible(false);
      }
    else if((unsigned int)m_start_policy_field->currentIndex() == m_start_policy_current_index)
      {
	m_start_sequence_number_label->setVisible(false);
	m_start_sequence_number_field->setVisible(false);
	m_replication_domain_label->setVisible(true);
	m_replication_domain_field->setVisible(true);
      }
    else
      {
	m_start_sequence_number_label->setVisible(true);
	m_start_sequence_number_field->setVisible(true);
	m_replication_domain_label->setVisible(true);
	m_replication_domain_field->setVisible(true);
      }
  }

  //----------------------------------------------------------------------------
  void configuration_widget::treat_add_library_event()
  {
    std::cout << "configuration_widget::received add_library_event" << std::endl ;
    QString l_file_name = QFileDialog::getOpenFileName(this,
                                                       tr("Add module library"),
#ifdef _WIN32
						       "*",
                                                       tr("Library files (*.so,*.dll)")
#else
						       "",
                                                       tr("*")
#endif
);
    std::string l_file_name_std = l_file_name.toStdString();
    if(l_file_name_std != "")
      {
	std::ifstream l_module_file;
	l_module_file.open(l_file_name_std.c_str());
	if(l_module_file.is_open())
	  {
	    m_library_list_table->add_library(l_file_name_std);
	  }
      }
  }

  //----------------------------------------------------------------------------
  void configuration_widget::treat_remove_library_event()
  {
    std::cout << "configuration_widget::received remove_library_event" << std::endl ;
    assert(!m_library_list_table->selectedItems().isEmpty());
    if(m_selected_row >= 0 && m_selected_row < m_library_list_table->rowCount())
    {
        m_library_list_table->removeRow(m_selected_row);
    }
  }

  //----------------------------------------------------------------------------
  void configuration_widget::treat_library_selected_event(int p_row)
  {
    std::cout << "configuration_widget::received library_selected_event " << p_row << std::endl ;
    m_selected_row = p_row;
  }

  //----------------------------------------------------------------------------
  void configuration_widget::treat_library_selection_changed_event()
  {
    std::cout << "configuration_widget::received library_selection_changed_event" << std::endl ;
    m_remove_library_button->setEnabled(!m_library_list_table->selectedItems().isEmpty());
  }

  //----------------------------------------------------------------------------
  void configuration_widget::treat_module_parameter_created_event(const QString & p_name,const QString & p_value)
  {
      std::cout << "configuration_widget::received module_parameter_created_event" << std::endl ;

      osm_diff_analyzer_if::module_configuration & l_module_config = m_module_list_widget->get_selection_configuration();
      l_module_config.add_parameter(p_name.toStdString(),p_value.toStdString());
      m_module_parameter_widget->display_module_parameters(l_module_config.get_parameters());
  }

  //----------------------------------------------------------------------------
  void configuration_widget::treat_module_parameter_modified_event(const QString & p_name,const QString & p_value)
  {
      std::cout << "configuration_widget::received module_parameter_modified_event" << std::endl ;
      osm_diff_analyzer_if::module_configuration & l_module_config = m_module_list_widget->get_selection_configuration();
      l_module_config.remove_parameter(p_name.toStdString());
      l_module_config.add_parameter(p_name.toStdString(),p_value.toStdString());
      m_module_parameter_widget->display_module_parameters(l_module_config.get_parameters());
  }

  //----------------------------------------------------------------------------
  void configuration_widget::treat_module_parameter_removed_event(const QString & p_name)
  {
      std::cout << "configuration_widget::received module_parameter_removed_event " << p_name.toStdString() << std::endl ;
      osm_diff_analyzer_if::module_configuration & l_module_config = m_module_list_widget->get_selection_configuration();
      l_module_config.remove_parameter(p_name.toStdString());
      m_module_parameter_widget->display_module_parameters(l_module_config.get_parameters());
  }

  //----------------------------------------------------------------------------
  void configuration_widget::treat_no_more_module_selection_event()
  {
      std::cout << "configuration_widget::received no_more_module_selection_event" << std::endl ;
      m_module_parameter_widget->setEnabled(false);
  }

  //----------------------------------------------------------------------------
  void configuration_widget::treat_module_selected_event()
  {
      std::cout << "configuration_widget::received treat_module_selected_event" << std::endl ;
      osm_diff_analyzer_if::module_configuration & l_module_config = m_module_list_widget->get_selection_configuration();
      m_module_parameter_widget->setEnabled(true);
      m_module_parameter_widget->display_module_parameters(l_module_config.get_parameters());

  }

  const unsigned int configuration_widget::m_start_policy_current_index = 0;
  const unsigned int configuration_widget::m_start_policy_stored_index = 1;
  const unsigned int configuration_widget::m_start_policy_user_defined_index = 2;


}
//EOF

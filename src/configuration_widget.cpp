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
#include "configuration_parser.h"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QTabWidget>
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

    QTabWidget * l_tab_widget = new QTabWidget();
    l_vertical_layout->addWidget(l_tab_widget);

    QWidget * l_general_widget = new QWidget();
    l_tab_widget->addTab(l_general_widget,tr("General"));

    QFormLayout * l_general_layout = new QFormLayout(l_general_widget);

    m_start_policy_field = new QComboBox();
    m_start_policy_field->addItem(tr("current"));
    m_start_policy_field->addItem(tr("stored"));
    m_start_policy_field->addItem(tr("user_defined"));
    m_start_policy_field->setCurrentIndex(0);
    m_start_policies.push_back("current");
    m_start_policies.push_back("stored");
    m_start_policies.push_back("user_defined");
    connect(m_start_policy_field,SIGNAL(activated (int)),this, SLOT(treat_start_policy_selection_event()));

    l_general_layout->addRow(tr("Start policy")+" :",m_start_policy_field);

    m_iteration_number_field = new QLineEdit("");
    m_iteration_number_field->setInputMask("000000000000");
    l_general_layout->addRow(tr("Iteration number")+" :",m_iteration_number_field);

    m_start_sequence_number_field = new QLineEdit("");
    m_start_sequence_number_field->setInputMask("000000000000");
    l_general_layout->addRow(tr("Start sequence number")+" :",m_start_sequence_number_field);

    m_replication_domain_label = new QLabel(tr("replication_domain")+" :");
    m_replication_domain_field = new QLineEdit("");
    l_general_layout->addRow(tr("Replication domain")+" :",m_replication_domain_field);


    // Module library configuration
    QWidget * l_module_library_widget = new QWidget();
    l_tab_widget->addTab(l_module_library_widget,tr("Module libraries"));
    QVBoxLayout * l_module_library_layout = new QVBoxLayout(l_module_library_widget);

    QGroupBox * l_module_libraries_configuration = new QGroupBox(tr("Module Libraries"));
    l_module_library_layout->addWidget(l_module_libraries_configuration);

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
    QWidget * l_module_configuration_widget = new QWidget();
    l_tab_widget->addTab(l_module_configuration_widget,tr("Modules"));

    QHBoxLayout * l_module_configuration_layout = new QHBoxLayout(l_module_configuration_widget);

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
    QWidget * l_domain_jump_configuration_widget = new QWidget();
    l_tab_widget->addTab(l_domain_jump_configuration_widget,tr("Domain jumps"));

    QVBoxLayout * l_domain_jump_layout = new QVBoxLayout(l_domain_jump_configuration_widget);

    m_domain_jump_configuration_widget = new domain_jump_configuration_widget();
    l_domain_jump_layout->addWidget(m_domain_jump_configuration_widget);

    // Proxy configuration
    QWidget * l_proxy_configuration_widget = new QWidget();
    l_tab_widget->addTab(l_proxy_configuration_widget,tr("Network"));

    QVBoxLayout * l_proxy_layout = new QVBoxLayout(l_proxy_configuration_widget);

    m_proxy_conf_box = new QCheckBox(tr("Behind a proxy"));
    connect(m_proxy_conf_box,SIGNAL(stateChanged(int)),this, SLOT(treat_proxy_conf_box_state_changed_event()));
    l_proxy_layout->addWidget(m_proxy_conf_box);

    QFormLayout *l_form_layout = new QFormLayout();
    l_proxy_layout->addLayout(l_form_layout);

    m_proxy_name_field = new QLineEdit("");
    l_form_layout->addRow(tr("Proxy name")+" :",m_proxy_name_field);

    m_proxy_port_field = new QLineEdit("");
    m_proxy_port_field->setInputMask("00000");
    l_form_layout->addRow(tr("Proxy port")+" :",m_proxy_port_field);

    m_proxy_login_field = new QLineEdit("");
    l_form_layout->addRow(tr("Proxy login")+" :",m_proxy_login_field);

    m_proxy_password_field = new QLineEdit("");
    m_proxy_password_field->setEchoMode(QLineEdit::Password);
    l_form_layout->addRow(tr("Proxy password")+" :",m_proxy_password_field);

    treat_start_policy_selection_event();
    treat_proxy_conf_box_state_changed_event();
  }

  //----------------------------------------------------------------
  void configuration_widget::save_configuration_file(const std::string & p_file_name)
  {
      std::ofstream l_config_file;
      l_config_file.open(p_file_name.c_str());
      if(!l_config_file.is_open())
      {
          // Display Error message
      }

      // XML Header
      l_config_file << "<?xml version='1.0' encoding='UTF-8'?>" << std::endl;
      l_config_file << "<osm_diff_watcher_configuration>" << std::endl ;

      // Start Policy
      l_config_file << "  <variable name=\"start_policy\" value=\"" << m_start_policy_field->currentText().toStdString() << "\"/>" << std::endl ;
      l_config_file << "  <variable name=\"iteration_number\" value=\"" << m_iteration_number_field->text().toStdString() << "\"/>" << std::endl ;
      l_config_file << "  <variable name=\"start_sequence_number\" value=\"" << m_start_sequence_number_field->text().toStdString() << "\"/>" << std::endl ;
      l_config_file << "  <variable name=\"replication_domain\" value=\"" << m_replication_domain_field->text().toStdString() << "\"/>" << std::endl;

      // Library list
      for(int l_iter = 0;
          l_iter < m_library_list_table->rowCount();
          ++l_iter)
      {
          l_config_file << "  <library name=\"" << m_library_list_table->item(l_iter,0)->text().toStdString() << "\"/>" << std::endl ;
      }

      // Module list
      const std::map<std::string,osm_diff_analyzer_if::module_configuration> & l_modules = m_module_list_widget->get_modules();
      for(std::map<std::string,osm_diff_analyzer_if::module_configuration>::const_iterator l_iter = l_modules.begin();
          l_iter != l_modules.end();
          ++l_iter)
      {
          l_config_file << "  <analyzer type=\"" << l_iter->second.get_type() << "\" name=\"" << l_iter->second.get_name() << "\">" << std::endl ;
          const std::map<std::string,std::string> & l_parameters = l_iter->second.get_parameters();
          for(std::map<std::string,std::string>::const_iterator l_param_iter = l_parameters.begin();
              l_param_iter != l_parameters.end();
              ++l_param_iter)
            {
              l_config_file << "    <parameter name=\"" << l_param_iter->first <<"\" value =\"" << l_param_iter->second << "\"/>" << std::endl;
            }
          l_config_file << "  </analyzer>" << std::endl ;

      }
      // Domain jumps
      std::vector<osm_diff_watcher::replication_domain_jump> l_domain_jumps;
      m_domain_jump_configuration_widget->get_domain_jumps(l_domain_jumps);
      for(std::vector<osm_diff_watcher::replication_domain_jump>::const_iterator l_iter = l_domain_jumps.begin();
          l_iter != l_domain_jumps.end();
          ++l_iter)
      {
          l_config_file << "  <replication_domain_jump last_sequence_number=\"" << l_iter->get_old_id() << "\" ";
          l_config_file << "old_domain=\"" << l_iter->get_old_domain() << "\" ";
          l_config_file << "first_sequence_number=\"" << l_iter->get_new_id() << "\" ";
          l_config_file << "new_domain=\"" << l_iter->get_new_domain() << "\" />" << std::endl;
      }

      // Proxy authentication
      l_config_file << "  <variable name=\"proxy_authentication.proxy_name\" value=\"" << m_proxy_name_field->text().toStdString() << "\"/>" << std::endl ;
      l_config_file << "  <variable name=\"proxy_authentication.proxy_port\" value=\"" << m_proxy_port_field->text().toStdString() << "\"/>" << std::endl ;
      l_config_file << "  <variable name=\"proxy_authentication.proxy_login\" value=\"" << m_proxy_login_field->text().toStdString() << "\"/>" << std::endl ;
      l_config_file << "  <variable name=\"proxy_authentication.proxy_password\" value=\"" << m_proxy_password_field->text().toStdString() << "\"/>" << std::endl ;

      l_config_file << "</osm_diff_watcher_configuration>" << std::endl ;

      l_config_file.close();
  }

  //----------------------------------------------------------------------------
  void configuration_widget::load_configuration_file(const std::string & p_library)
  {
      this->clear();
      const osm_diff_watcher::configuration * const l_configuration = osm_diff_watcher::configuration_parser::parse(p_library);

      // Libraries
      for(std::vector<std::string>::const_iterator l_iter = l_configuration->get_libraries().begin();
          l_iter != l_configuration->get_libraries().end();
          ++l_iter)
        {
            m_library_list_table->add_library(*l_iter);
        }

      // Proxy authentication
      std::string l_proxy_name = l_configuration->get_variable("proxy_authentication.proxy_name");
      std::string l_proxy_port = l_configuration->get_variable("proxy_authentication.proxy_port");
      std::string l_proxy_login = l_configuration->get_variable("proxy_authentication.proxy_login");
      std::string l_proxy_password = l_configuration->get_variable("proxy_authentication.proxy_password");

      m_proxy_name_field->setText(l_proxy_name.c_str());
      m_proxy_port_field->setText(l_proxy_port.c_str());
      m_proxy_login_field->setText(l_proxy_login.c_str());
      m_proxy_password_field->setText(l_proxy_password.c_str());

      if(l_proxy_name != "" &&
         l_proxy_port != "" &&
         l_proxy_login != "" &&
         l_proxy_password != "")
        {
          m_proxy_conf_box->setChecked(true);
        }

      // Manage replication dommain
      std::string l_replication_domain = l_configuration->get_variable("replication_domain");
      m_replication_domain_field->setText(l_replication_domain.c_str());

      // Start Policy
      std::string l_start_policy = l_configuration->get_variable("start_policy");
      if(l_start_policy == "current" || l_start_policy == "")
      {
          m_start_policy_field->setCurrentIndex(m_start_policy_current_index);
      }
      else if(l_start_policy == "stored")
      {
          m_start_policy_field->setCurrentIndex(m_start_policy_stored_index);
      }
      else if(l_start_policy == "user_defined")
      {
          m_start_policy_field->setCurrentIndex(m_start_policy_user_defined_index);
      }
      treat_start_policy_selection_event();

      // Start sequence number
      m_start_sequence_number_field->setText(l_configuration->get_variable("start_sequence_number").c_str());
      m_iteration_number_field->setText(l_configuration->get_variable("iteration_number").c_str());

      // Module configuration
      const std::vector<osm_diff_analyzer_if::module_configuration*> & l_module_configurations = l_configuration->get_module_configurations();
      for(std::vector<osm_diff_analyzer_if::module_configuration*>::const_iterator l_iter = l_module_configurations.begin();
          l_iter != l_module_configurations.end();
          ++l_iter)
      {
          m_module_list_widget->add(**l_iter);
      }
      // Domain jumps
      const std::map<uint64_t,osm_diff_watcher::replication_domain_jump> & l_domain_jumps =l_configuration->get_domain_jumps();
      for(std::map<uint64_t,osm_diff_watcher::replication_domain_jump>::const_iterator l_iter = l_domain_jumps.begin();
          l_iter != l_domain_jumps.end();
          ++l_iter)
      {
          m_domain_jump_configuration_widget->add(l_iter->second);
      }
      delete l_configuration;
  }

  //----------------------------------------------------------------------------
  void configuration_widget::treat_proxy_conf_box_state_changed_event()
  {
    std::cout << "configuration_widget::received proxy_conf_box_state_changed Event" << std::endl ;
    m_proxy_name_field->setEnabled(m_proxy_conf_box->isChecked());
    m_proxy_port_field->setEnabled(m_proxy_conf_box->isChecked());
    m_proxy_login_field->setEnabled(m_proxy_conf_box->isChecked());
    m_proxy_password_field->setEnabled(m_proxy_conf_box->isChecked());
    emit config_modified();
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
        m_start_sequence_number_field->setEnabled(false);
        m_replication_domain_field->setEnabled(false);
      }
    else if((unsigned int)m_start_policy_field->currentIndex() == m_start_policy_current_index)
      {
        m_start_sequence_number_field->setEnabled(false);
        m_replication_domain_field->setEnabled(true);
      }
    else
      {
        m_start_sequence_number_field->setEnabled(true);
        m_replication_domain_field->setEnabled(true);
      }
    emit config_modified();
  }
  
  //----------------------------------------------------------------------------
  void configuration_widget::clear(void)
  {
    // Start Policy
    m_start_policy_field->setCurrentIndex(0);
    m_iteration_number_field->setText("");
    m_start_sequence_number_field->setText("");
    m_replication_domain_field->setText("");

    // Library list
    m_library_list_table->setRowCount(0);

    //Modules
    m_module_list_widget->clear();
    m_module_parameter_widget->clear();

    // Domain jumps
    m_domain_jump_configuration_widget->clear();
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
	    l_module_file.close();
	    m_library_list_table->add_library(l_file_name_std);
            emit config_modified();
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
        emit config_modified();
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
      emit config_modified();
  }

  //----------------------------------------------------------------------------
  void configuration_widget::treat_module_parameter_modified_event(const QString & p_name,const QString & p_value)
  {
      std::cout << "configuration_widget::received module_parameter_modified_event" << std::endl ;
      osm_diff_analyzer_if::module_configuration & l_module_config = m_module_list_widget->get_selection_configuration();
      l_module_config.remove_parameter(p_name.toStdString());
      l_module_config.add_parameter(p_name.toStdString(),p_value.toStdString());
      m_module_parameter_widget->display_module_parameters(l_module_config.get_parameters());
      emit config_modified();
  }

  //----------------------------------------------------------------------------
  void configuration_widget::treat_module_parameter_removed_event(const QString & p_name)
  {
      std::cout << "configuration_widget::received module_parameter_removed_event " << p_name.toStdString() << std::endl ;
      osm_diff_analyzer_if::module_configuration & l_module_config = m_module_list_widget->get_selection_configuration();
      l_module_config.remove_parameter(p_name.toStdString());
      m_module_parameter_widget->display_module_parameters(l_module_config.get_parameters());
      emit config_modified();
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

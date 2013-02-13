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
#include "module_parameter_widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <iostream>
#include <cassert>

namespace soda_QtUi
{
  //------------------------------------------------------------------------
  module_parameter_widget::module_parameter_widget(QWidget * parent):
    QWidget(parent),
    m_parameters(NULL)
  {
    QVBoxLayout * l_layout = new QVBoxLayout(this);

    QHBoxLayout * l_module_parameter_fields_layout = new QHBoxLayout();
    l_layout->addLayout(l_module_parameter_fields_layout);

    l_module_parameter_fields_layout->addWidget(new QLabel(tr("Name")+" :"));
    m_module_parameter_name_field = new QLineEdit("");
    l_module_parameter_fields_layout->addWidget(m_module_parameter_name_field);
    connect(m_module_parameter_name_field,SIGNAL(textEdited(const QString&)),this, SLOT(treat_module_parameter_name_modification_event()));
    l_module_parameter_fields_layout->addWidget(new QLabel(tr("Value")+" :"));
    m_module_parameter_value_field = new QLineEdit("");
    l_module_parameter_fields_layout->addWidget(m_module_parameter_value_field);
    connect(m_module_parameter_value_field,SIGNAL(textEdited(const QString&)),this, SLOT(treat_module_parameter_value_modification_event()));

    l_layout->addWidget(new QLabel(tr("Parameter list")+" :"));

    m_module_parameter_list_table = new module_parameter_list_table();
    l_layout->addWidget(m_module_parameter_list_table);
    connect(m_module_parameter_list_table,SIGNAL(cellClicked (int, int)),this, SLOT(treat_module_parameter_selected_event(int)));
    connect(m_module_parameter_list_table,SIGNAL(itemSelectionChanged()),this, SLOT(treat_module_parameter_selection_changed_event()));

    QHBoxLayout * l_module_parameter_button_layout = new QHBoxLayout();
    l_layout->addLayout(l_module_parameter_button_layout);

    m_add_module_parameter_button = new QPushButton(tr("Create"));
    l_module_parameter_button_layout->addWidget(m_add_module_parameter_button);
    connect(m_add_module_parameter_button,SIGNAL(clicked()),this,SLOT(treat_add_module_parameter_event()));
    m_add_module_parameter_button->setEnabled(false);

    m_modify_module_parameter_button = new QPushButton(tr("Modify"));
    l_module_parameter_button_layout->addWidget(m_modify_module_parameter_button);
    connect(m_modify_module_parameter_button,SIGNAL(clicked()),this,SLOT(treat_modify_module_parameter_event()));
    m_modify_module_parameter_button->setEnabled(false);

    m_remove_module_parameter_button = new QPushButton(tr("Remove"));
    l_module_parameter_button_layout->addWidget(m_remove_module_parameter_button);
    connect(m_remove_module_parameter_button,SIGNAL(clicked()),this,SLOT(treat_remove_module_parameter_event()));
    m_remove_module_parameter_button->setEnabled(false);
  }

  //------------------------------------------------------------------------
  void module_parameter_widget::setEnabled(bool p_enabled)
  {
    m_module_parameter_name_field->setEnabled(p_enabled);
    m_module_parameter_value_field->setEnabled(p_enabled);
    //m_module_parameter_list_table->setEnabled(p_enabled);
    m_add_module_parameter_button->setEnabled(p_enabled);
    m_modify_module_parameter_button->setEnabled(p_enabled);
    m_remove_module_parameter_button->setEnabled(p_enabled);
    m_module_parameter_list_table->setRowCount(0);
  }

  //------------------------------------------------------------------------
  void module_parameter_widget::display_module_parameters(const std::map<std::string,std::string> & p_parameters)
  {
    m_module_parameter_list_table->setRowCount(0);
    for(std::map<std::string,std::string>::const_iterator l_iter_param = p_parameters.begin();
	l_iter_param != p_parameters.end();
	++l_iter_param)
      {
	m_module_parameter_list_table->add_parameter(l_iter_param->first,l_iter_param->second);
      }
    m_parameters = & p_parameters;
  }
  //----------------------------------------------------------------------------
  void module_parameter_widget::treat_module_parameter_name_modification_event()
  {
    std::cout << "module_parameter_widget::received module_parameter_name_modification_event" << std::endl ;
    treat_module_parameter_field_change_event();
  }

  //----------------------------------------------------------------------------
  void module_parameter_widget::treat_module_parameter_value_modification_event()
  {
    std::cout << "module_parameter_widget::received module_parameter_value_modification_event" << std::endl ;
    treat_module_parameter_field_change_event();
  }

  //----------------------------------------------------------------------------
  void module_parameter_widget::treat_module_parameter_selected_event(int p_row)
  {
    std::cout << "module_parameter_widget::received module_parameter_selected_event " << p_row << std::endl ;
    treat_module_parameter_selection_changed_event();
  }

  //----------------------------------------------------------------------------
  void module_parameter_widget::treat_module_parameter_selection_changed_event()
  {
    std::cout << "module_parameter_widget::received module_parameter_selection_changed_event " << std::endl ;
    if(m_module_parameter_list_table->selectedItems().isEmpty())
      {
	m_module_parameter_name_field->setText("");
	m_module_parameter_value_field->setText("");
      }
    else
      {
	m_module_parameter_name_field->setText(m_module_parameter_list_table->item(m_module_parameter_list_table->currentRow(),0)->text());
	m_module_parameter_value_field->setText(m_module_parameter_list_table->item(m_module_parameter_list_table->currentRow(),1)->text());
      }
    treat_module_parameter_field_change_event();
  }

  //----------------------------------------------------------------------------
  void module_parameter_widget::treat_add_module_parameter_event()
  {
    std::cout << "module_parameter_widget::received add_module_parameter_event" << std::endl ;
    emit parameter_created(m_module_parameter_name_field->text(),m_module_parameter_value_field->text());
    m_module_parameter_name_field->setText("");
    m_module_parameter_value_field->setText("");
  }

  //----------------------------------------------------------------------------
  void module_parameter_widget::treat_modify_module_parameter_event()
  {
    std::cout << "module_parameter_widget::received modify_module_parameter_event" << std::endl ;
    assert(!m_module_parameter_list_table->selectedItems().isEmpty());
    emit parameter_modified(m_module_parameter_name_field->text(),m_module_parameter_value_field->text());
    m_module_parameter_name_field->setText("");
    m_module_parameter_value_field->setText("");
  }

  //----------------------------------------------------------------------------
  void module_parameter_widget::treat_remove_module_parameter_event()
  {
    std::cout << "module_parameter_widget::received remove_module_parameter_event" << std::endl ;
    assert(!m_module_parameter_list_table->selectedItems().isEmpty());
    emit parameter_removed(m_module_parameter_list_table->item(m_module_parameter_list_table->currentRow(),0)->text());
    m_module_parameter_name_field->setText("");
    m_module_parameter_value_field->setText("");
  }

  //----------------------------------------------------------------------------
  void module_parameter_widget::treat_module_parameter_field_change_event(void)
  {
    std::string l_name = m_module_parameter_name_field->text().toStdString();
    std::string l_value = m_module_parameter_value_field->text().toStdString();
    bool l_complete = l_name != "" && l_value != "";
    bool l_selected =  !m_module_parameter_list_table->selectedItems().isEmpty();
    bool l_existing = m_parameters->find(l_name) != m_parameters->end();

    bool l_module_parameter_creation_enabled = false;
    bool l_module_parameter_modification_enabled = false;
    bool l_module_parameter_deletion_enabled = l_selected;

    if(l_complete)
      {
	if(l_selected)
	  {
	    std::string l_original_name = m_module_parameter_list_table->item(m_module_parameter_list_table->currentRow(),0)->text().toStdString();
	    std::map<std::string,std::string>::const_iterator l_iter_original = m_parameters->find(l_original_name);
	    assert(l_iter_original !=  m_parameters->end());
	    if(l_original_name == l_name && l_value != l_iter_original->second)
	      {
		l_module_parameter_modification_enabled = true;
	      }
	    else if(l_original_name != l_name && !l_existing)
	      {
		l_module_parameter_creation_enabled = true;
		m_module_parameter_list_table->clearSelection();
	      }
	  }
	else if(!l_existing)
	  {
	    l_module_parameter_creation_enabled = true;
	  }
      }
    m_add_module_parameter_button->setEnabled(l_module_parameter_creation_enabled);
    m_modify_module_parameter_button->setEnabled(l_module_parameter_modification_enabled);
    m_remove_module_parameter_button->setEnabled(l_module_parameter_deletion_enabled);
  }

}

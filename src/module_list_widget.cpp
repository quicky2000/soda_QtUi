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
#include "module_list_widget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <iostream>

namespace soda_QtUi
{
    //------------------------------------------------------------------------
    module_list_widget::module_list_widget(QWidget *parent):
        QWidget(parent)
    {
        QVBoxLayout * l_layout = new QVBoxLayout(this);

        QHBoxLayout * l_module_fields_layout = new QHBoxLayout();
        l_layout->addLayout(l_module_fields_layout);

        l_module_fields_layout->addWidget(new QLabel(tr("Name")+" :"));
        m_module_name_field = new QLineEdit("");
        l_module_fields_layout->addWidget(m_module_name_field);
        connect(m_module_name_field,SIGNAL(textEdited(const QString&)),this, SLOT(treat_module_name_modification_event()));
        l_module_fields_layout->addWidget(new QLabel(tr("Type")+" :"));
        m_module_type_field = new QLineEdit("");
        l_module_fields_layout->addWidget(m_module_type_field);
        connect(m_module_type_field,SIGNAL(textEdited(const QString&)),this, SLOT(treat_module_type_modification_event()));
        l_module_fields_layout->addWidget(new QLabel(tr("Enabled")+" :"));
        m_module_enabled_field = new QCheckBox();
        m_module_enabled_field->setChecked(true);
        l_module_fields_layout->addWidget(m_module_enabled_field);
        connect(m_module_enabled_field,SIGNAL(stateChanged(int)),this, SLOT(treat_module_enabled_state_changed_event()));

        l_layout->addWidget(new QLabel(tr("Module list")+" :"));

        m_module_list_table = new module_list_table();
        l_layout->addWidget(m_module_list_table);
        connect(m_module_list_table,SIGNAL(cellClicked (int, int)),this, SLOT(treat_module_selected_event(int)));
        connect(m_module_list_table,SIGNAL(itemSelectionChanged()),this, SLOT(treat_module_selection_changed_event()));

        QHBoxLayout * l_module_button_layout = new QHBoxLayout();
        l_layout->addLayout(l_module_button_layout);

        m_add_module_button = new QPushButton(tr("Create"));
        l_module_button_layout->addWidget(m_add_module_button);
        connect(m_add_module_button,SIGNAL(clicked()),this,SLOT(treat_add_module_event()));
        m_add_module_button->setEnabled(false);

        m_modify_module_button = new QPushButton(tr("Modify"));
        l_module_button_layout->addWidget(m_modify_module_button);
        connect(m_modify_module_button,SIGNAL(clicked()),this,SLOT(treat_modify_module_event()));
        m_modify_module_button->setEnabled(false);

        m_remove_module_button = new QPushButton(tr("Remove"));
        l_module_button_layout->addWidget(m_remove_module_button);
        connect(m_remove_module_button,SIGNAL(clicked()),this,SLOT(treat_remove_module_event()));
        m_remove_module_button->setEnabled(false);
    }

    //----------------------------------------------------------------------------
    const std::map<std::string,osm_diff_analyzer_if::module_configuration> & module_list_widget::get_modules(void)
    {
        return m_module_configurations;
    }
    //----------------------------------------------------------------------------
    void module_list_widget::clear(void)
    {
        clean_module_fields();
        m_module_list_table->setRowCount(0);
        m_module_configurations.clear();
        m_add_module_button->setEnabled(false);
        m_modify_module_button->setEnabled(false);
        m_remove_module_button->setEnabled(false);
    }

    //----------------------------------------------------------------------------
    void module_list_widget::add(const osm_diff_analyzer_if::module_configuration & p_conf)
    {
        std::map<std::string,osm_diff_analyzer_if::module_configuration>::iterator l_iter = m_module_configurations.insert(make_pair(p_conf.get_name(),osm_diff_analyzer_if::module_configuration(p_conf.get_name(),p_conf.get_type(),p_conf.is_enabled()))).first;
        m_module_list_table->add_module(p_conf.get_name(),p_conf.get_type(), p_conf.is_enabled());
        const std::map<std::string,std::string> & l_parameters = p_conf.get_parameters();
        for(std::map<std::string,std::string>::const_iterator l_iter_param = l_parameters.begin();
            l_iter_param != l_parameters.end();
            ++l_iter_param)
        {
            l_iter->second.add_parameter(l_iter_param->first,l_iter_param->second);
        }

    }

    //----------------------------------------------------------------------------
    void module_list_widget::treat_module_selected_event(int p_row)
    {
      std::cout << "configuration_widget::received module_selected_event " << p_row << std::endl ;
      treat_module_selection_changed_event();
    }

    //----------------------------------------------------------------------------
    void module_list_widget::treat_module_selection_changed_event()
    {
      std::cout << "configuration_widget::received module_selection_changed_event" << std::endl ;
      if(m_module_list_table->selectedItems().isEmpty())
        {
          std::cout << "Empty selection" << std::endl ;
          clean_module_fields();
          emit no_more_selection();
        }
      else
        {
          // Get row content
          m_module_name_field->setText(m_module_list_table->item(m_module_list_table->currentRow(),0)->text());
          m_module_type_field->setText(m_module_list_table->item(m_module_list_table->currentRow(),1)->text());
          std::string l_enabled = m_module_list_table->item(m_module_list_table->currentRow(),2)->text().toStdString();
          m_module_enabled_field->setChecked(l_enabled=="Yes");

          // Display module parameters
          std::map<std::string,osm_diff_analyzer_if::module_configuration>::iterator l_iter = m_module_configurations.find(m_module_list_table->item(m_module_list_table->currentRow(),0)->text().toStdString());
          assert(l_iter !=  m_module_configurations.end());
          emit module_selected();
          treat_module_field_change_event();
        }
    }

    //----------------------------------------------------------------------------
    void module_list_widget::treat_module_name_modification_event()
    {
      std::cout << "configuration_widget::received module_name_modification_event" << std::endl ;
      treat_module_field_change_event();
    }

    //----------------------------------------------------------------------------
    void module_list_widget::treat_module_type_modification_event()
    {
      std::cout << "configuration_widget::received module_type_modification_event" << std::endl ;
      treat_module_field_change_event();
    }

    //----------------------------------------------------------------------------
    void module_list_widget::treat_module_enabled_state_changed_event()
    {
      std::cout << "configuration_widget::received module_enabled_state_changed_event" << std::endl ;
      treat_module_field_change_event();
    }

    //----------------------------------------------------------------------------
    void module_list_widget::clean_module_fields(void)
    {
      m_module_name_field->setText("");
      m_module_type_field->setText("");
      treat_module_field_change_event();
    }

    //----------------------------------------------------------------------------
    void module_list_widget::treat_add_module_event()
    {
      std::cout << "configuration_widget::received add_module_event" << std::endl ;
      assert(m_module_list_table->selectedItems().isEmpty());
      std::string l_name = m_module_name_field->text().toStdString();
      add(osm_diff_analyzer_if::module_configuration(l_name,m_module_type_field->text().toStdString(), m_module_enabled_field->isChecked()));
      clean_module_fields();
    }

    //----------------------------------------------------------------------------
    void module_list_widget::treat_modify_module_event()
    {
      std::cout << "configuration_widget::received modify_module_event" << std::endl ;
      assert(!m_module_list_table->selectedItems().isEmpty());
      std::string l_original_name = m_module_list_table->item(m_module_list_table->currentRow(),0)->text().toStdString();
      std::map<std::string,osm_diff_analyzer_if::module_configuration>::iterator l_iter_original = m_module_configurations.find(l_original_name);
      assert(l_iter_original !=  m_module_configurations.end());
      m_module_configurations.erase(l_iter_original);
      m_module_configurations.insert(make_pair(m_module_name_field->text().toStdString(),osm_diff_analyzer_if::module_configuration(m_module_name_field->text().toStdString(),m_module_type_field->text().toStdString(), m_module_enabled_field->isChecked())));
      m_module_list_table->update_row(m_module_list_table->currentRow(),m_module_name_field->text().toStdString(),m_module_type_field->text().toStdString(), m_module_enabled_field->isChecked());
      clean_module_fields();
      m_module_list_table->selectRow(m_module_list_table->currentRow());
    }

    //----------------------------------------------------------------------------
    void module_list_widget::treat_remove_module_event()
    {
      std::cout << "configuration_widget::received remove_module_event" << std::endl ;
      assert(!m_module_list_table->selectedItems().isEmpty());
      std::string l_original_name = m_module_list_table->item(m_module_list_table->currentRow(),0)->text().toStdString();
      std::map<std::string,osm_diff_analyzer_if::module_configuration>::iterator l_iter_original = m_module_configurations.find(l_original_name);
      assert(l_iter_original !=  m_module_configurations.end());
      m_module_configurations.erase(l_iter_original);
      m_module_list_table->removeRow(m_module_list_table->currentRow());
      clean_module_fields();
    }

    //----------------------------------------------------------------------------
    void module_list_widget::treat_module_field_change_event(void)
    {
      // Get info from Gui fields
      std::string l_name = m_module_name_field->text().toStdString();
      std::string l_type = m_module_type_field->text().toStdString();
      bool l_enabled = m_module_enabled_field->isChecked();

      bool l_complete = l_name != "" && l_type != "";

      std::map<std::string,osm_diff_analyzer_if::module_configuration>::const_iterator l_iter = m_module_configurations.find(l_name);
      bool l_existing = m_module_configurations.end() != l_iter;

      bool l_selected =  !m_module_list_table->selectedItems().isEmpty();

      bool l_module_creation_enabled = false;
      bool l_module_modification_enabled = false;

      if(l_complete)
        {
          if(l_selected)
            {
              std::string l_original_name = m_module_list_table->item(m_module_list_table->currentRow(),0)->text().toStdString();
              std::map<std::string,osm_diff_analyzer_if::module_configuration>::const_iterator l_iter_original = m_module_configurations.find(l_original_name);
              assert(l_iter_original !=  m_module_configurations.end());
              if(l_original_name == l_name && ( l_type != l_iter_original->second.get_type() || l_iter_original->second.is_enabled() != l_enabled))
                {
                  l_module_modification_enabled = true;
                }
              else if(l_original_name != l_name && !l_existing)
                {
                  l_module_creation_enabled = true;
                  m_module_list_table->clearSelection();
                }

            }
          else if(!l_existing)
            {
              l_module_creation_enabled = true;
            }
        }
      bool l_module_deletion_enabled = l_selected;

      m_add_module_button->setEnabled(l_module_creation_enabled);
      m_modify_module_button->setEnabled(l_module_modification_enabled);
      m_remove_module_button->setEnabled(l_module_deletion_enabled);
    }

    //------------------------------------------------------------------------
    osm_diff_analyzer_if::module_configuration & module_list_widget::get_selection_configuration(void)
    {
        assert(!m_module_list_table->selectedItems().isEmpty());
        std::map<std::string,osm_diff_analyzer_if::module_configuration>::iterator l_iter = m_module_configurations.find(m_module_list_table->item(m_module_list_table->currentRow(),0)->text().toStdString());
        assert(l_iter !=  m_module_configurations.end());
        return l_iter->second;
    }


}

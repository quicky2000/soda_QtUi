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
#include "domain_jump_configuration_widget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <iostream>
#include <cassert>

namespace soda_QtUi
{
  //----------------------------------------------------------------------------
  domain_jump_configuration_widget::domain_jump_configuration_widget(QWidget * parent):
    QWidget(parent)
  {
    QVBoxLayout * l_layout = new QVBoxLayout(this);

    QHBoxLayout * l_layout_field = new QHBoxLayout();
    l_layout->addLayout(l_layout_field);

    l_layout_field->addWidget(new QLabel(tr("Start domain")+" :"));

    m_start_domain = new QLineEdit("");
    connect(m_start_domain,SIGNAL(textEdited(const QString&)),this, SLOT(treat_field_modification_event()));
    l_layout_field->addWidget(m_start_domain);

    l_layout_field->addWidget(new QLabel(tr("Start sequence number")+" :"));

    m_start_seq_number = new QLineEdit("");
    m_start_seq_number->setInputMask("000000000000");
    connect(m_start_seq_number,SIGNAL(textEdited(const QString&)),this, SLOT(treat_field_modification_event()));
    l_layout_field->addWidget(m_start_seq_number);

    l_layout_field->addWidget(new QLabel(tr("End domain")+" :"));

    m_old_domain = new QLineEdit("");
    connect(m_old_domain,SIGNAL(textEdited(const QString&)),this, SLOT(treat_field_modification_event()));
    l_layout_field->addWidget(m_old_domain);

    l_layout_field->addWidget(new QLabel(tr("End sequence number")+" :"));

    m_old_seq_number = new QLineEdit("");
    m_old_seq_number->setInputMask("000000000000");
    connect(m_old_seq_number,SIGNAL(textEdited(const QString&)),this, SLOT(treat_field_modification_event()));
    l_layout_field->addWidget(m_old_seq_number);

    l_layout->addWidget(new QLabel(tr("Domain Jump list")+" :"));

    m_domain_jump_table = new domain_jump_table();
    connect(m_domain_jump_table,SIGNAL(cellClicked (int, int)),this, SLOT(treat_domain_selected_event(int)));
    connect(m_domain_jump_table,SIGNAL(itemSelectionChanged()),this, SLOT(treat_domain_selection_changed_event()));
    l_layout->addWidget(m_domain_jump_table);

    QHBoxLayout * l_button_layout = new QHBoxLayout();
    l_layout->addLayout(l_button_layout);

    m_add_button = new QPushButton(tr("Add"));
    connect(m_add_button,SIGNAL(clicked()),this,SLOT(treat_add_event()));
    m_add_button->setEnabled(false);
    l_button_layout->addWidget(m_add_button);

    m_remove_button = new QPushButton(tr("Remove"));
    connect(m_remove_button,SIGNAL(clicked()),this,SLOT(treat_remove_event()));
    m_remove_button->setEnabled(false);
    l_button_layout->addWidget(m_remove_button);
  }

  //----------------------------------------------------------------------------
  void domain_jump_configuration_widget::treat_field_modification_event()
  {
    std::cout << "domain_jump_configuration_widget::received field_modification_event " << std::endl ;
    std::string l_start_domain = m_start_domain->text().toStdString();
    std::string l_start_seq_number = m_start_seq_number->text().toStdString();
    std::string l_old_domain = m_old_domain->text().toStdString();
    std::string l_old_seq_number = m_old_seq_number->text().toStdString();
    bool l_complete = l_start_domain != "" && l_start_seq_number != "" &&  l_old_domain != "" && l_old_seq_number != "";
    m_add_button->setEnabled(l_complete && !m_domain_jump_table->contains(l_start_domain,l_start_seq_number,l_old_domain,l_old_seq_number));
  }

  //----------------------------------------------------------------------------
  void domain_jump_configuration_widget::treat_domain_selected_event(int p_row)
  {
    std::cout << "domain_jump_configuration_widget::received domain_selected_event " << p_row << std::endl ;
    treat_domain_selection_changed_event();
  }

  //----------------------------------------------------------------------------
  void domain_jump_configuration_widget::treat_domain_selection_changed_event()
  {
    std::cout << "domain_configuration_widget::received domain_selection_changed_event" << std::endl ;
    m_remove_button->setEnabled(!m_domain_jump_table->selectedItems().isEmpty());
  }

  //----------------------------------------------------------------------------
  void domain_jump_configuration_widget::treat_add_event()
  {
    std::cout << "domain_configuration_widget::received add_event" << std::endl ;
    m_domain_jump_table->add(m_start_domain->text().toStdString(),
			     m_start_seq_number->text().toStdString(),
			     m_old_domain->text().toStdString(),
			     m_old_seq_number->text().toStdString());
    m_add_button->setEnabled(false);
    clean_fields();
  }

  //----------------------------------------------------------------------------
  void domain_jump_configuration_widget::treat_remove_event()
  {
    std::cout << "domain_configuration_widget::received remove_event" << std::endl ;
    assert(!m_domain_jump_table->selectedItems().isEmpty());
    m_domain_jump_table->removeRow(m_domain_jump_table->currentRow());
    m_remove_button->setEnabled(false);
    clean_fields();
  }

  //----------------------------------------------------------------------------
  void domain_jump_configuration_widget::clean_fields(void)
  {
    m_start_domain->setText("");
    m_start_seq_number->setText("");
    m_old_domain->setText("");
    m_old_seq_number->setText("");
  }

}
//EOF

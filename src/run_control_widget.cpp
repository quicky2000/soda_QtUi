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
#include "run_control_widget.h"
#include "configuration_dialog_widget.h"


#include <QVBoxLayout>
#include <QHBoxLayout>
#include <iostream>

namespace soda_QtUi
{
  //----------------------------------------------------------------------------
  run_control_widget::run_control_widget(QWidget * parent):
    QWidget(parent)
  {
    QVBoxLayout * l_layout = new QVBoxLayout(this);
    QHBoxLayout * l_button_layout = new QHBoxLayout();
    l_layout->addLayout(l_button_layout);

    m_configure_button = new QPushButton(tr("Configure"));
    connect(m_configure_button,SIGNAL(clicked()),this,SLOT(treat_configure_event()));
    l_button_layout->addWidget(m_configure_button);

    m_start_button = new QPushButton(tr("Start"));
    connect(m_start_button,SIGNAL(clicked()),this,SLOT(treat_start_event()));
    l_button_layout->addWidget(m_start_button);

    m_stop_button = new QPushButton(tr("Stop"));
    connect(m_stop_button,SIGNAL(clicked()),this,SLOT(treat_stop_event()));
    l_button_layout->addWidget(m_stop_button);

    m_text_zone = new QTextEdit();
    //    m_text_zone->setReadOnly(true);
    m_text_zone->insertHtml("<html>");
    m_text_zone->insertHtml("<head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">");
    m_text_zone->insertHtml("<title>alignment_detector Node Alignement Report </title>");
    m_text_zone->insertHtml("</head>");
    m_text_zone->insertHtml("<body><H1>alignment_detector Node alignement Report</H1>");
    m_text_zone->insertHtml("<A HREF=\"http://www.openstreetmap.org/browse/way/190882978\">Way 190882978</A> has been aligned by <A HREF=\"http://www.openstreetmap.org/user/quicky\">quicky</A> in <A HREF=\"http://www.openstreetmap.org/browse/changeset/13881120\">Changeset 13881120</A><BR>");
    m_text_zone->insertHtml("<A HREF=\"http://www.openstreetmap.org/browse/way/190883118\">Way 190883118</A> has been aligned by <A HREF=\"http://www.openstreetmap.org/user/quicky\">quicky</A> in <A HREF=\"http://www.openstreetmap.org/browse/changeset/13881140\">Changeset 13881140</A><BR>");
  m_text_zone->insertHtml("</body>");
  m_text_zone->insertHtml("</html>");
    l_layout->addWidget(m_text_zone);
  }


  //----------------------------------------------------------------------------
  void run_control_widget::treat_start_event()
  {
    std::cout << "Run_control_widget::Receive start event" << std::endl ;
    configuration_dialog_widget l_dialog(this);
    l_dialog.exec();
  }

  //----------------------------------------------------------------------------
  void run_control_widget::treat_stop_event()
  {
    std::cout << "Run_control_widget::Receive stop event" << std::endl ;
  }

}
//EOF

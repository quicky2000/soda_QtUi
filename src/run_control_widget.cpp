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


#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QThread>
#include <QLCDNumber>
#include <QColorGroup>
#include <iostream>
#include <QGroupBox>
#include <QDesktopServices>
#include <QMessageBox>

namespace soda_QtUi
{
  //----------------------------------------------------------------------------
  run_control_widget::run_control_widget(QWidget * parent):
    QWidget(parent),
    m_configure_button(NULL),
    m_start_button(NULL),
    m_stop_button(NULL),
    m_current_domain(NULL),
    m_current_seq_number(NULL),
    m_current_date_time(NULL),
    m_tab_widget(NULL),
    m_text_zone(NULL),
    m_soda_thread(NULL),
    m_common_text("")
  {
    QVBoxLayout * l_layout = new QVBoxLayout(this);
    QHBoxLayout * l_button_layout = new QHBoxLayout();
    l_layout->addLayout(l_button_layout);

    m_configure_button = new QPushButton(tr("Configure"));
    connect(m_configure_button,SIGNAL(clicked()),this,SLOT(treat_config_event()));
    l_button_layout->addWidget(m_configure_button);

    m_start_button = new QPushButton(tr("Start"));
    connect(m_start_button,SIGNAL(clicked()),this,SLOT(treat_start_event()));
    l_button_layout->addWidget(m_start_button);

    m_stop_button = new QPushButton(tr("Stop"));
    m_stop_button->setEnabled(false);
    connect(m_stop_button,SIGNAL(clicked()),this,SLOT(treat_stop_event()));
    l_button_layout->addWidget(m_stop_button);

    m_progress_bar = new QProgressBar();
    m_progress_bar->setVisible(false);
    m_progress_bar->setTextVisible(true);
    l_layout->addWidget(m_progress_bar);

    QGroupBox * l_analyzed_diff_box = new QGroupBox("Analyzed diff");
    l_layout->addWidget(l_analyzed_diff_box);

    QHBoxLayout * l_current_diff_layout = new QHBoxLayout();
    l_analyzed_diff_box->setLayout(l_current_diff_layout);

    m_current_domain = new QLineEdit();
    m_current_domain->setReadOnly(true);
    l_current_diff_layout->addWidget(m_current_domain);

    m_current_seq_number = new QLCDNumber();
    m_current_seq_number->setDigitCount(9);
    l_current_diff_layout->addWidget(m_current_seq_number);

    m_current_date_time = new QDateTimeEdit();
    m_current_date_time->setReadOnly(true);
    m_current_date_time->setDisplayFormat("dd/MM/yyyy hh:mm");
    l_current_diff_layout->addWidget(m_current_date_time);

    m_tab_widget = new QTabWidget();
    l_layout->addWidget(m_tab_widget);

    //m_text_zone = new QWebView();
    m_text_zone = new QTextBrowser();
    m_text_zone->setOpenLinks(false);
    m_text_zone->setReadOnly(true);
    connect(m_text_zone,SIGNAL(anchorClicked(QUrl)),this,SLOT(anchor_clicked(QUrl)));

    m_tab_widget->addTab(m_text_zone,"General");
  }

  //----------------------------------------------------------------------------
  void run_control_widget::manage_start_button(bool p_enabled)
  {
    m_start_button->setEnabled(p_enabled);
  }

  //----------------------------------------------------------------------------
  void run_control_widget::treat_start_event()
  {
    std::cout << "Run_control_widget::Receive start event" << std::endl ;
    m_start_button->setEnabled(false);
    m_configure_button->setEnabled(false);

    m_module_widgets_by_pointer.clear();

    m_soda_thread = new soda_thread();

    // Connect thread controler slots to GUI slots
    connect(m_soda_thread,SIGNAL(display_status_message(const QString & )),this,SLOT(display_status_message(const QString &)));
    connect(m_soda_thread,SIGNAL(sig_append_common_text(const QString & )),this,SLOT(append_common_text(const QString &)));
    connect(m_soda_thread,SIGNAL(sig_clear_common_text()),this,SLOT(clear_common_text()));
    connect(m_soda_thread,SIGNAL(sig_update_diff_state(unsigned long long ,QString,QString)),this,SLOT(update_diff_state(unsigned long long ,QString,QString)));
    connect(m_soda_thread,SIGNAL(sig_instantiate_module(QString,const void*)),this,SLOT(instantiate_module(QString,const void*)));
    connect(m_soda_thread,SIGNAL(sig_configure_progress_display(uint,uint)),this,SLOT(configure_progress_display(uint,uint)));
    connect(m_soda_thread,SIGNAL(sig_update_progress(uint)),this,SLOT(update_progress(uint)));
    connect(m_soda_thread,SIGNAL(sig_append_text(const void*,QString)),this,SLOT(append_text(const void*,QString)));
    connect(m_soda_thread,SIGNAL(sig_declare_html_report(const void*,QString)),this,SLOT(declare_html_report(const void*,QString)));
    connect(m_soda_thread,SIGNAL(error_received(const QString &,const QString &)),this,SLOT(display_error_message(const QString &,const QString &)));

    connect(m_tab_widget,SIGNAL(currentChanged(int)),this,SLOT(module_tab_change(int)));

    QThread *soda_threadThread = new QThread(this);

    connect(soda_threadThread, SIGNAL(started()), m_soda_thread, SLOT(doWork()));
    connect(m_soda_thread,SIGNAL(end_of_execution()),soda_threadThread,SLOT(quit()));

    connect(soda_threadThread, SIGNAL(finished()), this, SLOT(end_of_thread_event()));
    connect(soda_threadThread, SIGNAL(finished()), m_soda_thread, SLOT(deleteLater()));
    connect(soda_threadThread, SIGNAL(finished()), soda_threadThread, SLOT(deleteLater()));

    //  connect(m_soda_thread, SIGNAL(end_of_execution()), this, SLOT(end_of_thread()));
    m_soda_thread->moveToThread(soda_threadThread);

    // Starts an event loop, and emits soda_threadThread->started()
    soda_threadThread->start();

    m_stop_button->setEnabled(true);
    std::cout << "Run_control_widget::End of Start event" << std::endl ;

  }

  //----------------------------------------------------------------------------
  void run_control_widget::treat_config_event()
  {
    std::cout << "Run_control_widget::Receive config event" << std::endl ;
    emit config_button_pressed();
  }

  //----------------------------------------------------------------------------
  void run_control_widget::treat_stop_event()
  {
    std::cout << "Run_control_widget::Receive stop event" << std::endl ;
    m_stop_button->setEnabled(false);
    m_soda_thread->stop_work();
  }

  //------------------------------------------------------------------------------
  void run_control_widget::end_of_thread_event()
  {
//    m_text_zone->setHtml("Thread terminated");
    m_stop_button->setEnabled(false);
    //  delete m_soda_thread;
    m_soda_thread = NULL;
    m_start_button->setEnabled(true);
    m_configure_button->setEnabled(true);
  }

  //------------------------------------------------------------------------------
  void run_control_widget::display_status_message(const QString & p_text)
  {
      m_text_zone->append(p_text);
//      m_text_zone->setHtml(p_text);
  }

  //------------------------------------------------------------------------------
  void run_control_widget::clear_common_text()
  {
      m_common_text = "";
      m_text_zone->clear();
  }

  //------------------------------------------------------------------------------
  void run_control_widget::append_common_text(const QString & p_text)
  {
   //   m_common_text += p_text + "<BR>";
      m_text_zone->append(p_text);
   //m_text_zone->setHtml(m_common_text);
  }

  //------------------------------------------------------------------------------
  void run_control_widget::update_diff_state(const unsigned long long & p_seq_number,const QString & p_timestamp, const QString & p_replication_domain)
  {
    m_current_seq_number->display((int)p_seq_number);
    m_current_domain->setText(p_replication_domain);
    std::string l_timestamp(p_timestamp.toStdString());
    int l_year = atoi(l_timestamp.substr(0,4).c_str());
    int l_mon  = atoi(l_timestamp.substr(5,2).c_str()) ;
    int l_day = atoi(l_timestamp.substr(8,2).c_str()) ;
    int l_hour = atoi(l_timestamp.substr(11,2).c_str());
    int l_min  = atoi(l_timestamp.substr(14,2).c_str());
    int l_sec = atoi(l_timestamp.substr(17,2).c_str());
    m_current_date_time->setDateTime(QDateTime(QDate(l_year,l_mon,l_day),QTime(l_hour,l_min,l_sec)));
  }

  void run_control_widget::instantiate_module(const QString & p_name,const void * p_module)
  {
      std::map<std::string,module_display_widget*>::iterator l_iter = m_module_widgets_by_name.find(p_name.toStdString());

      // Remove existing tab with this name
      if(m_module_widgets_by_name.end() != l_iter)
      {
          m_tab_widget->removeTab(m_tab_widget->indexOf(l_iter->second));
          m_module_widgets_by_name.erase(l_iter);
      }
      module_display_widget * l_widget = new module_display_widget();
      m_module_widgets_by_pointer.insert(std::map<const void*,module_display_widget*>::value_type(p_module,l_widget));
      m_module_widgets_by_name.insert(std::map<std::string,module_display_widget*>::value_type(p_name.toStdString(),l_widget));
      m_tab_widget->addTab(l_widget,p_name);
  }

  //------------------------------------------------------------------------------
  void run_control_widget::configure_progress_display(const unsigned int & p_min,const unsigned int & p_max)
  {
      m_progress_bar->setVisible(true);
      m_progress_bar->setMinimum(p_min);
      m_progress_bar->setMaximum(p_max);
  }

  //------------------------------------------------------------------------------
  void run_control_widget::update_progress(const unsigned int & p_value)
  {
      m_progress_bar->setValue(p_value);
  }
  //------------------------------------------------------------------------------
  void run_control_widget::append_text(const void * p_module,const QString & p_text)
  {
      std::map<const void*,module_display_widget*>::iterator l_iter =  m_module_widgets_by_pointer.find(p_module);
      if(m_module_widgets_by_pointer.end() != l_iter)
	{
	  l_iter->second->append_text(p_text);
	}
      else
	{
	  if(m_error_unregistered.end() == m_error_unregistered.find(p_module))
	    {
	      m_error_unregistered.insert(p_module);
	      display_error_message(tr("Erreur de module"),tr("Un module non enregsitre tente d afficher un message, contactez le developpeur du module"));
	    }
	}
  }

  //------------------------------------------------------------------------------
  void run_control_widget::declare_html_report(const void * p_module,const QString & p_name)
  {
    std::map<const void*,module_display_widget*>::iterator l_iter =  m_module_widgets_by_pointer.find(p_module);
    if(m_module_widgets_by_pointer.end() != l_iter)
      {
	l_iter->second->declare_html_report(p_name);
      }
    else
      {
	  if(m_error_unregistered.end() == m_error_unregistered.find(p_module))
	    {
	      m_error_unregistered.insert(p_module);
	      display_error_message(tr("Erreur de module"),tr("Un module non enregsitre tente d enregistrer son rapport HTML, contactez le developpeur du module"));
	    }
      }
  }

  //------------------------------------------------------------------------------
  void run_control_widget::module_tab_change(int p_index)
  {
      if(m_tab_widget->tabText(p_index) != "General")
      {
          module_display_widget * l_module_display_widget = static_cast<module_display_widget*>(m_tab_widget->widget(p_index));
          l_module_display_widget->refresh_report();
      }
  }

  //------------------------------------------------------------------------------
  void run_control_widget::anchor_clicked(const QUrl & p_name)
  {
    QDesktopServices::openUrl(p_name);
  }

  //------------------------------------------------------------------------------
  void run_control_widget::display_error_message(const QString & p_title, const QString & p_text)
  {
    QMessageBox::critical(this,p_title,p_text, QMessageBox::Ok,QMessageBox::Ok);
  }


}
//EOF

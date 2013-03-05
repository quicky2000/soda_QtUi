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
#include "main_window.h"
#include <QAction>
#include <QMenu>
#include <QLabel>
#include <QMessageBox>
#include <QStatusBar>
#include <QCloseEvent>
#include <QMenuBar>
#include <QDialog>
#include <QThread>
#include <QScrollArea>
#include <QFileDialog>
#include <iostream>
#include <cassert>
#include "soda_thread.h"
#include "configuration_widget.h"
#include <fstream>

using namespace std;

namespace soda_QtUi
{

  //------------------------------------------------------------------------------
  main_window::main_window(void):
    m_exit_action(NULL),
    m_file_menu(NULL),
    m_status_label(NULL),
    m_tab_widget(NULL),
    m_config_file_name(""),
    m_config_modified(false),
    m_url_reader(quicky_url_reader::url_reader::instance())
  {
    set_title();
    create_actions();
    create_menus();
    create_status_bar();


    m_configuration_widget = new configuration_widget();
    connect(m_configuration_widget,SIGNAL(config_modified()),this,SLOT(treat_config_modified_event()));

    m_configuration_dialog = new configuration_dialog_widget(m_configuration_widget,this);
    connect(m_configuration_dialog,SIGNAL(finished(int)),this,SLOT(treat_dialog_closed_event()));
    m_run_control_widget = new run_control_widget();
    connect(m_run_control_widget,SIGNAL(config_button_pressed()),m_configuration_dialog,SLOT(exec()));

    setCentralWidget(m_run_control_widget);

    ifstream l_tmp_config_file;
    l_tmp_config_file.open("tmp.conf");
    if(l_tmp_config_file.is_open())
      {
	try
	  {
	    m_configuration_widget->load_configuration_file("tmp.conf");
	    m_run_control_widget->append_common_text("Load previously used configuration");
	    m_run_control_widget->manage_start_button(true);
	    l_tmp_config_file.close();
	    m_config_modified = false;
	    set_title();
	  }
	catch(quicky_exception::quicky_runtime_exception & e)
	  {
	    std::stringstream l_stream;
#ifdef SODA_QTUI_DEBUG
	    l_stream << " from line " << e.get_line() << " in file " << e.get_file();
#endif
	    m_run_control_widget->display_error_message("Runtime exception",QString(e.what())+QString(l_stream.str().c_str()));
	  }
	catch(quicky_exception::quicky_logic_exception & e)
	  {
	    std::stringstream l_stream;
#ifdef SODA_QTUI_DEBUG
	    l_stream << " from line " << e.get_line() << " in file " << e.get_file();
#endif
	     m_run_control_widget->display_error_message("Logic exception",QString(e.what())+QString(l_stream.str().c_str()));
	  }
	catch(std::logic_error & e)
	  {
	     m_run_control_widget->display_error_message("Logic error",e.what());
	  }
	catch(std::runtime_error & e)
	  {
	     m_run_control_widget->display_error_message("Runtime error",e.what());
	  }
      }
    else
      {
        m_run_control_widget->append_common_text("Configure to be able to start");
	m_run_control_widget->manage_start_button(false);
      }
    setMinimumSize(160, 160);
    resize(1024, 768);
  }

  //----------------------------------------------------------------
  void main_window::treat_dialog_closed_event()
  {
    cout << "QtEvent::Dialog closed" << endl ;
    m_configuration_widget->save_configuration_file("tmp.conf");
    m_run_control_widget->append_common_text("Configuration done.");
    m_run_control_widget->manage_start_button(true);
  }
  //----------------------------------------------------------------
  void main_window::set_title(void)
  {
    std::string l_title = (m_config_file_name == "" ? "SODA" : m_config_file_name);
    if(m_config_modified)
      {
	l_title += "*";
      }
    setWindowTitle(l_title.c_str());
  }

  //---------------------------------------------------
  // Methods inherited from fichier_client_UI_if class
  //---------------------------------------------------
  // Comunication with user
  //------------------------------------------------------------------------------
  //------------------------------------------------------------------------------
  void main_window::display_status_message(const QString & p_text)
  {
    m_status_label->setText(p_text);
  }

  //------------------------------------------------------------------------------
  bool main_window::ask_yes_no_qestion(const std::string & p_title, const std::string & p_question)
  {
    int l_result = QMessageBox::question(this,
					 p_title.c_str(),
					 p_question.c_str(),
					 QMessageBox::Yes | QMessageBox::Default,
					 QMessageBox::No);
    return l_result == QMessageBox::Yes;
  }

  //------------------------------------------------------------------------------
  void main_window::exit(void)
  {
    cout << "QtEvent::Exit" << endl ;
    close();
  }

  //------------------------------------------------------------------------------
  void main_window::treat_open_event(void)
  {
    cout << "QtEvent::Open" << endl ;
    QString l_file_name = QFileDialog::getOpenFileName(this,
                                                       tr("Open configuration file"),
#ifdef _WIN32
						       "*",
                                                       tr("Configuration file (*.conf)")
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
	    try
	      {
		m_config_file_name = l_file_name_std;
		m_configuration_widget->load_configuration_file(l_file_name_std);
		m_config_modified = false;
		set_title();
		m_close_config_action->setEnabled(true);
		m_open_action->setEnabled(false);
		m_save_action->setEnabled(false);
		m_save_as_action->setEnabled(true);
		m_configuration_widget->save_configuration_file("tmp.conf");
		m_run_control_widget->append_common_text("Configuration loaded");
		m_run_control_widget->manage_start_button(true);
	      }
	    catch(quicky_exception::quicky_runtime_exception & e)
	      {
		std::stringstream l_stream;
#ifdef SODA_QTUI_DEBUG
		l_stream << " from line " << e.get_line() << " in file " << e.get_file();
#endif
		m_run_control_widget->display_error_message("Runtime exception",QString(e.what())+QString(l_stream.str().c_str()));
	      }
	    catch(quicky_exception::quicky_logic_exception & e)
	      {
		std::stringstream l_stream;
#ifdef SODA_QTUI_DEBUG
		l_stream << " from line " << e.get_line() << " in file " << e.get_file();
#endif
		m_run_control_widget->display_error_message("Logic exception",QString(e.what())+QString(l_stream.str().c_str()));
	      }
	    catch(std::logic_error & e)
	      {
		m_run_control_widget->display_error_message("Logic error",e.what());
	      }
	    catch(std::runtime_error & e)
	      {
		m_run_control_widget->display_error_message("Runtime error",e.what());
	      }

          }
      }

  }

  //------------------------------------------------------------------------------
  void main_window::treat_close_config_event()
  {
    bool l_close = true;
    if(m_config_modified)
      {
	l_close = ask_yes_no_qestion(tr("Close configuration").toStdString(),tr("You have pending modifications, do you really want to close the configuration ?").toStdString());
      }
    if(l_close)
      {
        m_configuration_widget->clear();
        m_config_file_name = "";
        m_config_modified = false;
        set_title();
        m_open_action->setEnabled(true);
        m_close_config_action->setEnabled(false);
        m_save_action->setEnabled(false);
        m_save_as_action->setEnabled(false);
      }
  }
  //------------------------------------------------------------------------------
  void main_window::treat_save_event()
  {
    cout << "QtEvent::Save" << endl ;
    assert(m_config_file_name != "");
    m_configuration_widget->save_configuration_file(m_config_file_name);
    m_save_action->setEnabled(false);
    m_config_modified = false;
    set_title();
  }

  //------------------------------------------------------------------------------
  void main_window::treat_save_as_event()
  {
    cout << "QtEvent::Save as" << endl ;
    std::string l_file_name = QFileDialog::getSaveFileName(this,tr("Save configuration file")).toStdString();
    if(l_file_name != "")
      {
#ifndef _WIN32
	std::ifstream l_check_file;
	l_check_file.open(l_file_name.c_str());
	bool l_write = true;
	if(l_check_file.is_open())
          {
	    l_check_file.close();
	    l_write = ask_yes_no_qestion(tr("Overwrite file ?").toStdString(),tr("The file already exists. Overwrite ?").toStdString());
          }
	if(l_write)
          {
#endif
            m_config_file_name = l_file_name;
            treat_save_event();
            set_title();
#ifndef _WIN32
          }
#endif
      }
  }

  //------------------------------------------------------------------------------
  void main_window::closeEvent(QCloseEvent *event)
  {
    string l_question("Are you sure want to quit ?");
    if(m_config_modified)
      {
        l_question += "\nYou have pending modifications !\n";
      }
    int l_result = QMessageBox::question(this, tr("Quit"),
					 tr(l_question.c_str()),
					 QMessageBox::Yes | QMessageBox::Default,
					 QMessageBox::No);
    if (l_result == QMessageBox::Yes)
      {
	event->accept();
      }
    else
      {
	event->ignore();
      }
  }

  //------------------------------------------------------------------------------
  void main_window::create_actions(void)
  {
    m_open_action = new QAction(tr("&Load configuration"),this);
    m_open_action->setShortcut(tr("Ctrl+O"));
    m_open_action->setStatusTip(tr("Load configuration file"));
    connect(m_open_action,SIGNAL(triggered()),this,SLOT(treat_open_event()));

    m_save_action = new QAction(tr("&Save configuration"),this);
    m_save_action->setShortcut(tr("Ctrl+S"));
    m_save_action->setStatusTip(tr("Save current configuration file"));
    m_save_action->setEnabled(false);
    connect(m_save_action,SIGNAL(triggered()),this,SLOT(treat_save_event()));

    m_save_as_action = new QAction(tr("Save configuration as"),this);
    m_save_as_action->setShortcut(tr("Ctrl+W"));
    m_save_as_action->setStatusTip(tr("Save configuration file"));
    connect(m_save_as_action,SIGNAL(triggered()),this,SLOT(treat_save_as_event()));

    m_close_config_action = new QAction(tr("&Close configuration"),this);
    m_close_config_action->setShortcut(tr("Ctrl+C"));
    m_close_config_action->setStatusTip(tr("Close configuration file"));
    m_close_config_action->setEnabled(false);
    connect(m_close_config_action,SIGNAL(triggered()),this,SLOT(treat_close_config_event()));

    m_exit_action = new QAction(tr("&Quit"),this);
    m_exit_action->setShortcut(tr("Ctrl+Q"));
    m_exit_action->setStatusTip(tr("Quit the application")); 
    connect(m_exit_action,SIGNAL(triggered()),this,SLOT(exit()));
  }

  //------------------------------------------------------------------------------
  void main_window::create_menus(void)
  {
    m_file_menu = menuBar()->addMenu(tr("&File"));
    m_file_menu->addAction(m_open_action);
    m_file_menu->addAction(m_save_action);
    m_file_menu->addAction(m_save_as_action);
    m_file_menu->addAction(m_close_config_action);
    m_file_menu->addAction(m_exit_action);

    // Manage action activation
    m_exit_action->setEnabled(true);
  }

  //------------------------------------------------------------------------------
  void main_window::create_status_bar(void)
  {
    m_status_label = new QLabel("Status bar");
    statusBar()->addWidget(m_status_label);
  }

  //------------------------------------------------------------------------------
  void main_window::treat_config_modified_event()
  {
    m_save_action->setEnabled(m_config_file_name != "");
    m_config_modified = true;
    set_title();
  }

}
//EOF

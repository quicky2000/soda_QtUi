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
#include "run_control_widget.h"
#include <fstream>

using namespace std;

namespace soda_QtUi
{

  //------------------------------------------------------------------------------
  main_window::main_window(void):
    m_exit_action(NULL),
    m_launch_thread_action(NULL),
    m_stop_thread_action(NULL),
    m_file_menu(NULL),
    m_status_label(NULL),
    m_tab_widget(NULL),
    m_soda_thread(NULL),
    m_config_file_name(""),
    m_config_modified(false)
  {
    set_title();
    create_actions();
    create_menus();
    create_status_bar();

    m_tab_widget = new QTabWidget();
    connect(m_tab_widget,SIGNAL(currentChanged(int)),this, SLOT(treat_tab_changed_event(int)));

    QScrollArea * l_scroll_area = new QScrollArea();
    m_configuration_widget = new configuration_widget();
    connect(m_configuration_widget,SIGNAL(config_modified()),this,SLOT(treat_config_modified_event()));
    l_scroll_area->setWidget(m_configuration_widget);

    m_tab_widget->addTab(l_scroll_area,tr("Configuration"));
    //   m_tab_widget->addTab(new run_control_widget(),tr("Run"));
    setCentralWidget(m_tab_widget);

    setMinimumSize(160, 160);
    resize(1024, 768);
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
  void main_window::end_of_thread()
  {
    display_status_message("Thread terminated");
    m_stop_thread_action->setEnabled(false);
    //  delete m_soda_thread;
    m_soda_thread = NULL;
    m_launch_thread_action->setEnabled(true);
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
  void main_window::treat_tab_changed_event(int index)
  {
    std::cout << "QtEvent::tab changed" << std::endl ;
    assert(m_tab_widget);
  }

  //------------------------------------------------------------------------------
  void main_window::exit(void)
  {
    cout << "QtEvent::Exit" << endl ;
    close();
  }

  //------------------------------------------------------------------------------
  void main_window::treat_stop_thread_event(void)
  {
    cout << "QtEvent::Stop_Thread" << endl ;
    m_launch_thread_action->setEnabled(true);
    m_soda_thread->stop_work();
    m_stop_thread_action->setEnabled(false);
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
            m_config_file_name = l_file_name_std;
            m_configuration_widget->load_configuration_file(l_file_name_std);
            m_config_modified = false;
            set_title();
            m_close_config_action->setEnabled(true);
            m_open_action->setEnabled(false);
            m_save_action->setEnabled(false);
            m_save_as_action->setEnabled(true);
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
  void main_window::treat_launch_thread_event(void)
  {
    cout << "QtEvent::Launch_Thread" << endl ;
    m_launch_thread_action->setEnabled(false);

    m_soda_thread = new soda_thread();
    connect(m_soda_thread,SIGNAL(display_status_message(const QString & )),this,SLOT(display_status_message(const QString &)));
    QThread *soda_threadThread = new QThread(this);

    connect(soda_threadThread, SIGNAL(started()), m_soda_thread, SLOT(doWork()));
    connect(m_soda_thread,SIGNAL(end_of_execution()),soda_threadThread,SLOT(quit()));

    connect(soda_threadThread, SIGNAL(finished()), this, SLOT(end_of_thread()));
    connect(soda_threadThread, SIGNAL(finished()), m_soda_thread, SLOT(deleteLater()));
    connect(soda_threadThread, SIGNAL(finished()), soda_threadThread, SLOT(deleteLater()));

    //  connect(m_soda_thread, SIGNAL(end_of_execution()), this, SLOT(end_of_thread()));
    m_soda_thread->moveToThread(soda_threadThread);

    // Starts an event loop, and emits soda_threadThread->started()
    soda_threadThread->start();

    m_stop_thread_action->setEnabled(true);
    std::cout << "End of Launch Thread event" << std::endl ;
  }

  //------------------------------------------------------------------------------
  void main_window::closeEvent(QCloseEvent *event)
  {
    string l_question("Are you sure want to quit ?");
    if(m_config_modified)
    {
        l_question += "\nYou have pending modifications !\n";
    }
    if(m_soda_thread)
      {
	l_question += "\nSome thread are still running !\n";
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
    m_launch_thread_action = new QAction(tr("&Launch Thread"),this);
    m_launch_thread_action->setShortcut(tr("Ctrl+L"));
    m_launch_thread_action->setStatusTip(tr("Launch Thread")); 
    connect(m_launch_thread_action,SIGNAL(triggered()),this,SLOT(treat_launch_thread_event()));

    m_open_action = new QAction(tr("&Open"),this);
    m_open_action->setShortcut(tr("Ctrl+O"));
    m_open_action->setStatusTip(tr("Open configuration file"));
    connect(m_open_action,SIGNAL(triggered()),this,SLOT(treat_open_event()));

    m_save_action = new QAction(tr("&Save"),this);
    m_save_action->setShortcut(tr("Ctrl+S"));
    m_save_action->setStatusTip(tr("Save current configuration file"));
    m_save_action->setEnabled(false);
    connect(m_save_action,SIGNAL(triggered()),this,SLOT(treat_save_event()));

    m_save_as_action = new QAction(tr("Save As"),this);
    m_save_as_action->setShortcut(tr("Ctrl+W"));
    m_save_as_action->setStatusTip(tr("Save configuration file"));
    connect(m_save_as_action,SIGNAL(triggered()),this,SLOT(treat_save_as_event()));

    m_close_config_action = new QAction(tr("&Close"),this);
    m_close_config_action->setShortcut(tr("Ctrl+C"));
    m_close_config_action->setStatusTip(tr("Close configuration file"));
    m_close_config_action->setEnabled(false);
    connect(m_close_config_action,SIGNAL(triggered()),this,SLOT(treat_close_config_event()));

    m_stop_thread_action = new QAction(tr("&Stop Thread"),this);
    m_stop_thread_action->setShortcut(tr("Ctrl+L"));
    m_stop_thread_action->setStatusTip(tr("Stop Thread")); 
    connect(m_stop_thread_action,SIGNAL(triggered()),this,SLOT(treat_stop_thread_event()));

    m_exit_action = new QAction(tr("&Quit"),this);
    m_exit_action->setShortcut(tr("Ctrl+Q"));
    m_exit_action->setStatusTip(tr("Quit the application")); 
    connect(m_exit_action,SIGNAL(triggered()),this,SLOT(exit()));
  }

  //------------------------------------------------------------------------------
  void main_window::create_menus(void)
  {
    m_file_menu = menuBar()->addMenu(tr("&File"));
    m_file_menu->addAction(m_launch_thread_action);
    m_file_menu->addAction(m_stop_thread_action);
    m_file_menu->addAction(m_open_action);
    m_file_menu->addAction(m_save_action);
    m_file_menu->addAction(m_save_as_action);
    m_file_menu->addAction(m_close_config_action);
    m_file_menu->addAction(m_exit_action);

    // Manage action activation
    m_exit_action->setEnabled(true);
    m_launch_thread_action->setEnabled(true);
    m_stop_thread_action->setEnabled(false);
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

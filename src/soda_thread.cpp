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
#include "soda_thread.h"
#include "quicky_exception.h"
#include <unistd.h>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

#include "osm_diff_watcher.h"

namespace soda_QtUi
{
  //------------------------------------------------------------------------------
  soda_thread::soda_thread(void):
    m_watcher(NULL)
  {
  }

  //------------------------------------------------------------------------------
  void soda_thread::doWork(void)
  {
      std::cout << "soda_thread::start run" << std::endl ;

      try
      {
	m_watcher = new osm_diff_watcher::osm_diff_watcher("tmp.conf",*this);
	m_watcher->run();
      }
      catch(quicky_exception::quicky_runtime_exception & e)
      {
          std::stringstream l_stream;
#ifdef SODA_QTUI_DEBUG
         l_stream << " from line " << e.get_line() << " in file " << e.get_file();
#endif
          emit error_received("Runtime exception",QString(e.what())+QString(l_stream.str().c_str()));
      }
      catch(quicky_exception::quicky_logic_exception & e)
      {
          std::stringstream l_stream;
#ifdef SODA_QTUI_DEBUG
          l_stream << " from line " << e.get_line() << " in file " << e.get_file();
#endif
          emit error_received("Logic exception",QString(e.what())+QString(l_stream.str().c_str()));
      }
      catch(std::logic_error & e)
      {
          emit error_received("Logic error",e.what());
      }
      catch(std::runtime_error & e)
      {
          emit error_received("Runtime error",e.what());
      }

      delete m_watcher;
      m_watcher = NULL;

   std::cout << "soda_thread::End run" << std::endl ;
   emit end_of_execution();
  }

  // General communication
  //----------------------------------------------------------------------------
  void soda_thread::clear_common_text()
  {
      emit sig_clear_common_text();
  }

  //----------------------------------------------------------------------------
  void soda_thread::append_common_text(const std::string & p_text)
  {
      emit sig_append_common_text(QString(p_text.c_str()));
  }

  //----------------------------------------------------------------------------
  void soda_thread::update_diff_state(const osm_diff_analyzer_if::osm_diff_state & p_diff_state)
  {
    emit sig_update_diff_state(p_diff_state.get_sequence_number(),QString(p_diff_state.get_timestamp().c_str()),QString(p_diff_state.get_replication_domain().c_str()));
  }

  //----------------------------------------------------------------------------
  void soda_thread::instantiate_module(const std::string & p_name,const osm_diff_analyzer_if::analyzer_base & p_module)
  {
    emit sig_instantiate_module(QString(p_name.c_str()),&p_module);
  }

  //----------------------------------------------------------------------------
  void soda_thread::configure_progress_display(const unsigned int & p_min,const unsigned int & p_max)
  {
      emit sig_configure_progress_display(p_min,p_max);
  }

  //----------------------------------------------------------------------------
  void soda_thread::update_progress(const unsigned int & p_value)
  {
      emit sig_update_progress(p_value);
  }

  // Module communication
  //----------------------------------------------------------------------------
  void soda_thread::append_log_text(const osm_diff_analyzer_if::analyzer_base & p_module,
				    const std::string & p_text)
  {
    emit sig_append_text(&p_module,QString(p_text.c_str()));
  }

  //----------------------------------------------------------------------------
  void soda_thread::declare_html_report(const osm_diff_analyzer_if::analyzer_base & p_module,const std::string & p_name)
  {
      emit sig_declare_html_report(&p_module,QString(p_name.c_str()));
  }

  //------------------------------------------------------------------------------
  void soda_thread::stop_work(void)
  {
    std::cout << "soda_thread::stop request received from user" << std::endl ;
    if(m_watcher)
      {
	m_watcher->stop();
      }
  }
}
//EOF


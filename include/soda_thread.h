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
#ifndef _SODA_THREAD_H_
#define _SODA_THREAD_H_

#include "soda_Ui_if.h"
#include <QObject>

namespace osm_diff_watcher
{
    class osm_diff_watcher;
}
namespace soda_QtUi
{

  class soda_thread : public QObject, public osm_diff_watcher::soda_Ui_if
  {
    Q_OBJECT
      public:
    soda_thread(void);
    void stop_work(void);
    // Methods inherited from soda_Ui_if
    // General communication
    void clear_common_text();
    void append_common_text(const std::string & p_text);
    void update_diff_state(const osm_diff_analyzer_if::osm_diff_state & p_diff_state);
    void instantiate_module(const std::string & p_name,const osm_diff_analyzer_if::analyzer_base & p_module);
    void configure_progress_display(const unsigned int & p_min,const unsigned int & p_max);
    void update_progress(const unsigned int & p_value);
    // Module communication
    void append_log_text(const osm_diff_analyzer_if::analyzer_base & p_module,const std::string & p_text);
    void declare_html_report(const osm_diff_analyzer_if::analyzer_base & p_module,const std::string & p_name);
    // Ends of methods inherited from soda_Ui_if

  signals:
    void display_status_message(const QString & p_text);
    void error_received(const QString & p_title, const QString & p_text);
    void end_of_execution();
    // General communication
    void sig_clear_common_text();
    void sig_append_common_text(const QString & p_text);
    void sig_update_diff_state(const unsigned long long & p_seq_number,const QString & p_timestamp, const QString & p_replication_domain);
    void sig_instantiate_module(const QString & p_name,const void * p_module);
    void sig_configure_progress_display(const unsigned int & p_min,const unsigned int & p_max);
    void sig_update_progress(const unsigned int & p_value);
    // Module communication
    void sig_append_text(const void * p_module,const QString & p_text);
    void sig_declare_html_report(const void * p_module,const QString & p_name);
    public slots:
    void doWork(void);
  private:
    osm_diff_watcher::osm_diff_watcher * m_watcher;
  };
}
#endif //  _SODA_THREAD_H_
//EOF

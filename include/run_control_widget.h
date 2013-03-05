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
#ifndef RUN_CONTROL_WIDGET_H
#define RUN_CONTROL_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QTextBrowser>
#include <QLCDNumber>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QProgressBar>
#include <QTabWidget>
#include "module_display_widget.h"
#include "configuration_dialog_widget.h"
#include "soda_thread.h"
#include <set>

namespace soda_QtUi
{
  class run_control_widget : public QWidget
  {
    Q_OBJECT
      public:
    run_control_widget(QWidget * parent = NULL);
    void manage_start_button(bool p_enabled);

  signals:
    void config_button_pressed();
  public slots:
    void append_common_text(const QString & p_text);

    public slots:
    void display_error_message(const QString & p_title, const QString & p_text);
    private slots:
    void treat_config_event();
    void treat_start_event();
    void treat_stop_event();
    void end_of_thread_event();
    void module_tab_change(int);
    void display_status_message(const QString & p_text);
    void anchor_clicked(const QUrl & p_name);
    // General communication
    void clear_common_text();
    void instantiate_module(const QString & p_name,const void * p_module);
    void update_diff_state(const unsigned long long & p_seq_number,const QString & p_timestamp, const QString & p_replication_domain);
    void configure_progress_display(const unsigned int & p_min,const unsigned int & p_max);
    void update_progress(const unsigned int & p_value);
    // Module communication
    void append_text(const void * p_module,const QString & p_text);
    void declare_html_report(const void * p_module,const QString & p_name);
  private:
    QPushButton * m_configure_button;
    QPushButton * m_start_button;
    QPushButton * m_stop_button;

    QProgressBar *m_progress_bar;

    QLineEdit * m_current_domain;
    QLCDNumber * m_current_seq_number;
    QDateTimeEdit * m_current_date_time;

    QTabWidget * m_tab_widget;
//    QWebView * m_text_zone;
    QTextBrowser * m_text_zone;
    std::map<std::string,module_display_widget*> m_module_widgets_by_name;
    std::map<const void*,module_display_widget*> m_module_widgets_by_pointer;
    std::set<const void*> m_error_unregistered;

    soda_thread * m_soda_thread;
    QString m_common_text;
  };
}
#endif // RUN_CONTROL_WIDGET_H
//EOF

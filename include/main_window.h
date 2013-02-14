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
#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <QMainWindow>

class QAction;
class QMenu;
class QLabel;
class QTabWidget;

namespace soda_QtUi
{
  class configuration_widget;
  class soda_thread;

  class main_window : public QMainWindow
  {
    Q_OBJECT

      public:
    main_window(void);

    //---------------------------------------------------
    // Methods inherited from fichier_client_UI_if class
    //---------------------------------------------------
    // Comunication with user
    bool ask_yes_no_qestion(const std::string & p_title, const std::string & p_question);

    private slots:
    void display_status_message(const QString & p_text);
    void end_of_thread();
    //
    void treat_tab_changed_event(int index);
    void treat_launch_thread_event();
    void treat_stop_thread_event();
    void treat_open_event();
    void treat_close_config_event();
    void treat_save_event();
    void treat_save_as_event();
    void treat_config_modified_event();
    void exit();
  

  private:
    void set_title(void);
    void closeEvent(QCloseEvent *event);
    void create_actions(void);
    void create_menus(void);
    void create_status_bar(void);

    configuration_widget * m_configuration_widget;
    QAction * m_exit_action;
    QAction * m_open_action;
    QAction * m_save_action;
    QAction * m_save_as_action;
    QAction * m_close_config_action;
    QAction * m_launch_thread_action;
    QAction * m_stop_thread_action;
    QMenu * m_file_menu;
    QLabel * m_status_label;
    QTabWidget *m_tab_widget;

    soda_thread * m_soda_thread;
    std::string m_config_file_name;
    bool m_config_modified;
  };
}
#endif // _MAIN_WINDOW_H_
//EOF

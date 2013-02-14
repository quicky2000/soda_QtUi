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
#include <QTextEdit>

namespace soda_QtUi
{
  class run_control_widget : public QWidget
  {
    Q_OBJECT
      public:
    run_control_widget(QWidget * parent = NULL);
  signals:
    private slots:
    void treat_start_event();
    void treat_stop_event();
  private:
    QPushButton * m_configure_button;
    QPushButton * m_start_button;
    QPushButton * m_stop_button;
    QTextEdit * m_text_zone;
  };
}
#endif // RUN_CONTROL_WIDGET_H
//EOF

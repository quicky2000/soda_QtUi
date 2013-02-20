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
#include "configuration_dialog_widget.h"
#include "configuration_widget.h"
#include <QVBoxLayout>

namespace soda_QtUi
{
  //----------------------------------------------------------------------------
  configuration_dialog_widget::configuration_dialog_widget(QWidget * parent):
    QDialog(parent)
  {
    QVBoxLayout * l_layout = new QVBoxLayout(this);
    l_layout->addWidget(new configuration_widget());
  }

}
// EOF
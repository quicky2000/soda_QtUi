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
#include <unistd.h>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace soda_QtUi
{
  //------------------------------------------------------------------------------
  soda_thread::soda_thread(void):
    m_run(true)
  {
  }

  //------------------------------------------------------------------------------
  void soda_thread::doWork(void)
  {
    std::cout << "soda_thread::start run" << std::endl ;
    for(unsigned int l_index = 0 ;
	l_index < 10 && m_run ;
	++l_index)
      {
	std::cout << "Loop " << l_index << std::endl ;      
	std::stringstream l_stream;
	l_stream << "loop " << l_index;
	emit display_status_message(QString(l_stream.str().c_str()));
#ifndef _WIN32
	sleep(1);
#else
	Sleep(1000*1);
#endif

      }
    std::cout << "soda_thread::End my run" << std::endl ;
    emit end_of_execution();
  }


  //------------------------------------------------------------------------------
  void soda_thread::stop_work(void)
  {
    std::cout << "soda_threadstop received" << std::endl ;
    m_run = false;
  }
}
//EOF


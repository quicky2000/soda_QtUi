/*
    This file is part of osm_diff_analyzer_if, Interface definitions of
    libraries containing diff analyzers
    Copyright (C) 2012  Julien Thevenon ( julien_thevenon at yahoo.fr )

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
#include "module_display_widget.h"
#include <QDesktopServices>

#include <QVBoxLayout>
#include <iostream>

namespace soda_QtUi
{
    module_display_widget::module_display_widget(QWidget * parent):
        QWidget(NULL),
        m_report_name("")
    {
        QVBoxLayout *l_layout = new QVBoxLayout(this);

        m_module_report_label = new QLabel(tr("Module report")+" :");
        m_module_report_label->setVisible(false);
        l_layout->addWidget(m_module_report_label);

        m_report_zone = new QWebView();
        m_report_zone->setVisible(false);
	l_layout->addWidget(m_report_zone);
        m_report_zone->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
        connect(m_report_zone,SIGNAL(linkClicked(QUrl)),this,SLOT(link_clicked(QUrl)));

        l_layout->addWidget(new QLabel(tr("Console log")+" :"));

        m_log_zone = new QTextEdit();
	m_log_zone->setReadOnly(true);
        l_layout->addWidget(m_log_zone);
    }

    void module_display_widget::declare_html_report(const QString &p_name)
    {
        m_report_name = p_name;
        m_report_zone->setVisible(true);
        m_module_report_label->setVisible(true);
        refresh_report();
    }

    void module_display_widget::refresh_report(void)
    {
        if(m_report_name != "")
        {
            m_report_zone->load(m_report_name);
        }
    }

    void module_display_widget::append_text(const QString & p_text)
    {
        m_log_zone->append(p_text);
    }

    //------------------------------------------------------------------------------
    void module_display_widget::link_clicked(const QUrl & p_link)
    {
        std::cout << "link_clicked \"" << p_link.toString().toStdString() << std::endl ;
      QDesktopServices::openUrl(p_link);
    }

}
//EOF

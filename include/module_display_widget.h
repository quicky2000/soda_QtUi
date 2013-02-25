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
#ifndef MODULE_DISPLAY_WIDGET_H
#define MODULE_DISPLAY_WIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QtWebKit/QWebView>
#include <QLabel>

namespace soda_QtUi
{
    class module_display_widget: public QWidget
    {
      Q_OBJECT
    public:
        module_display_widget(QWidget * parent = NULL);
        void append_text(const QString & p_text);
        void declare_html_report(const QString & p_name);
        void refresh_report(void);
    signals:
    private slots:
        void link_clicked(const QUrl & p_link);
    private:
        QLabel * m_module_report_label;
        QWebView * m_report_zone;
        QTextEdit * m_log_zone;
        QString m_report_name;
    };
}
#endif // MODULE_DISPLAY_WIDGET_H

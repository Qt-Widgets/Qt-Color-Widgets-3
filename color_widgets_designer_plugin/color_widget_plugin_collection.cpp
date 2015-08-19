/**

@author Mattia Basaglia

@section License

    Copyright (C) 2013-2015 Mattia Basaglia

    This file is part of Color Widgets.

    Color Widgets is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Color Widgets is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Color Widgets.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "color_widget_plugin_collection.hpp"
#include "color_preview_plugin.hpp"
#include "color_wheel_plugin.hpp"
#include "gradient_slider_plugin.hpp"
#include "hue_slider_plugin.hpp"
#include "color_selector_plugin.hpp"
#include "color_list_plugin.hpp"

ColorWidgets_PluginCollection::ColorWidgets_PluginCollection(QObject *parent) :
    QObject(parent)
{
    widgets.push_back(new ColorPreview_Plugin(this));
    widgets.push_back(new ColorWheel_Plugin(this));
    widgets.push_back(new GradientSlider_Plugin(this));
    widgets.push_back(new HueSlider_Plugin(this));
    widgets.push_back(new ColorSelector_Plugin(this));
    widgets.push_back(new ColorListWidget_Plugin(this));
}

QList<QDesignerCustomWidgetInterface *> ColorWidgets_PluginCollection::customWidgets() const
{
    return widgets;
}

#ifndef FRAMEWORK_QT5
#include <QtCore/qplugin.h>
Q_EXPORT_PLUGIN2(color_widgets, ColorWidgets_PluginCollection)
#endif
/**

@author Mattia Basaglia

@section License

    Copyright (C) 2015 Mattia Basaglia

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Color Widgets.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "color_palette_widget.hpp"
#include "ui_color_palette_widget.h"
#include "color_dialog.hpp"

namespace color_widgets {

class ColorPaletteWidget::Private : public Ui::ColorPaletteWidget
{
public:
    ColorPaletteModel* model = nullptr;
    bool read_only = false;
};


ColorPaletteWidget::ColorPaletteWidget(QWidget* parent)
    : QWidget(parent), p(new Private)
{
    p->setupUi(this);
    connect(p->swatch, &Swatch::colorSizeChanged, this, &ColorPaletteWidget::colorSizeChanged);
    connect(p->swatch, &Swatch::colorSizePolicyChanged, this, &ColorPaletteWidget::colorSizePolicyChanged);
    connect(p->swatch, &Swatch::forcedRowsChanged, this, &ColorPaletteWidget::forcedRowsChanged);
    connect(p->swatch, &Swatch::forcedColumnsChanged, this, &ColorPaletteWidget::forcedColumnsChanged);
    connect(p->swatch, &Swatch::colorSelected, this, &ColorPaletteWidget::currentColorChanged);
    connect(p->button_color_add, &QAbstractButton::clicked, [this](){
        if ( !p->read_only && p->palette_list->currentIndex() != -1 )
        {
            ColorDialog dialog(this);
            dialog.setAlphaEnabled(false);
            if ( p->swatch->selected() != -1 )
                dialog.setColor(p->swatch->selectedColor());
            if ( dialog.exec() )
            {
                p->swatch->palette().appendColor(dialog.color());
                p->swatch->setSelected(p->swatch->palette().count()-1);
            }
        }
    });
    connect(p->button_color_remove, &QAbstractButton::clicked, p->swatch, &Swatch::removeSelected);
    connect(p->button_palette_delete, &QAbstractButton::clicked, [this]() {
        if ( !p->read_only && p->model && p->palette_list->currentIndex() != -1 )
            p->model->removeRow(p->palette_list->currentIndex());
    });
    connect(p->button_palette_save, &QAbstractButton::clicked, [this](){
        if ( !p->read_only && p->model && p->palette_list->currentIndex() != -1 && p->swatch->palette().dirty() )
            if ( p->model->updatePalette( p->palette_list->currentIndex(), p->swatch->palette() ) )
            {
                p->swatch->palette().setDirty(false);
            }
            /// \todo else ask for a file name (?)
    });
}

ColorPaletteWidget::~ColorPaletteWidget() = default;

ColorPaletteModel* ColorPaletteWidget::model() const
{
    return p->model;
}

const ColorPalette& ColorPaletteWidget::currentPalette() const
{
    return p->swatch->palette();
}

QSize ColorPaletteWidget::colorSize() const
{
    return p->swatch->colorSize();
}

Swatch::ColorSizePolicy ColorPaletteWidget::colorSizePolicy() const
{
    return p->swatch->colorSizePolicy();
}

int ColorPaletteWidget::forcedRows() const
{
    return p->swatch->forcedRows();
}
int ColorPaletteWidget::forcedColumns() const
{
    return p->swatch->forcedColumns();
}

bool ColorPaletteWidget::readOnly() const
{
    return p->read_only;
}

QColor ColorPaletteWidget::currentColor() const
{
    return p->swatch->selectedColor();
}

void ColorPaletteWidget::setModel(ColorPaletteModel* model)
{
    if ( model == p->model )
        return;
    p->model = model;
    p->swatch->setPalette(ColorPalette());
    p->palette_list->setModel(model);
}

void ColorPaletteWidget::setColorSize(const QSize& colorSize)
{
    p->swatch->setColorSize(colorSize);
}

void ColorPaletteWidget::setColorSizePolicy(Swatch::ColorSizePolicy colorSizePolicy)
{
    p->swatch->setColorSizePolicy(colorSizePolicy);
}
void ColorPaletteWidget::setForcedRows(int forcedRows)
{
    p->swatch->setForcedRows(forcedRows);
}
void ColorPaletteWidget::setForcedColumns(int forcedColumns)
{
    p->swatch->setForcedColumns(forcedColumns);
}
void ColorPaletteWidget::setReadOnly(bool readOnly)
{
    if ( readOnly == p->read_only )
        return;

    p->swatch->setReadOnly(readOnly);
    p->group_edit_list->setVisible(!readOnly);
    p->group_edit_palette->setVisible(!readOnly);
    emit readOnlyChanged(p->read_only = readOnly);
}

bool ColorPaletteWidget::setCurrentColor(const QColor& color)
{
    const auto& palette = p->swatch->palette();
    for ( int i = 0; i < palette.count(); i++ )
    {
        if ( palette.colorAt(i) == color )
        {
            p->swatch->setSelected(i);
            return true;
        }
    }

    p->swatch->clearSelection();
    return false;
}

bool ColorPaletteWidget::setCurrentColor(const QString& name)
{
    const auto& palette = p->swatch->palette();
    for ( int i = 0; i < palette.count(); i++ )
    {
        if ( palette.nameAt(i) == name )
        {
            p->swatch->setSelected(i);
            return true;
        }
    }

    p->swatch->clearSelection();
    return false;
}

void ColorPaletteWidget::on_palette_list_currentIndexChanged(int index)
{
    if ( !p->model )
        p->swatch->setPalette(ColorPalette());
    else
        p->swatch->setPalette(p->model->palette(index));
}

void ColorPaletteWidget::on_swatch_doubleClicked(int index)
{
    if ( !p->read_only )
    {
        ColorDialog dialog(this);
        dialog.setAlphaEnabled(false);
        dialog.setColor(p->swatch->palette().colorAt(index));
        if ( dialog.exec() )
            p->swatch->palette().setColorAt(index, dialog.color());
    }
}

} // namespace color_widgets

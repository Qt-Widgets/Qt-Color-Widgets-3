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

#ifndef COLOR_WIDGETS_COLOR_PALETTE_HPP
#define COLOR_WIDGETS_COLOR_PALETTE_HPP

#include <QColor>
#include <QString>
#include <QVector>
#include <QObject>
#include <QPair>
#include "colorwidgets_global.hpp"

namespace color_widgets {

class QCP_EXPORT ColorPalette : public QObject
{
    Q_OBJECT

    /**
     * \brief The list of colors
     */
    Q_PROPERTY(QVector<QColor> colors READ colors WRITE setColors NOTIFY colorsChanged)
    /**
     * \brief The list of color names
     */
    Q_PROPERTY(QVector<QString> names READ names NOTIFY namesChanged)
    /**
     * \brief Name of the palette
     */
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    /**
     * \brief Number of colors to display in a row, if -1 unspecified
     */
    Q_PROPERTY(int columns READ columns WRITE setColumns NOTIFY columnsChanged)
    /**
     * \brief Number of rows (based on \c count and \c columns), if -1 unspecified
     */
    Q_PROPERTY(int rows READ rows NOTIFY rowsChanged)
    /**
     * \brief Number of colors
     */
    Q_PROPERTY(int count READ count)
    /**
     * \brief Name of the file the palette has been read from
     */
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)

public:
    ColorPalette(const QVector<QColor>& colors, const QString& name = QString(), int columns = -1);
    ColorPalette(const QVector<QPair<QColor,QString> >& colors, const QString& name = QString(), int columns = -1);
    explicit ColorPalette(const QString& name = QString());
    ColorPalette(const ColorPalette& other);
    ColorPalette& operator=(const ColorPalette& other);
    ~ColorPalette();
#ifdef Q_COMPILER_RVALUE_REFS
    ColorPalette(ColorPalette&& other);
    ColorPalette& operator=(ColorPalette&& other);
#endif // Q_COMPILER_RVALUE_REFS

    /**
     * \brief Color at the given index
     */
    QColor colorAt(int index) const;

    /**
     * \brief Color name at the given index
     */
    QString nameAt(int index) const;

    QVector<QColor> colors() const;
    QVector<QString> names() const;

    int count() const;
    int rows() const;
    int columns();

    QString name() const;

    /**
     * \brief Load a ColorPalette from a Gimp palette (gpl) file
     */
    static ColorPalette fromFile(const QString& name);


    QString fileName() const;

public slots:
    void setColumns(int columns);

    void setColors(const QVector<QColor>& colors);
    void setColors(const QVector<QPair<QColor,QString> >& colors);

    /**
     * \brief Change the color at the given index
     */
    void setColorAt(int index, const QColor& color, const QString& name = QString());
    /**
     * \brief Change the name of a color
     */
    void setNameAt(int index, const QString& name = QString());
    /**
     * \brief Append a color at the end
     */
    void appendColor(const QColor& color, const QString& name = QString());
    /**
     * \brief Insert a color in an arbitrary location
     */
    void insertColor(int index, const QColor& color, const QString& name = QString());
    /**
     * \brief Remove the color at the given index
     */
    void eraseColor(int index);

    /**
     * \brief Change file name and save
     * \returns \b true on success
     */
    bool save(const QString& filename);
    /**
     * \brief save to file, the filename is \c fileName or determined automatically
     * \returns \b true on success
     */
    bool save() const;

    void setName(const QString& name);
    void setFileName(const QString& name);

signals:
    void colorsChanged(const QVector<QColor>&);
    void namesChanged(const QVector<QString>&);
    void columnsChanged(int);
    void rowsChanged(int);
    void nameChanged(const QString&);
    void fileNameChanged(const QString&);

private:
    /**
     * \brief Returns \c name if it isn't null, otherwise a default value
     */
    QString unnamed(const QString& name = QString()) const;

    class Private;
    Private *p;
};

} // namespace color_widgets

#endif // COLOR_WIDGETS_COLOR_PALETTE_HPP
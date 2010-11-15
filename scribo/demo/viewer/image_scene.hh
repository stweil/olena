
// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef   	IMAGE_SCENE_HH_
# define   	IMAGE_SCENE_HH_

# include <QtGui>

class ImageRegion;

class ImageScene
  : public QGraphicsScene
{
  Q_OBJECT

public:
  ImageScene();
  ~ImageScene();

  void mousePressEvent(QGraphicsSceneMouseEvent* event);
  void clear();

signals:
  void selected(QString, QString);
  void deselected();

private:
  ImageRegion* selected_;
  QVector<ImageRegion*> region_vector_;
};

#endif	    /* !IMAGE_SCENE_HH_ */
/*
 * DestinationGLCanvas.cpp
 *
 * (c) 2013 Sofian Audry -- info(@)sofianaudry(.)com
 * (c) 2013 Alexandre Quessy -- alexandre(@)quessy(.)net
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "DestinationGLCanvas.h"
#include "MainWindow.h"

DestinationGLCanvas::DestinationGLCanvas(QWidget* parent, const QGLWidget * shareWidget)
: MapperGLCanvas(parent, shareWidget)
{
}

Shape* DestinationGLCanvas::getShapeFromMappingId(uid mappingId)
{
  if (mappingId == NULL_UID)
    return NULL;
  else
    return MainWindow::getInstance().getMappingManager().getMappingById(mappingId)->getShape().get();
}

void DestinationGLCanvas::doDraw(QPainter* painter)
{
//  // No sources = nothing to do.
//  if (Common::nImages() == 0)
//    return;
//
//  // TODO: Ceci est un hack necessaire car tout est en fonction de la width/height de la texture.
//  // Il faut changer ca.
//  std::tr1::shared_ptr<TextureMapping> textureMapping = std::tr1::static_pointer_cast<TextureMapping>(Common::currentMapping);
//  Q_CHECK_PTR(textureMapping);
//
//  std::tr1::shared_ptr<Texture> texture = std::tr1::static_pointer_cast<Texture>(textureMapping->getPaint());
//  Q_CHECK_PTR(texture);
//
//  for (int i=0; i < Common::nImages(); i++)
//  {
//    std::tr1::shared_ptr<Texture> tex = std::tr1::static_pointer_cast<Texture>(Common::mappings[i]->getPaint());
//    Q_CHECK_PTR(tex);
//
//    // FIXME: maybe the texture id is actually 0 and it's ok, no?
//    // we should use a boolean is_texture_loaded, or so
//    if (tex->getTextureId() == 0)
//    {
//      tex->loadTexture();
//    }
//  }

  glPushMatrix();

  MappingManager& mappingManager = MainWindow::getInstance().getMappingManager();
  QVector<Mapping::ptr> mappings = mappingManager.getVisibleMappings();
  for (QVector<Mapping::ptr>::const_iterator it = mappings.begin(); it != mappings.end(); ++it)
  {
    Mapping::ptr mapping = (*it);

    // XXX: change : UGLY!
    if (mapping->getType().endsWith("_texture"))
    {
      std::tr1::shared_ptr<TextureMapping> textureMapping = std::tr1::static_pointer_cast<TextureMapping>(mapping);
      Q_CHECK_PTR(textureMapping);

      std::tr1::shared_ptr<Texture> texture = std::tr1::static_pointer_cast<Texture>(textureMapping->getPaint());
      Q_CHECK_PTR(texture);

      if (texture->getTextureId() == 0)
        texture->loadTexture();
    }

    // Draw the mappings.
    MainWindow::getInstance().getMapperByMappingId(mapping->getId())->draw(painter);
  }

  // Draw the controls of current mapping.
  if (MainWindow::getInstance().hasCurrentMapping() &&
      getCurrentShape() != NULL)
  {
    MainWindow::getInstance().getMapperByMappingId(MainWindow::getInstance().getCurrentMappingId())->drawControls(painter);
  }

  glPopMatrix();
}


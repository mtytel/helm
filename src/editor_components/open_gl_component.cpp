/* Copyright 2013-2017 Matt Tytel
 *
 * helm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * helm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with helm.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "open_gl_component.h"

void OpenGLComponent::setViewPort(OpenGLContext& open_gl_context) {
  float scale = open_gl_context.getRenderingScale();
  Rectangle<int> top_level = getTopLevelComponent()->getScreenBounds();
  Rectangle<int> local = getScreenBounds();

  glViewport(scale * (local.getX() - top_level.getX()),
             scale * (top_level.getBottom() - local.getBottom()),
             scale * getWidth(), scale * getHeight());
}

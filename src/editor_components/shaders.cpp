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

#include "shaders.h"
#include "JuceHeader.h"

const char* Shaders::getShader(Shader shader) {
  return shaders_[shader];
}

const char* Shaders::shaders_[] = {
  "attribute vec4 position;\n"
  "attribute vec2 tex_coord_in;\n"
  "\n"
  "varying vec2 tex_coord_out;\n"
  "\n"
  "void main()\n"
  "{\n"
  "    tex_coord_out = tex_coord_in;\n"
  "    gl_Position = position;\n"
  "}\n",

  "varying " JUCE_MEDIUMP " vec2 tex_coord_out;\n"
  "\n"
  "uniform sampler2D texture;\n"
  "\n"
  "void main()\n"
  "{\n"
  "    gl_FragColor = texture2D(texture, tex_coord_out);\n"
  "}\n",

  "attribute vec4 position;\n"
  "\n"
  "void main()\n"
  "{\n"
  "    gl_Position = position;\n"
  "}\n",
  
  "void main()\n"
  "{\n"
  "    " JUCE_LOWP " vec4 colour = vec4(0.9, 0.9, 0.9, 1.0);\n"
  "    gl_FragColor = colour;\n"
  "}\n",

  "void main()\n"
  "{\n"
  "    " JUCE_MEDIUMP " vec4 colour1 = vec4 (1.0, 0.4, 0.6, 1.0);\n"
  "    " JUCE_MEDIUMP " vec4 colour2 = vec4 (0.0, 0.8, 0.6, 1.0);\n"
  "    " JUCE_MEDIUMP " float alpha = pixelPos.x / 1000.0;\n"
  "    gl_FragColor = pixelAlpha * mix (colour1, colour2, alpha);\n"
  "}\n",

  "void main()\n"
  "{\n"
  "    " JUCE_MEDIUMP " vec4 colour1 = vec4 (1.0, 0.4, 0.6, 1.0);\n"
  "    " JUCE_MEDIUMP " vec4 colour2 = vec4 (0.0, 0.8, 0.6, 1.0);\n"
  "    " JUCE_MEDIUMP " float alpha = pixelPos.x / 1000.0;\n"
  "    gl_FragColor = pixelAlpha * mix (colour1, colour2, alpha);\n"
  "}\n"};

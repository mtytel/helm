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
  "attribute " JUCE_MEDIUMP " vec4 position;\n"
  "attribute " JUCE_MEDIUMP " vec2 tex_coord_in;\n"
  "\n"
  "varying " JUCE_MEDIUMP " vec2 tex_coord_out;\n"
  "\n"
  "void main()\n"
  "{\n"
  "    tex_coord_out = tex_coord_in;\n"
  "    gl_Position = position;\n"
  "}\n",

  "varying " JUCE_MEDIUMP " vec2 tex_coord_out;\n"
  "\n"
  "uniform sampler2D image;\n"
  "\n"
  "void main()\n"
  "{\n"
  "    gl_FragColor = texture2D(image, tex_coord_out);\n"
  "}\n",

  "attribute " JUCE_MEDIUMP " vec4 position;\n"
  "\n"
  "void main()\n"
  "{\n"
  "    gl_Position = position;\n"
  "}\n",
 
    
  "void main()\n"
  "{\n"
  "    " JUCE_MEDIUMP " vec4 color = vec4(1.10, 1.10, 1.10, 1.0);\n"
  "    gl_FragColor = color;\n"
  "}\n",
     
    
  "attribute " JUCE_MEDIUMP " vec4 position;\n"
  "attribute " JUCE_MEDIUMP " vec2 coordinates;\n"
  "attribute " JUCE_MEDIUMP " vec2 range;\n"
  "\n"
  "varying " JUCE_MEDIUMP " vec2 coordinates_out;\n"
  "varying " JUCE_MEDIUMP " vec2 range_out;\n"
  "\n"
  "void main()\n"
  "{\n"
  "    coordinates_out = coordinates;\n"
  "    range_out = range;\n"
  "    gl_Position = position;\n"
  "}\n",

  "varying " JUCE_MEDIUMP " vec2 coordinates_out;\n"
  "varying " JUCE_MEDIUMP " vec2 range_out;\n"
  "\n"
  "uniform float radius;\n"
  "\n"
  "void main()\n"
  "{\n"
  "    float dist = length(coordinates_out);\n"
  "    float dist_amp = clamp(20.0 * (radius - dist), 0.0, 1.0);"
  "    float rads = atan(coordinates_out.x, coordinates_out.y);\n"
  "    float rads_amp_low = clamp(20.0 * (rads - range_out.x), 0.0, 1.0);"
  "    float rads_amp_high = clamp(20.0 * (range_out.y - rads), 0.0, 1.0);"
  "    gl_FragColor = vec4(0.2, 0.941, 0.45, dist_amp * rads_amp_low * rads_amp_high);\n"
  "}\n",

  "attribute " JUCE_MEDIUMP " vec4 position;\n"
  "\n"
  "varying " JUCE_MEDIUMP " vec2 position_out;\n"
  "\n"
  "void main()\n"
  "{\n"
  "    gl_Position = position;\n"
  "    position_out = position.xz;\n"
  "}\n",

  "varying " JUCE_MEDIUMP " vec2 position_out;\n"
  "void main()\n"
  "{\n"
  "    const float bars = 20.0;\n"
  "    float boost = bars * position_out.x;\n"
  "    float fraction = boost - floor(boost);\n"
  "    float amp = clamp(3.0 * fraction, 0.0, 1.0) * clamp(3.0 * (0.95 - fraction), 0.0, 1.0);\n"
  "    " JUCE_MEDIUMP " vec4 color = amp * vec4(0.5 * position_out.x + 0.5, 1.0, 0.4, 1.0);\n"
  "    if (position_out.x > 0.4142)\n"
  "        color = amp * vec4(1.0, 1.0 - 0.85 * position_out.x, 0.2, 1.0);\n"
  "    gl_FragColor = color;\n"
  "}\n",
};

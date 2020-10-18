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

#include "open_gl_envelope.h"

#include "colors.h"
#include "synth_gui_interface.h"
#include "utils.h"

namespace {
  const float ATTACK_RANGE_PERCENT = 0.33f;
  const float DECAY_RANGE_PERCENT = 0.33f;
  const float HOVER_DISTANCE = 20.0f;
  const int GRID_CELL_WIDTH = 8;
  const float MARKER_WIDTH = 6.0f;
  const int IMAGE_HEIGHT = 256;

} // namespace


OpenGLEnvelope::OpenGLEnvelope() {
  attack_hover_ = false;
  decay_hover_ = false;
  sustain_hover_ = false;
  release_hover_ = false;
  mouse_down_ = false;

  attack_slider_ = nullptr;
  decay_slider_ = nullptr;
  sustain_slider_ = nullptr;
  release_slider_ = nullptr;
  envelope_amp_ = nullptr;
  envelope_phase_ = nullptr;

  position_vertices_ = new float[16] {
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, -1.0f, 0.0f, 0.0f,
    0.1f, -1.0f, 1.0f, 0.0f,
    0.1f, 1.0f, 1.0f, 1.0f
  };

  position_triangles_ = new int[6] {
    0, 1, 2,
    2, 3, 0
  };
}

OpenGLEnvelope::~OpenGLEnvelope() {
  delete[] position_vertices_;
  delete[] position_triangles_;
}

void OpenGLEnvelope::paintBackground() {
  static const DropShadow shadow(Colour(0xbb000000), 5, Point<int>(0, 0));

  if (getWidth() <= 0 || getHeight() <= 0)
    return;

  float ratio = getHeight() / 100.0f;

  const Displays::Display& display = Desktop::getInstance().getDisplays().getMainDisplay();
  float scale = display.scale;
  background_image_ = Image(Image::ARGB, scale * getWidth(), scale * getHeight(), true);
  Graphics g(background_image_);
  g.addTransform(AffineTransform::scale(scale, scale));

  g.fillAll(Colour(0xff424242));

  g.setColour(Colour(0xff4a4a4a));
  for (int x = 0; x < getWidth(); x += GRID_CELL_WIDTH)
    g.drawLine(x, 0, x, getHeight());
  for (int y = 0; y < getHeight(); y += GRID_CELL_WIDTH)
    g.drawLine(0, y, getWidth(), y);

  shadow.drawForPath(g, envelope_line_);
  g.setColour(Colors::graph_fill);
  g.fillPath(envelope_line_);

  g.setColour(Colour(0xff505050));
  g.drawLine(getAttackX(), 0.0f, getAttackX(), getHeight());
  g.drawLine(getDecayX(), getSustainY(), getDecayX(), getHeight());

  g.setColour(Colors::modulation);
  float line_width = 1.5f * ratio;
  PathStrokeType stroke(line_width, PathStrokeType::beveled, PathStrokeType::rounded);
  g.strokePath(envelope_line_, stroke);

  float hover_line_x = -20;
  if (attack_hover_)
    hover_line_x = getAttackX();
  else if (decay_hover_)
    hover_line_x = getDecayX();
  else if (release_hover_)
    hover_line_x = getReleaseX();

  g.setColour(Colour(0xbbffffff));
  g.fillRect(hover_line_x - 0.5f, 0.0f, 1.0f, 1.0f * getHeight());

  float grab_radius = 20.0f * ratio;
  float hover_radius = 7.0f * ratio;
  if (sustain_hover_) {
    if (mouse_down_) {
      g.setColour(Colour(0x11ffffff));
      g.fillEllipse(getDecayX() - grab_radius, getSustainY() - grab_radius,
                    2.0f * grab_radius, 2.0f * grab_radius);
    }

    g.setColour(Colour(0xbbffffff));
    g.drawEllipse(getDecayX() - hover_radius, getSustainY() - hover_radius,
                  2.0f * hover_radius, 2.0f * hover_radius, 1.0);
  }
  else if (mouse_down_) {
    g.setColour(Colour(0x11ffffff));
    g.fillRect(hover_line_x - 10.0f, 0.0f, 20.0f, 1.0f * getHeight());
  }

  g.setColour(Colors::modulation);
  float marker_radius = ratio * MARKER_WIDTH / 2.0f;
  g.fillEllipse(getDecayX() - marker_radius, getSustainY() - marker_radius,
                2.0f * marker_radius, 2.0f * marker_radius);
  g.setColour(Colour(0xff000000));
  g.fillEllipse(getDecayX() - marker_radius / 2.0f, getSustainY() - marker_radius / 2.0f,
                marker_radius, marker_radius);

  background_.updateBackgroundImage(background_image_);
}

void OpenGLEnvelope::paintPositionImage() {
  int min_image_width = roundToInt(4 * MARKER_WIDTH);
  int image_width = mopo::utils::nextPowerOfTwo(min_image_width);
  int marker_width = 2 * MARKER_WIDTH;
  int image_height = roundToInt(2 * IMAGE_HEIGHT);
  position_image_ = Image(Image::ARGB, image_width, image_height, true);
  Graphics g(position_image_);

  g.setColour(Colour(0x77ffffff));
  g.fillRect(image_width / 2.0f - 0.5f, 0.0f, 1.0f, 1.0f * image_height);

  g.setColour(Colors::modulation);
  g.fillEllipse(image_width / 2 - marker_width / 2, image_height / 2 - marker_width / 2,
                marker_width, marker_width);
  g.setColour(Colour(0xff000000));
  g.fillEllipse(image_width / 2 - marker_width / 4, image_height / 2 - marker_width / 4,
                marker_width / 2, marker_width / 2);
}

void OpenGLEnvelope::resized() {
  resetEnvelopeLine();

  SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
  if (envelope_amp_ == nullptr && parent)
    envelope_amp_ = parent->getSynth()->getModSource(getName().toStdString() + "_amp");

  if (envelope_phase_ == nullptr && parent)
    envelope_phase_ = parent->getSynth()->getModSource(getName().toStdString() + "_phase");
}

void OpenGLEnvelope::mouseMove(const MouseEvent& e) {
  float x = e.getPosition().x;
  float attack_delta = fabs(x - getAttackX());
  float decay_delta = fabs(x - getDecayX());
  float release_delta = fabs(x - getReleaseX());
  float sustain_delta = fabs(e.getPosition().y - getSustainY());

  bool a_hover = attack_delta < decay_delta && attack_delta < HOVER_DISTANCE;
  bool d_hover = !attack_hover_ && decay_delta < release_delta && decay_delta < HOVER_DISTANCE;
  bool r_hover = !decay_hover_ && release_delta < HOVER_DISTANCE;
  bool s_hover = !a_hover && !r_hover && x > getDecayX() - HOVER_DISTANCE &&
  x < getDecayX() + HOVER_DISTANCE && sustain_delta < HOVER_DISTANCE;

  if (a_hover != attack_hover_ || d_hover != decay_hover_ ||
      s_hover != sustain_hover_ || r_hover != release_hover_) {
    attack_hover_ = a_hover;
    decay_hover_ = d_hover;
    sustain_hover_ = s_hover;
    release_hover_ = r_hover;
    paintBackground();
  }
}

void OpenGLEnvelope::mouseExit(const MouseEvent& e) {
  attack_hover_ = false;
  decay_hover_ = false;
  sustain_hover_ = false;
  release_hover_ = false;
  paintBackground();
}

void OpenGLEnvelope::mouseDown(const MouseEvent& e) {
  mouse_down_ = true;
  paintBackground();
}

void OpenGLEnvelope::mouseDrag(const MouseEvent& e) {
  if (attack_hover_)
    setAttackX(e.getPosition().x);
  else if (decay_hover_)
    setDecayX(e.getPosition().x);
  else if (release_hover_)
    setReleaseX(e.getPosition().x);

  if (sustain_hover_)
    setSustainY(e.getPosition().y);

  if (attack_hover_ || decay_hover_ || sustain_hover_ || release_hover_) {
    resetEnvelopeLine();
    paintBackground();
  }
}

void OpenGLEnvelope::mouseUp(const MouseEvent& e) {
  mouse_down_ = false;
  paintBackground();
}

void OpenGLEnvelope::guiChanged(SynthSlider* slider) {
  resetEnvelopeLine();
  paintBackground();
}

float OpenGLEnvelope::getAttackX() {
  if (!attack_slider_)
    return 0.0;

  double percent = attack_slider_->valueToProportionOfLength(attack_slider_->getValue());
  return 1 + (getWidth() - 1) * percent * ATTACK_RANGE_PERCENT;
}

float OpenGLEnvelope::getDecayX() {
  if (!decay_slider_)
    return 0.0;

  double percent = decay_slider_->valueToProportionOfLength(decay_slider_->getValue());
  return getAttackX() + getWidth() * percent * DECAY_RANGE_PERCENT;
}

float OpenGLEnvelope::getSustainY() {
  if (!sustain_slider_)
    return 0.0;

  double percent = sustain_slider_->valueToProportionOfLength(sustain_slider_->getValue());
  return getHeight() * (1.0 - percent);
}

float OpenGLEnvelope::getReleaseX() {
  if (!release_slider_)
    return 0.0;

  double percent = release_slider_->valueToProportionOfLength(release_slider_->getValue());
  return getDecayX() + getWidth() * percent * (1.0 - DECAY_RANGE_PERCENT - ATTACK_RANGE_PERCENT);
}

void OpenGLEnvelope::setAttackX(double x) {
  if (!attack_slider_)
    return;

  double percent = x / (getWidth() * ATTACK_RANGE_PERCENT);
  attack_slider_->setValue(attack_slider_->proportionOfLengthToValue(percent));
}

void OpenGLEnvelope::setDecayX(double x) {
  if (!decay_slider_)
    return;

  double percent = (x - getAttackX()) / (getWidth() * DECAY_RANGE_PERCENT);
  decay_slider_->setValue(decay_slider_->proportionOfLengthToValue(percent));
}

void OpenGLEnvelope::setSustainY(double y) {
  if (!sustain_slider_)
    return;

  sustain_slider_->setValue(sustain_slider_->proportionOfLengthToValue(1.0 - y / getHeight()));
}

void OpenGLEnvelope::setReleaseX(double x) {
  if (!release_slider_)
    return;

  double percent = (x - getDecayX()) /
  (getWidth() * (1.0 - DECAY_RANGE_PERCENT - ATTACK_RANGE_PERCENT));
  release_slider_->setValue(release_slider_->proportionOfLengthToValue(percent));
}

void OpenGLEnvelope::setAttackSlider(SynthSlider* attack_slider) {
  attack_slider_ = attack_slider;
  attack_slider_->addSliderListener(this);
  resetEnvelopeLine();
}

void OpenGLEnvelope::setDecaySlider(SynthSlider* decay_slider) {
  decay_slider_ = decay_slider;
  decay_slider_->addSliderListener(this);
  resetEnvelopeLine();
}

void OpenGLEnvelope::setSustainSlider(SynthSlider* sustain_slider) {
  sustain_slider_ = sustain_slider;
  sustain_slider_->addSliderListener(this);
  resetEnvelopeLine();
}

void OpenGLEnvelope::setReleaseSlider(SynthSlider* release_slider) {
  release_slider_ = release_slider;
  release_slider_->addSliderListener(this);
  resetEnvelopeLine();
}

void OpenGLEnvelope::resetEnvelopeLine() {
  envelope_line_.clear();
  envelope_line_.startNewSubPath(1, getHeight());
  envelope_line_.lineTo(getAttackX(), 0.0f);
  envelope_line_.quadraticTo(0.5f * (getAttackX() + getDecayX()), getSustainY(),
                             getDecayX(), getSustainY());

  envelope_line_.quadraticTo(0.5f * (getReleaseX() + getDecayX()), getHeight(),
                             getReleaseX(), getHeight());

  paintBackground();
}

Point<float> OpenGLEnvelope::valuesToPosition(float phase, float amp) {
  float y = (1.0 - amp) * getHeight();
  float x = 0.0;

  if (phase < 0.0 || phase > 2.5)
    return Point<float>(-2.0, -2.0);

  float attack_x = getAttackX();
  float decay_x = getDecayX();
  float release_x = getReleaseX();
  float sustain = sustain_slider_->getValue();

  if (phase <= 0.5f) {
    x = amp * attack_x;
  }
  else if (phase <= 1.5f) {
    float delta = decay_x - attack_x;
    float amp_diff = 1.0f - sustain;
    if (amp_diff == 0.0f)
      x = decay_x;
    else {
      float percent = (1.0f - amp) / amp_diff;
      x = attack_x + percent * delta;
    }
  }
  else {
    float delta = release_x - decay_x;
    x = decay_x + delta - delta * (amp / sustain);
  }

  Point<float> closest;
  envelope_line_.getNearestPoint(Point<float>(x, y), closest);
  if (phase > 1.5f && phase < 2.5f && closest.x < decay_x) {
    closest.x = decay_x;
    closest.y = (1.0 - amp) * getHeight();
  }
  if (phase > 0.5f && phase < 1.5f && closest.x > decay_x) {
    closest.x = decay_x;
    closest.y = (1.0 - amp) * getHeight();
  }
  return Point<float>(2.0f * closest.x / getWidth() - 1.0f, 1.0f - 2.0f * closest.y / getHeight());
}

void OpenGLEnvelope::init(OpenGLContext& open_gl_context) {
  paintPositionImage();

  open_gl_context.extensions.glGenBuffers(1, &vertex_buffer_);
  open_gl_context.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);

  GLsizeiptr vert_size = static_cast<GLsizeiptr>(static_cast<size_t>(16 * sizeof(float)));
  open_gl_context.extensions.glBufferData(GL_ARRAY_BUFFER, vert_size,
                                          position_vertices_, GL_STATIC_DRAW);

  open_gl_context.extensions.glGenBuffers(1, &triangle_buffer_);
  open_gl_context.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_buffer_);

  GLsizeiptr tri_size = static_cast<GLsizeiptr>(static_cast<size_t>(6 * sizeof(float)));
  open_gl_context.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, tri_size,
                                          position_triangles_, GL_STATIC_DRAW);

  background_.init(open_gl_context);
}

void OpenGLEnvelope::drawPosition(OpenGLContext& open_gl_context) {
  if (position_texture_.getWidth() != position_image_.getWidth())
    position_texture_.loadImage(position_image_);

  if (envelope_phase_ == nullptr || envelope_amp_ == nullptr || envelope_amp_->buffer[0] <= 0.0)
    return;

  Point<float> point = valuesToPosition(envelope_phase_->buffer[0], envelope_amp_->buffer[0]);
  float x = point.x;
  float y = point.y;

  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  int draw_width = getWidth();
  int draw_height = getHeight();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  float ratio = getHeight() / 100.0f;

  float position_height = ratio * (0.5f * position_texture_.getHeight()) / draw_height;
  float position_width = ratio * (0.5f * position_texture_.getWidth()) / draw_width;
  position_vertices_[0] = x - position_width;
  position_vertices_[1] = y + position_height;
  position_vertices_[4] = x - position_width;
  position_vertices_[5] = y - position_height;
  position_vertices_[8] = x + position_width;
  position_vertices_[9] = y - position_height;
  position_vertices_[12] = x + position_width;
  position_vertices_[13] = y + position_height;

  open_gl_context.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
  GLsizeiptr vert_size = static_cast<GLsizeiptr>(static_cast<size_t>(16 * sizeof(float)));
  open_gl_context.extensions.glBufferData(GL_ARRAY_BUFFER, vert_size,
                                          position_vertices_, GL_STATIC_DRAW);

  open_gl_context.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_buffer_);
  position_texture_.bind();

  open_gl_context.extensions.glActiveTexture(GL_TEXTURE0);

  if (background_.texture_uniform() != nullptr)
    background_.texture_uniform()->set(0);

  background_.shader()->use();

  background_.enableAttributes(open_gl_context);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  background_.disableAttributes(open_gl_context);

  position_texture_.unbind();

  open_gl_context.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
  open_gl_context.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OpenGLEnvelope::render(OpenGLContext& open_gl_context, bool animate) {
  MOPO_ASSERT(glGetError() == GL_NO_ERROR);

  setViewPort(open_gl_context);

  background_.render(open_gl_context);

  if (animate)
    drawPosition(open_gl_context);

  MOPO_ASSERT(glGetError() == GL_NO_ERROR);
}

void OpenGLEnvelope::destroy(OpenGLContext& open_gl_context) {
  position_texture_.release();

  texture_ = nullptr;
  open_gl_context.extensions.glDeleteBuffers(1, &vertex_buffer_);
  open_gl_context.extensions.glDeleteBuffers(1, &triangle_buffer_);
  background_.destroy(open_gl_context);
}

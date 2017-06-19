/* Copyright 2013-2017 Matt Tytel
 *
 * mopo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mopo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mopo.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef MOPO_H
#define MOPO_H

#include "alias.h"
#include "arpeggiator.h"
#include "bit_crush.h"
#include "biquad_filter.h"
#include "bypass_router.h"
#include "circular_queue.h"
#include "common.h"
#include "delay.h"
#include "distortion.h"
#include "envelope.h"
#include "feedback.h"
#include "formant_manager.h"
#include "linear_slope.h"
#include "magnitude_lookup.h"
#include "memory.h"
#include "midi_lookup.h"
#include "mono_panner.h"
#include "note_handler.h"
#include "operators.h"
#include "oscillator.h"
#include "portamento_slope.h"
#include "processor.h"
#include "processor_router.h"
#include "resonance_lookup.h"
#include "reverb.h"
#include "reverb_all_pass.h"
#include "reverb_comb.h"
#include "simple_delay.h"
#include "smooth_filter.h"
#include "smooth_value.h"
#include "state_variable_filter.h"
#include "step_generator.h"
#include "stutter.h"
#include "tick_router.h"
#include "trigger_operators.h"
#include "utils.h"
#include "value.h"
#include "voice_handler.h"
#include "wave.h"

#endif // MOPO_H

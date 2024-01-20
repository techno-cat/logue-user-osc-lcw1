/*
Copyright 2023 Tomoaki Itoh
This software is released under the MIT License, see LICENSE.txt.
//*/

#include "userosc.h"
#include "LCWCommon.h"
#include "LCWPitchTable.h"
#include "LCWWaveTable.h"

#define LCW_OSC_TIMER_BITS (LCW_PITCH_DELTA_VALUE_BITS)
#define LCW_OSC_TIMER_MAX (1 << LCW_OSC_TIMER_BITS)
#define LCW_OSC_TIMER_MASK ((LCW_OSC_TIMER_MAX) - 1)

typedef struct {
  uint32_t t;
  uint32_t dt;
  LCWOscWaveTable *table;
} LCWOscState;

static struct {
  float shape = 0;
  float shiftshape = 0;
  int32_t mainTableIndex = 0;
  int32_t subTableIndex = 0;
} s_param;

static struct {
  LCWOscState osc[2];
  int32_t pitch1 = 0;   // s7.24
  int32_t pitch2 = 0;   // s7.24
  int32_t shape_lfo = 0;
} s_state;

#define LCW_WAV_FRAC_BITS (LCW_OSC_TIMER_BITS - LCW_WAV_TABLE_BITS)
SQ3_12 lookupWaveTable(LCWOscWaveTable *table, uint32_t t)
{
  const uint32_t i = t >> (LCW_OSC_TIMER_BITS - LCW_WAV_TABLE_BITS);
  const uint32_t frac =
    (t & (LCW_OSC_TIMER_MASK >> LCW_WAV_TABLE_BITS)) >> (LCW_WAV_FRAC_BITS - 8);
  const int32_t val1 = (int32_t)(*table)[i & LCW_WAV_TABLE_MASK];
  const int32_t val2 =  (int32_t)(*table)[(i+1) & LCW_WAV_TABLE_MASK];
  return (SQ3_12)( val1 + (((val2 - val1) * (int32_t)frac) >> 8) );
}

void OSC_INIT(uint32_t platform, uint32_t api)
{
  s_param.shape = 0.f;
  s_param.shiftshape = 0.f;

  s_state.osc[0].t = 0;
  s_state.osc[0].dt = 0;
  s_state.osc[0].table = &(lcwWaveTables[0]);
  s_state.osc[1].t = 0;
  s_state.osc[1].dt = 0;
  s_state.osc[1].table = &(lcwWaveTables[0]);
  s_state.pitch1 = (LCW_NOTE_NO_A4 << 24) / 12;
  s_state.pitch2 = (LCW_NOTE_NO_A4 << 24) / 12;
  s_state.shape_lfo = 0;
}

#define LCW_PULSE_TABLE_INDEX_MAX (LCW_PULSE_TABLE_COUNT - 1)
void OSC_CYCLE(const user_osc_param_t * const params,
               int32_t *yn,
               const uint32_t frames)
{
  // s11.20に拡張してから、整数部がoctaveになるように加工
  int32_t pitch1 = (int32_t)params->pitch << 12;
  pitch1 = (pitch1 - (LCW_NOTE_NO_A4 << 20)) / 12;
  const int32_t detune = (int32_t)si_roundf(128.f * s_param.shape);
  // [0 .. 128] -> [-64 .. +64] -> [-1/32oct .. +1/32oct]
  int32_t pitch2 = pitch1 + ((detune - 64) << (20 - 6 - 5));

  int32_t lfo_delta = (params->shape_lfo - s_state.shape_lfo) / (int32_t)frames;

  // s11.20 -> s7.24
  pitch1 <<= 4;
  pitch2 <<= 4;

  // Temporaries.
  int32_t shape_lfo = s_state.shape_lfo;

  q31_t * __restrict y = (q31_t *)yn;
  const q31_t * y_e = y + frames;

  // Main Mix/Sub Mix, 8bit(= [0-256])
  const int32_t subVol0 = (int32_t)(0x100 * s_param.shiftshape);

  LCWOscState *osc = &(s_state.osc[0]);
  osc[0].table = &(lcwWaveTables[LCW_CLIP(s_param.mainTableIndex, 0, LCW_PULSE_TABLE_INDEX_MAX)]);
  osc[1].table = &(lcwWaveTables[LCW_CLIP(s_param.subTableIndex, 0, LCW_PULSE_TABLE_INDEX_MAX)]);

  for (; y != y_e; ) {
    int32_t subVol = subVol0 + (shape_lfo >> 24);
    subVol = LCW_CLIP(subVol, 0, 0x100);
    const int32_t mainVol = 0x100 - subVol;

    const SQ3_12 out1 = lookupWaveTable( osc[0].table, osc[0].t );
    const SQ3_12 out2 = lookupWaveTable( osc[1].table, osc[1].t );
    int32_t out = (out1 * mainVol) + (out2 * subVol);
    *(y++) = (q31_t)( LCW_CLIP(out, -0x01000000, 0x00FFFFFF) << (31 - (12 + 8)) );

    osc[0].dt = pitch_to_timer_delta( pitch1 >> 8 );
    osc[1].dt = pitch_to_timer_delta( pitch2 >> 8 );
    osc[0].t = (osc[0].t + osc[0].dt) & LCW_OSC_TIMER_MASK;
    osc[1].t = (osc[1].t + osc[1].dt) & LCW_OSC_TIMER_MASK;

    shape_lfo += lfo_delta;
  }

  s_state.shape_lfo = params->shape_lfo;
  s_state.pitch1 = pitch1;
  s_state.pitch2 = pitch2;
}

void OSC_NOTEON(const user_osc_param_t * const params)
{
  return;
}

void OSC_NOTEOFF(const user_osc_param_t * const params)
{
  return;
}

void OSC_PARAM(uint16_t index, uint16_t value)
{
  switch (index) {
  case k_user_osc_param_shape:
    s_param.shape = clip01f( param_val_to_f32(value) );
    break;
  case k_user_osc_param_shiftshape:
    s_param.shiftshape = clip01f( param_val_to_f32(value) );
    break;
  case k_user_osc_param_id1:
    s_param.mainTableIndex = (int32_t)value;
    break;
  case k_user_osc_param_id2:
    s_param.subTableIndex = (int32_t)value;
    break;
  default:
    break;
  }
}

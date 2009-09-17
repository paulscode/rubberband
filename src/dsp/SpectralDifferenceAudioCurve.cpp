/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    Rubber Band
    An audio time-stretching and pitch-shifting library.
    Copyright 2007-2009 Chris Cannam.
    
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.  See the file
    COPYING included with this distribution for more information.
*/

#include "SpectralDifferenceAudioCurve.h"

#include "system/Allocators.h"
#include "system/VectorOps.h"

namespace RubberBand
{

SpectralDifferenceAudioCurve::SpectralDifferenceAudioCurve(size_t sampleRate, size_t windowSize) :
    AudioCurveCalculator(sampleRate, windowSize)
{
    m_mag = allocate<double>(m_windowSize/2 + 1);
    m_tmpbuf = allocate<double>(m_windowSize/2 + 1);
    v_zero(m_mag, m_windowSize/2 + 1);
}

SpectralDifferenceAudioCurve::~SpectralDifferenceAudioCurve()
{
    deallocate(m_mag);
    deallocate(m_tmpbuf);
}

void
SpectralDifferenceAudioCurve::reset()
{
    v_zero(m_mag, m_windowSize/2 + 1);
}

void
SpectralDifferenceAudioCurve::setWindowSize(size_t newSize)
{
    deallocate(m_tmpbuf);
    deallocate(m_mag);
    m_windowSize = newSize;
    m_mag = allocate<double>(m_windowSize/2 + 1);
    m_tmpbuf = allocate<double>(m_windowSize/2 + 1);
    reset();
}

float
SpectralDifferenceAudioCurve::processFloat(const float *R__ mag, size_t increment)
{
    double result = 0.0;

    const int hs1 = m_windowSize/2 + 1;

    v_convert(m_tmpbuf, mag, hs1);
    v_square(m_tmpbuf, hs1);
    v_subtract(m_mag, m_tmpbuf, hs1);
    v_abs(m_mag, hs1);
    v_sqrt(m_mag, hs1);
    
    for (int i = 0; i < hs1; ++i) {
        result += m_mag[i];
    }

    v_copy(m_mag, m_tmpbuf, hs1);
    return result;
}

double
SpectralDifferenceAudioCurve::processDouble(const double *R__ mag, size_t increment)
{
    double result = 0.0;

    const int hs1 = m_windowSize/2 + 1;

    v_convert(m_tmpbuf, mag, hs1);
    v_square(m_tmpbuf, hs1);
    v_subtract(m_mag, m_tmpbuf, hs1);
    v_abs(m_mag, hs1);
    v_sqrt(m_mag, hs1);
    
    for (int i = 0; i < hs1; ++i) {
        result += m_mag[i];
    }

    v_copy(m_mag, m_tmpbuf, hs1);
    return result;
}

}


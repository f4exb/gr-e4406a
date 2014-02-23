/* -*- c++ -*- */
/*
 * Copyright 2005,2012 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

/* WARNING: this file is machine generated. Edits will be overwritten */

#ifndef INCLUDED_E4406A_VARIABLE_RESAMPLER_CCC_H
#define	INCLUDED_E4406A_VARIABLE_RESAMPLER_CCC_H

#include <e4406a/api.h>
#include <gnuradio/block.h>

namespace gr {
namespace e4406a {

/*!
* \brief Rational Resampling Polyphase FIR filter with gr_complex
* input, gr_complex output and gr_complex taps.
* \ingroup resamplers_blk
*/
class E4406A_API variable_resampler_ccc : virtual public gr::block
{
public:
    // gr::filter::variable_resampler_ccc::sptr
    typedef boost::shared_ptr<variable_resampler_ccc> sptr;

    static sptr make(unsigned interpolation,
            unsigned decimation,
            const std::vector<gr_complex> &taps);

    virtual unsigned interpolation() const = 0;
    virtual unsigned decimation() const = 0;

    virtual void set_taps(const std::vector<gr_complex> &taps) = 0;
    virtual void set_decim(unsigned decimation) = 0;
    virtual void set_interp(unsigned interpolation) = 0;
    virtual std::vector<gr_complex> taps() const = 0;
};

} /* namespace e4406a */
} /* namespace gr */

#endif /* INCLUDED_E4406A_VARIABLE_RESAMPLER_CCC_H */

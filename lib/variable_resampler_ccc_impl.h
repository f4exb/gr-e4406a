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

#ifndef INCLUDED_FILTER_VARIABLE_RESAMPLER_CCC_IMPL_H
#define	INCLUDED_FILTER_VARIABLE_RESAMPLER_CCC_IMPL_H

#include <gnuradio/filter/fir_filter.h>
#include <gnuradio/thread/thread.h>
#include <e4406a/variable_resampler_ccc.h>

namespace gr {
namespace e4406a {

class variable_resampler_ccc_impl : public variable_resampler_ccc
{
private:
    unsigned d_history;
    unsigned d_interpolation;
    unsigned d_decimation;
    unsigned d_ctr;
    std::vector<gr_complex> d_new_taps;
    std::vector<filter::kernel::fir_filter_ccc *> d_firs;
    gr::thread::mutex d_mutex; 
    bool d_updated;

    void install_taps(const std::vector<gr_complex> &taps);

public:
    variable_resampler_ccc_impl(unsigned interpolation, unsigned decimation,
    const std::vector<gr_complex> &taps);

    ~variable_resampler_ccc_impl();

    unsigned history() const { return d_history; }
    void set_history(unsigned history) { d_history = history; }

    unsigned interpolation() const { return d_interpolation; }
    unsigned decimation() const { return d_decimation; }

    void set_taps(const std::vector<gr_complex> &taps);
    void set_decim(unsigned decimation);
    void set_interp(unsigned interpolation);
    std::vector<gr_complex> taps() const;

    void forecast(int noutput_items, gr_vector_int &ninput_items_required);
    int  general_work(int noutput_items,
    gr_vector_int &ninput_items,
    gr_vector_const_void_star &input_items,
    gr_vector_void_star &output_items);
    
protected:
    void set_params(unsigned interpolation, unsigned decimation, const std::vector<gr_complex> &taps);

};

} /* namespace e4406a */
} /* namespace gr */

#endif /* INCLUDED_FILTER_RATIONAL_RESAMPLER_CCC_IMPL_H */

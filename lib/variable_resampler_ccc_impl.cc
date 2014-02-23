/* -*- c++ -*- */
/*
 * Copyright 2004,2010,2012 Free Software Foundation, Inc.
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "variable_resampler_ccc_impl.h"
#include <gnuradio/io_signature.h>
#include <volk/volk.h>
#include <stdexcept>

namespace gr {
namespace e4406a {
    
variable_resampler_ccc::sptr variable_resampler_ccc::make(unsigned interpolation,
        unsigned decimation,
        const std::vector<gr_complex> &taps)
{
    return gnuradio::get_initial_sptr(new variable_resampler_ccc_impl(interpolation, decimation, taps));
}

variable_resampler_ccc_impl::variable_resampler_ccc_impl(unsigned interpolation,
        unsigned decimation,
        const std::vector<gr_complex> &taps)
    : block("variable_resampler_ccc",
      io_signature::make(1, 1, sizeof(gr_complex)),
      io_signature::make(1, 1, sizeof(gr_complex))),
      d_history(1),
      d_interpolation(interpolation),
      d_decimation(decimation),
      d_ctr(0),
      d_firs(interpolation),
      d_updated(false)
{
    if(interpolation == 0)
    {
        throw std::out_of_range("variable_resampler_ccc_impl: interpolation must be > 0");
    }
    
    if(decimation == 0)
    {
        throw std::out_of_range("variable_resampler_ccc_impl: decimation must be > 0");
    }

    set_relative_rate(1.0 * interpolation / decimation);
    set_output_multiple(1);

    std::vector<gr_complex> dummy_taps;

    for(unsigned i = 0; i < interpolation; i++) 
    {
        d_firs[i] = new filter::kernel::fir_filter_ccc(1, dummy_taps);
    }

    set_taps(taps);
    install_taps(d_new_taps);
}

variable_resampler_ccc_impl::~variable_resampler_ccc_impl()
{
    int interp = interpolation();
    for(int i = 0; i < interp; i++) 
    {
        delete d_firs[i];
    }
}


void variable_resampler_ccc_impl::set_decim(unsigned decimation)
{
    std::vector<gr_complex> new_taps(d_new_taps);
    set_params(d_interpolation, decimation, new_taps);
}


void variable_resampler_ccc_impl::set_interp(unsigned interpolation)
{
    std::vector<gr_complex> new_taps(d_new_taps);
    set_params(interpolation, d_decimation, new_taps);
}


void variable_resampler_ccc_impl::set_params(unsigned interp, unsigned decim, const std::vector<gr_complex> &taps)
{
    int old_interp = d_interpolation;
    int old_decim = d_decimation;
    
    if ((interp != old_interp) || (decim != old_decim))
    {
        gr::thread::scoped_lock guard(d_mutex);

        if (interp != old_interp)
        {
            for (int i = 0; i < old_interp; i++)
            {
                delete d_firs[i];
            }
            std::vector<gr_complex> dummy_taps;

            d_firs.clear();
            
            for (unsigned i = 0; i < interp; i++)
            {
                //d_firs.push_back(gr_fir_util::create_gr_fir_ccc(dummy_taps)); old way
                //d_firs[i] = new filter::kernel::fir_filter_ccc(1, dummy_taps); new way in constructor
                d_firs.push_back(new filter::kernel::fir_filter_ccc(1, dummy_taps));
            }

            set_taps (taps);
            //install_taps (d_new_taps); // Do in work so it can return 0
            
            d_interpolation = interp;
        }

        if (decim != old_decim)
        {
            d_decimation = decim;
        }

        set_relative_rate (1.0 * interp / decim);
        d_ctr = 0;
    }
}


void variable_resampler_ccc_impl::set_taps(const std::vector<gr_complex> &taps)
{
    d_new_taps.clear();
    //d_new_taps = taps;
    d_updated = true;

    // round up length to a multiple of the interpolation factor
    int n = taps.size() % interpolation();
    
    if(n > 0) 
    {
        n = interpolation() - n;
        
        while(n-- > 0) 
        {
            d_new_taps.insert(d_new_taps.begin(), 0);
        }
    }
    
    d_new_taps.insert(d_new_taps.end(), taps.begin(), taps.end());
    assert(d_new_taps.size() % interpolation() == 0);
}


void variable_resampler_ccc_impl::install_taps(const std::vector<gr_complex> &taps)
{
    int nfilters = interpolation();
    int nt = taps.size() / nfilters;

    assert(nt * nfilters == (int) taps.size());

    std::vector< std::vector <gr_complex> > xtaps(nfilters);

    for(int n = 0; n < nfilters; n++)
    {
        xtaps[n].resize (nt);
    }

    for(int i = 0; i < (int)taps.size(); i++)
    {
        xtaps[i % nfilters][i / nfilters] = taps[i];
    }

    for(int n = 0; n < nfilters; n++)
    {
        d_firs[n]->set_taps(xtaps[n]);
    }

    set_history(nt);
    d_updated = false;
}

std::vector<gr_complex> variable_resampler_ccc_impl::taps() const
{
    return d_new_taps;
}

void variable_resampler_ccc_impl::forecast(int noutput_items, gr_vector_int &ninput_items_required)
{
    int nreqd = std::max((unsigned)1, (int)((double) (noutput_items+1) * decimation() / interpolation()) + history() - 1);
    unsigned ninputs = ninput_items_required.size();

    for(unsigned i = 0; i < ninputs; i++)
    {
        ninput_items_required[i] = nreqd;
    }
}


int variable_resampler_ccc_impl::general_work(int noutput_items,
        gr_vector_int &ninput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
{
    const gr_complex *in = (const gr_complex *)input_items[0];
    gr_complex *out = (gr_complex *)output_items[0];

    gr::thread::scoped_lock guard(d_mutex);

    if(d_updated) 
    {
        install_taps(d_new_taps);
        return 0;		// history requirement may have increased.
    }

    unsigned int ctr = d_ctr;
    int i = 0;
    
    while(i < noutput_items) 
    {
        out[i++] = d_firs[ctr]->filter(in);
        ctr += decimation();
        while(ctr >= interpolation()) 
        {
            ctr -= interpolation();
            in++;
        }
    }

    d_ctr = ctr;
    consume_each(in - (gr_complex*)input_items[0]);
    return i;
}

} /* namespace e4406a */
} /* namespace gr */

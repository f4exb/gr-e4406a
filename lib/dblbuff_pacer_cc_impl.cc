/* -*- c++ -*- */
/* 
 * Copyright 2014 Edouard Griffiths, F4EXB.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "dblbuff_pacer_cc_impl.h"

namespace gr {
namespace e4406a {

//==================================================================================================
dblbuff_pacer_cc::sptr dblbuff_pacer_cc::make(unsigned out_size, unsigned block_factor)
{
    return gnuradio::get_initial_sptr(new dblbuff_pacer_cc_impl(out_size, block_factor));
}

//==================================================================================================
dblbuff_pacer_cc_impl::dblbuff_pacer_cc_impl(unsigned out_size, unsigned block_factor)
    : gr::block("dblbuff_pacer_cc",
            gr::io_signature::make(1, 1, sizeof(gr_complex)),
            gr::io_signature::make(1, 1, sizeof(gr_complex))),
    d_out_size(out_size),
    d_block_factor(block_factor),
    d_buff_parity(0),
    d_block_index(0),
    d_initialize(true)
{
    d_block_factor = (d_block_factor/2) * 2; // round to multiple of 2
    
    if (d_block_factor == 0)
    {
        d_block_factor = 2;
    }
    
    d_buff = (gr_complex *) malloc(2*d_out_size*d_block_factor*sizeof(gr_complex)); // allocate double buffer
    
    for (unsigned int i=0; i<d_out_size*d_block_factor; i++)
    {
        d_buff[i] = 0; // clear first single buffer
    }
    
    gr::block::set_output_multiple(d_out_size); // make sure multiple of fixed output size blocks are requested
}

//==================================================================================================
dblbuff_pacer_cc_impl::~dblbuff_pacer_cc_impl()
{
    free(d_buff);
}

//==================================================================================================
void dblbuff_pacer_cc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
{
    if (d_initialize)
    {
        ninput_items_required[0] = d_out_size*d_block_factor; // one single buffer
    }
    else if ((d_block_index == d_block_factor) || (d_block_index == d_block_factor*2))
    {
        ninput_items_required[0] = d_out_size*d_block_factor; // one single buffer
    }
    else
    {
        ninput_items_required[0] = 0; // nothing
    }
}

//==================================================================================================
int dblbuff_pacer_cc_impl::general_work (int noutput_items,
        gr_vector_int &ninput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
{
    const gr_complex *in = (const gr_complex *) input_items[0];
    gr_complex *out = (gr_complex *) output_items[0];
    unsigned consumed = 0;

    // fill second single buffer at start

    if (d_initialize)
    {
        std::cerr << "Initialize" << std::endl;
        d_buff_parity = (d_buff_parity + 1) % 2; // toggle parity 
        read_input_block(in);
        consumed = d_out_size*d_block_factor;
        d_initialize = false;
    }

    // fill one single buffer when the other is consumed

    if (d_block_index == d_block_factor)
    {
        read_input_block(in);
        consumed = d_out_size*d_block_factor;
    }
    else if (d_block_index == 2*d_block_factor)
    {
        read_input_block(in);
        consumed = d_out_size*d_block_factor;
        d_block_index = 0;
    }
    
    // serve data from current block index
    
    gr_complex *buff_start = &d_buff[d_out_size*d_block_index];
    
    for (unsigned i=0; i<d_out_size; i++)
    {
        out[i] = buff_start[i];
    }
    
    //std::cerr << "block " << d_block_index << " written, consumed " << consumed << std::endl;
    d_block_index++;
    
    // Tell runtime system how many input items we consumed on
    // each input stream.
    consume_each (consumed);

    // Tell runtime system how many output items we produced.
    return d_out_size;
}

//==================================================================================================
void dblbuff_pacer_cc_impl::read_input_block(const gr_complex *in)
{
    gr_complex *buff_start = &d_buff[d_buff_parity*d_out_size*d_block_factor];
    
    std::cerr << "Fetch..." << std::endl;
    
    for (unsigned i=0; i<d_out_size*d_block_factor; i++)
    {
        buff_start[i] = in[i];
    }
    
    d_buff_parity = (d_buff_parity + 1) % 2; // toggle parity 
}

} /* namespace e4406a */
} /* namespace gr */


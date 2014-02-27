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

#ifndef INCLUDED_E4406A_DBLBUFF_PACER_CC_IMPL_H
#define INCLUDED_E4406A_DBLBUFF_PACER_CC_IMPL_H

#include <e4406a/dblbuff_pacer_cc.h>

namespace gr {
namespace e4406a {

class dblbuff_pacer_cc_impl : public dblbuff_pacer_cc
{
private:
// Nothing to declare in this block.

public:

    /**
     * Constructor
     * \param out_size Size of output blocks
     * \param block_factor Multiplicative factor i.e. input block size is this value
     */
    dblbuff_pacer_cc_impl(unsigned out_size, unsigned block_factor);
    
    
    /**
     * Destructor
     */
    ~dblbuff_pacer_cc_impl();

    // Where all the action really happens
    void forecast (int noutput_items, gr_vector_int &ninput_items_required);

    int general_work(int noutput_items,
            gr_vector_int &ninput_items,
            gr_vector_const_void_star &input_items,
            gr_vector_void_star &output_items);
    
protected:
    unsigned d_out_size;     //!< Size of output block
    unsigned d_block_factor; //!< Multiplicative factor to output block size to get input block size
    unsigned d_buff_parity;  //!< Block parity for double buffer structure: 0: first buffer, 1: second buffer
    unsigned d_block_index;  //!< Current block index to serve
    gr_complex *d_buff;      //!< Double buffer
    bool d_initialize;       //!< True to initialize double buffer
    
    /**
     * Read input block
     * \brief Reads a new single block from input
     * \param in Pointer to input
     */
    void read_input_block(const gr_complex *in);
};

} // namespace e4406a
} // namespace gr

#endif /* INCLUDED_E4406A_DBLBUFF_PACER_CC_IMPL_H */


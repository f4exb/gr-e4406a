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

#ifndef INCLUDED_E4406A_SIMPLE_DECIMATOR_CC_IMPL_H
#define INCLUDED_E4406A_SIMPLE_DECIMATOR_CC_IMPL_H

#include <gnuradio/thread/thread.h>
#include <e4406a/simple_decimator_cc.h>

namespace gr {
namespace e4406a {

class simple_decimator_cc_impl : public simple_decimator_cc
{
private:
// Nothing to declare in this block.

public:
    /**
     * Constructor
     * \param decim Decimation factor
     */
    simple_decimator_cc_impl(unsigned decim);
    
    /**
     * Destructor
     */
    ~simple_decimator_cc_impl();
    
    /**
     * Set dicimation factor
     * \param decim Decimation factor
     */
    void set_decim(unsigned decim);

    /**
     * Worker
     * \brief Where all the action really happens
     */
    int work(int noutput_items,
            gr_vector_const_void_star &input_items,
            gr_vector_void_star &output_items);
        
protected:
    gr::thread::mutex d_decim_mutex; //!< decimation protection
};

} // namespace e4406a
} // namespace gr

#endif /* INCLUDED_E4406A_SIMPLE_DECIMATOR_CC_IMPL_H */


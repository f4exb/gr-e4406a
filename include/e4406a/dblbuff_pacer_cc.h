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

#ifndef INCLUDED_E4406A_DBLBUFF_PACER_CC_H
#define INCLUDED_E4406A_DBLBUFF_PACER_CC_H

#include <e4406a/api.h>
#include <gnuradio/block.h>

namespace gr {
namespace e4406a {

/**
* Double Buffered Paceer with complex input and output
* 
* \brief This is a block that fetches larger amount of data on its input 
* than it serves on its output. It uses a double buffering to optimise the 
* fetching process. This is somehow to circumvent the fact that GNURadio is
* based on a thread per block (TPB) approach
* \ingroup e4406a
*
*/
class E4406A_API dblbuff_pacer_cc : virtual public gr::block
{
public:
    typedef boost::shared_ptr<dblbuff_pacer_cc> sptr;

    /*!
    * \brief Return a shared_ptr to a new instance of e4406a::dblbuff_pacer_cc.
    *
    * To avoid accidental use of raw pointers, e4406a::dblbuff_pacer_cc's
    * constructor is in a private implementation
    * class. e4406a::dblbuff_pacer_cc::make is the public interface for
    * creating new instances.
    * 
    * \param out_size Size of output blocks
    * \param block_factor Multiplicative factor i.e. input block size is this value
    * times the size of output block.
    */
    static sptr make(unsigned out_size, unsigned block_factor);
};

} // namespace e4406a
} // namespace gr

#endif /* INCLUDED_E4406A_DBLBUFF_PACER_CC_H */


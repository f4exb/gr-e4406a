/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
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


#ifndef INCLUDED_E4406A_E4406A_SOURCE_H
#define INCLUDED_E4406A_E4406A_SOURCE_H

#include <e4406a/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace e4406a {

/*!
 * \brief Source block for E4406A VSA. Produces I/Q (Complex) samples.
 * \ingroup e4406a
 *
 */
class E4406A_API E4406A_source : virtual public gr::sync_block
{
public:
    typedef boost::shared_ptr<E4406A_source> sptr;

    /*!
    * \brief Return a shared_ptr to a new instance of e4406a::E4406A_source.
    *
    * To avoid accidental use of raw pointers, e4406a::E4406A_source's
    * constructor is in a private implementation
    * class. e4406a::E4406A_source::make is the public interface for
    * creating new instances.
    * 
    * \param ip_addr E4406A IP address
    * \param frequency Center frequency in Hz
    * \param resbw Resolution bandwidth in Hz
    * \param nb_points Number of I/Q points per E4406A block
    */
    static sptr make(const char *ip_addr, unsigned long int frequency, unsigned int resbw, unsigned int nb_points);
};

} // namespace e4406a
} // namespace gr

#endif /* INCLUDED_E4406A_E4406A_SOURCE_H */

 

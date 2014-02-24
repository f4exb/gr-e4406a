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


#ifndef INCLUDED_E4406A_SIMPLE_DECIMATOR_CC_H
#define INCLUDED_E4406A_SIMPLE_DECIMATOR_CC_H

#include <e4406a/api.h>
#include <gnuradio/sync_decimator.h>

namespace gr {
namespace e4406a {

/**
* \brief <+description of block+>
* \ingroup e4406a
*
*/
class E4406A_API simple_decimator_cc : virtual public gr::sync_decimator
{
public:
    typedef boost::shared_ptr<simple_decimator_cc> sptr;

    /**
    * \brief Return a shared_ptr to a new instance of e4406a::simple_decimator_cc.
    *
    * To avoid accidental use of raw pointers, e4406a::simple_decimator_cc's
    * constructor is in a private implementation
    * class. e4406a::simple_decimator_cc::make is the public interface for
    * creating new instances.
    * 
    * \param decim Decimation factor
    */
    static sptr make(unsigned decim);
    
    /**
     * Set dicimation factor
     * \param decim Decimation factor
     */
    virtual void set_decim(unsigned decim) = 0;
};

} // namespace e4406a
} // namespace gr

#endif /* INCLUDED_E4406A_SIMPLE_DECIMATOR_CC_H */


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

#ifndef INCLUDED_E4406A_E4406A_SOURCE_IMPL_H
#define INCLUDED_E4406A_E4406A_SOURCE_IMPL_H

#include <e4406a/E4406A_source.h>
#include "vxi11_user.h"

namespace gr {
namespace e4406a {

class E4406A_source_impl : public E4406A_source
{
private:
// Nothing to declare in this block.

public:  
    /**
    * Constructor
    * \param ip_addr E4406A IP address
    * \param frequency Center frequency in Hz
    * \param resbw Resolution bandwidth in Hz
    * \param nb_points Number of I/Q points per E4406A block
    */
    E4406A_source_impl(const char *ip_addr, unsigned long int frequency, unsigned int resbw, unsigned int nb_points);
    
    /**
     * Destructor
     * \brief releases libvxi11 resources and internal buffers
     */
    ~E4406A_source_impl();

    /**
     * Worker
     * \brief Where all the action really happens
     */
    int work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items);
   
protected:
    unsigned long int d_frequency; //!< Center frequency in Hz
    unsigned int d_resbw;          //!< Resolution bandwidth in Hz
    unsigned int d_nb_points;      //!< Number of I/Q points per E4406A block
    char  *d_ip_addr;               //!< E4406A IP Address
    CLINK d_vxi_link;                //!< VXI-11 instrument handler

    /**
     * Send command with double argument to instrument utility
     * \parm command Command fixed part (prefix)
     * \parm value Argument as floating point with double precision value
     */
    int send_command_double(const char *command, double value);

    /**
     * Send command with unsigned integer argument to instrument utility
     * \parm command Command fixed part (prefix)
     * \parm value Argument as unsigned integer point value
     */
    int send_command_u(const char *command, unsigned int value);

    /**
     * Send command with unsigned long integer argument to instrument utility
     * \parm command Command fixed part (prefix)
     * \parm value Argument as unsigned long integer point value
     */
    int send_command_ul(const char *command, unsigned long int value);
};

} // namespace e4406a
} // namespace gr

#endif /* INCLUDED_E4406A_E4406A_SOURCE_IMPL_H */


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


#ifndef INCLUDED_E4406A_E4406A_REMOTE_CONTROL_H
#define INCLUDED_E4406A_E4406A_REMOTE_CONTROL_H

#include <inttypes.h>
#include <e4406a/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace e4406a {

/*!
 * \brief Source block for E4406A VSA. Produces I/Q (Complex) samples.
 * \ingroup e4406a
 *
 */
class E4406A_API E4406A_remote_control : virtual public gr::sync_block
{
public:
    typedef boost::shared_ptr<E4406A_remote_control> sptr;

    /*!
    * \brief Return a shared_ptr to a new instance of e4406a::E4406A_remote_control.
    *
    * To avoid accidental use of raw pointers, e4406a::E4406A_remote_control's
    * constructor is in a private implementation
    * class. e4406a::E4406A_remote_control::make is the public interface for
    * creating new instances.
    * 
    * \param ip_addr E4406A IP address
    * \param frequency Center frequency in Hz
    */
    static sptr make(const std::string& ip_addr, 
            float frequency);
    
    /**
     * Set frequency
     * \param frequency Center frequency in Hz
     */
    virtual void set_frequency(float frequency) = 0;
    
    /**
     * Lock/unlock keyboard
     * \param lock 0 to unlock, != 0 to lock
     */
    virtual void key_lock(int lock) = 0;
    
    /**
     * Get frequency
     * \return Center frequency in Hz
     */
    virtual float get_frequency() = 0;
    
    /**
     * Send command without argument to instrument utility
     * \param command Command 
     * \param protect Protect communication with E4406A with the mutex
     */
    virtual void send_command(const char *command, bool protect = true) = 0;

    /**
     * Send command and get response from instrument as a double value
     * \param command Command 
     * \param protect Protect communication with E4406A with the mutex
     * \return Value returned from the command as a double
     */
    virtual double send_command_and_get_response_double(const char *command, 
            bool protect = true) = 0;
            
};

} // namespace e4406a
} // namespace gr

#endif /* INCLUDED_E4406A_E4406A_REMOTE_CONTROL_H */

 

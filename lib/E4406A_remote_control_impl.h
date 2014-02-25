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


#ifndef INCLUDED_E4406A_E4406A_REMOTE_CONTROL_IMPL_H
#define INCLUDED_E4406A_E4406A_REMOTE_CONTROL_IMPL_H

#include <e4406a/E4406A_remote_control.h>
#include <string>
#include <inttypes.h>
#include "vxi11_user.h"


namespace gr {
namespace e4406a {

/**
* \brief Remotely control an Agilent E4406A VSA
*
*/
class E4406A_remote_control_impl : public E4406A_remote_control
{
public:
    /**
     * Constructor
     * \param ip_addr IP Address to which the E4406A is connected
     * \param frequency Dummy frequency parameter
     */
    E4406A_remote_control_impl(const std::string& ip_addr,
            float frequency);
    
    /**
     * Destructor
     */
    ~E4406A_remote_control_impl();
    
    /**
     * Set frequency
     * \param frequency Center frequency in Hz
     */
    void set_frequency(float frequency);
    
    /**
     * Lock/unlock keyboard
     * \param lock 0 to unlock, != 0 to lock
     */
    void key_lock(int lock);
    
    /**
     * Get frequency
     * \return Center frequency in Hz
     */
    float get_frequency();
    
    /**
     * Get reference to the protection mutex
     */
    gr::thread::mutex& get_mutex();
    
    /**
     * Send command without argument to instrument utility
     * \param command Command 
     * \param protect Protect communication with E4406A with the mutex
     */
    void send_command(const char *command, bool protect = true);

    /**
     * Send command and get response from instrument as a double value
     * \param command Command 
     * \param protect Protect communication with E4406A with the mutex
     * \return Value returned from the command as a double
     */
    double send_command_and_get_response_double(const char *command, 
            bool protect = true);
            
    /**
     * Worker
     * \brief Dummy worker that outputs all zeroes. Just sink it to a null sink.
     * This kind of stupidity is just what GNURadio deserves. If you wanna play
     * the smart ass I'm gonna be smarter than you.
     */
    int work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items);
            
    
protected:

     std::string d_ip_addr;            //!< E4406A IP Address
     CLINK  d_vxi_link;                //!< VXI-11 instrument handler
     gr::thread::mutex d_e4406a_mutex; //!< E4406A communication protection
     
    /**
     * Send command with double argument to instrument utility
     * \param command Command fixed part (prefix)
     * \param value Argument as floating point with double precision value
     * \param protect Protect communication with E4406A with the mutex
     */
    void send_command_double(const char *command, double value, bool protect = true);

    /**
     * Send command with unsigned integer argument to instrument utility
     * \param command Command fixed part (prefix)
     * \param value Argument as unsigned integer point value
     * \param protect Protect communication with E4406A with the mutex
     */
    void send_command_u(const char *command, unsigned int value, bool protect = true);

    /**
     * Send command with unsigned long integer argument to instrument utility
     * \param command Command fixed part (prefix)
     * \param value Argument as unsigned long integer point value
     * \param protect Protect communication with E4406A with the mutex
     */
    void send_command_ul(const char *command, unsigned long int value, bool protect = true);

    /**
     * Send command and get response from instrument 
     * \param command Command 
     * \param buf Buffer for response
     * \param bufsize Size of response buffer
     * \param protect Protect communication with E4406A with the mutex
     * \return Number of bytes returned in the response
     */
    size_t send_command_and_get_response(const char *command, 
            char *buf, 
            const size_t bufsize, 
            bool protect = true);
};

} // namespace e4406a
} // namespace gr

#endif /* INCLUDED_E4406A_E4406A_REMOTE_CONTROL_IMPL_H */


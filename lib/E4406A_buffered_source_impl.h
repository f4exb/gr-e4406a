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

#ifndef INCLUDED_E4406A_E4406A_BUFFERED_SOURCE_IMPL_H
#define INCLUDED_E4406A_E4406A_BUFFERED_SOURCE_IMPL_H

#include <e4406a/E4406A_buffered_source.h>
#include <gnuradio/thread/thread.h>
#include <string>
#include "vxi11_user.h"

namespace gr {
namespace e4406a {

class E4406A_buffered_source_impl : public E4406A_buffered_source
{
private:
    // Nothing to declare in this block.

public:
    /**
    * Constructor
    * \param ip_addr E4406A IP address
    * \param frequency Center frequency in Hz
    * \param resbw Resolution bandwidth in Hz
    * \param rfgain External RF gain in dB (can be internal preamp if this mod is fitted)
    * \param nb_points Number of I/Q points per atomic block
    * \param block_factor Number of atomic blocks in buffer
    */
    E4406A_buffered_source_impl(const std::string& ip_addr, 
            float frequency, 
            float resbw,
            float rfgain, 
            uint32_t nb_points,
            uint32_t block_factor = 1);
    
    /**
     * Destructor
     * \brief releases libvxi11 resources and internal buffers
     */
    ~E4406A_buffered_source_impl();

    /**
     * Set central frequency
     * \param frequency Central frequency in Hz
     */
    void set_frequency(float frequency);

    /**
     * Set resolution bandwidth
     * \param resbw Resolution bandwidth in Hz
     */
    void set_resbw(float resbw);

    /**
     * Get ratio between resolution bandwidth and sample frequency
     * \return Ratio between resolution bandwidth and sample frequency (always < 0.533 for E4406A)
     */
     float get_resbw_ratio();
     
     /**
      * Get trace sweep time
      * \return Trace sweep time in seconds
      */
     float get_sweep_time();
     
     /**
      * Get effective sample rate
      * \return Effective sample rate in seconds
      */
     float get_samp_rate(); 
     
     /**
      * Get optimum decimation factor for FFT display
      * \return Decimation factor
      */
     int get_decim(); 
     
     /**
      * Get decimated bandwidth
      * \return Decimated bandwidth in Hz
      */
     float get_decimated_bw(); 
     
     
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
    unsigned int d_nb_points;      //!< Number of I/Q points per atomic block
    unsigned int d_block_factor;   //!< Number of atomic blocks in buffer
    unsigned int d_block_index;    //!< Current atomic block index to be served
    double d_samp_rate;             //!< Efective sample rate in seconds returned by E4406A
    double d_sweep_time;            //!< Sweep time set
    std::string d_ip_addr;          //!< E4406A IP Address
    CLINK  d_vxi_link;             //!< VXI-11 instrument handler
    char   *d_e4406a_buf;          //!< Buffer for I/Q data block returned by E4406A
    float d_rfgain;                //!< External RF gain
    size_t d_e4406a_bufsize;       //!< Size of buffer used for wave data retrieval from E4406A (:READ:WAV0?)
    size_t d_e4406a_bufsize_iq;    //!< Size of buffer for I/Q data block for strictly necessary data
    size_t d_counter_size;         //!< Size of block counter returned by E4406A
    bool d_init;                   //!< Initial block fetch from E4406A
    gr::thread::mutex d_e4406a_mutex; //!< E4406A communication protection

    /**
     * Send command without argument to instrument utility
     * \param command Command 
     * \param protect Protect communication with E4406A with the mutex
     */
    void send_command(const char *command, bool protect = true);

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

    /**
     * Send command and get response from instrument as a double value
     * \param command Command 
     * \param value Pointer to returned value placeholder
     * \param protect Protect communication with E4406A with the mutex
     */
    void send_command_and_get_response_double(const char *command, 
            double *value, 
            bool protect = true);

    /**
     * Set bandwidth and sweep time
     * \brief Sets bandwidth and sweep time according to values given in bandwidth
     * and number of points attributes. Updates bandwidth with bandwidth set by the 
     * instrument.
     */
    void set_bandwidth_and_sweep_time();

    /**
     * Set RF gain
     * \brief Uses external attenuator setting with the opposite value
     * \param rfgain Gain in dB
     */
    void set_rfgain(float rfgain);
        
};

} // namespace e4406a
} // namespace gr

#endif /* INCLUDED_E4406A_E4406A_BUFFERED_SOURCE_IMPL_H */


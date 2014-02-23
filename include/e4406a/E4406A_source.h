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
    * \param rfgain External RF gain in dB (can be internal preamp if this mod is fitted)
    */
    static sptr make(const std::string& ip_addr, 
            float frequency, 
            float resbw, 
            uint32_t nb_points,
            float rfgain);
    
    /**
     * Set central frequency
     * \param frequency Central frequency in Hz
     */
    virtual void set_frequency(float frequency) = 0;

    /**
     * Set resolution bandwidth
     * \param resbw Resolution bandwidth in Hz
     */
    virtual void set_resbw(float resbw) = 0;

    /**
     * Set RF gain
     * \brief Uses external attenuator setting with the opposite value
     * \param rfgain Gain in dB
     */
    virtual void set_rfgain(float rfgain) = 0;
    
    /**
     * Get ratio between resolution bandwidth and sample frequency
     * \return Ratio between resolution bandwidth and sample frequency (always < 0.533 for E4406A)
     */
     virtual float get_resbw_ratio() = 0;
     
     /**
      * Get trace sweep time
      * \return Trace sweep time in seconds
      */
     virtual float get_sweep_time() = 0;
     
     /**
      * Get effective sample rate
      * \return Effective sample rate in seconds
      */
     virtual float get_samp_rate() = 0; 
     
     /**
      * Get optimum decimation factor for FFT display
      * \return Decimation factor
      */
     virtual int get_decim() = 0; 
     
     /**
      * Get decimated bandwidth
      * \return Decimated bandwidth in Hz
      */
     virtual float get_decimated_bw() = 0; 
     
};

} // namespace e4406a
} // namespace gr

#endif /* INCLUDED_E4406A_E4406A_SOURCE_H */

 

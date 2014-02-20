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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "E4406A_source_impl.h"

namespace gr {
namespace e4406a { 

// ================================================================================================
E4406A_source::sptr E4406A_source::make(const char *ip_addr, unsigned long int frequency, unsigned int resbw, unsigned int nb_points)
{
  return gnuradio::get_initial_sptr
    (new E4406A_source_impl(ip_addr, frequency, resbw, nb_points));
}

// ================================================================================================
E4406A_source_impl::E4406A_source_impl(const char *ip_addr, unsigned long int frequency, unsigned int resbw, unsigned int nb_points) 
  : gr::sync_block("E4406A_source",
    gr::io_signature::make(0, 0, 0),
    gr::io_signature::make(1, 1, sizeof(gr_complex))),
    d_frequency(frequency),
    d_resbw(resbw),
    d_nb_points(nb_points)
{
    const size_t MAXSIZE = 256;
    char rcv[MAXSIZE];
    
    d_ip_addr = strdup(ip_addr);
    
    int ret = vxi11_open_device(ip_addr, &d_vxi_link);
    
    if (ret != 0) 
    {
        std::cerr << "E4406A: cannot open VXI-11 channel" << std::endl;
        throw std::runtime_error("cannot open VXI-11 channel");
    }
    
    if (send_command_ul(":FREQ:CENT", d_frequency) < 0)
    {
        std::cerr << "E4406A: cannot set frequency" << d_frequency << std::endl;
        throw std::runtime_error("cannot set frequency");
    }
    
    if (send_command_u(":WAV:BWID", d_resbw) < 0)
    {
        std::cerr << "E4406A: cannot set resolution bandwidth" << d_resbw << std::endl;
        throw std::runtime_error("cannot set resolution bandwidth");
    }
    
    int found = vxi11_send_and_receive(&d_vxi_link, ":WAV:APER?", rcv, MAXSIZE, 10);
    
    if (found > 0)
    {
        rcv[found] = '\0';
    }
    else
    {
        std::cerr << "E4406A: cannot get effective sample rate" << std::endl;
        throw std::runtime_error("cannot get effective sample rate");
    }
    
    double samp_rate;
    sscanf(rcv, "%lf", &samp_rate);
    double sweep_time = samp_rate * (d_nb_points - 1);
    
    if (send_command_double(":WAV:SWE:TIME", sweep_time) < 0)
    {
        std::cerr << "E4406A: cannot set sweep time" << sweep_time << std::endl;
        throw std::runtime_error("cannot set sweep time");
    }
    
    gr::block::set_output_multiple(d_nb_points); // Make sure noutput_items will be a multiple of E4406A I/Q block size
}

// ================================================================================================
E4406A_source_impl::~E4406A_source_impl()
{
    vxi11_close_device(d_ip_addr, &d_vxi_link);
    free(d_ip_addr);
}

// ================================================================================================
int E4406A_source_impl::work(int noutput_items,
    gr_vector_const_void_star &input_items,
    gr_vector_void_star &output_items)
{
    gr_complex *out = (gr_complex *) output_items[0];

    // Do <+signal processing+>

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

// ================================================================================================
int E4406A_source_impl::send_command_double(const char *command, double value)
{
    char cmd[64];
    
    sprintf(cmd, "%s %.6lf", command, value);
    return vxi11_send(&d_vxi_link, cmd);
}

// ================================================================================================
int E4406A_source_impl::send_command_u(const char *command, unsigned int value)
{
    char cmd[64];
    
    sprintf(cmd, "%s %u", command, value);
    return vxi11_send(&d_vxi_link, cmd);
}

// ================================================================================================
int E4406A_source_impl::send_command_ul(const char *command, unsigned long int value)
{
    char cmd[64];
    
    sprintf(cmd, "%s %lu", command, value);
    return vxi11_send(&d_vxi_link, cmd);
}

} /* namespace e4406a */
} /* namespace gr */


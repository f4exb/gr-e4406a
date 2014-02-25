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
#include <iostream>
#include "E4406A_remote_control_impl.h"

namespace gr {
namespace e4406a {

// ================================================================================================
E4406A_remote_control::sptr E4406A_remote_control::make(const std::string& ip_addr,
        float frequency)
{
    return gnuradio::get_initial_sptr(new E4406A_remote_control_impl(ip_addr, frequency));
}

//==================================================================================================
E4406A_remote_control_impl::E4406A_remote_control_impl(const std::string& ip_addr,
        float frequency)
  : gr::sync_block("E4406A_remote_control",
            gr::io_signature::make(0, 0, 0),
            gr::io_signature::make(1, 1, sizeof(gr_complex))),
    d_ip_addr(ip_addr)
{
    int ret = vxi11_open_device(ip_addr.c_str(), &d_vxi_link); // try to establish VXI-11 communication link with E4406A
    
    if (ret != 0) 
    {
        std::cerr << "E4406A: cannot open VXI-11 channel" << std::endl;
        throw std::runtime_error("cannot open VXI-11 channel");
    }
}

//==================================================================================================
E4406A_remote_control_impl::~E4406A_remote_control_impl()
{
    vxi11_close_device(d_ip_addr.c_str(), &d_vxi_link); // close VXI-11 communication link with E4406A
}

// ================================================================================================
void E4406A_remote_control_impl::send_command(const char *command, bool protect)
{
    int ret;
 
    if (protect)
    {
        gr::thread::scoped_lock guard(d_e4406a_mutex); // protect communication with E4406A
        ret = vxi11_send(&d_vxi_link, command);
    }
    else
    {
        ret = vxi11_send(&d_vxi_link, command);
    }
    
    if (ret < 0)
    {
        std::cerr << "E4406A: cannot send command \"" << command << "\"" << std::endl;
        throw std::runtime_error("cannot send command to E4406A");
    }    
}

// ================================================================================================
void E4406A_remote_control_impl::send_command_double(const char *command, double value, bool protect)
{    char cmd[64];
    
    sprintf(cmd, "%s %.6lf", command, value);
    send_command(cmd, protect);
}

// ================================================================================================
void E4406A_remote_control_impl::send_command_u(const char *command, unsigned int value, bool protect)
{
    char cmd[64];
    
    sprintf(cmd, "%s %u", command, value);
    send_command(cmd, protect);
}

// ================================================================================================
void E4406A_remote_control_impl::send_command_ul(const char *command, unsigned long int value, bool protect)
{
    char cmd[64];
    
    sprintf(cmd, "%s %lu", command, value);
    send_command(cmd, protect);
}

// ================================================================================================
size_t E4406A_remote_control_impl::send_command_and_get_response(const char *command, 
        char *buf, 
        const size_t bufsize, 
        bool protect)
{
    int ret, bytes_returned;
    
    if (protect)
    {
        gr::thread::scoped_lock guard(d_e4406a_mutex); // protect communication with E4406A
        ret = vxi11_send(&d_vxi_link, command);
        bytes_returned = vxi11_receive(&d_vxi_link, buf, bufsize);
    }
    else
    {
        ret = vxi11_send(&d_vxi_link, command);
        bytes_returned = vxi11_receive(&d_vxi_link, buf, bufsize);
    }
    
    if (ret < 0)
    {
        std::cerr << "E4406A: cannot send command \"" << command << "\"" << std::endl;
        throw std::runtime_error("cannot send command to E4406A");
    }
    else if (bytes_returned > 0)
    {
        return bytes_returned;
    }
    else
    {
        std::cerr << "E4406A: no reply to command \"" << command << "\"" << std::endl;
        throw std::runtime_error("cannot get reply from E4406A");
    }    
}

// ================================================================================================
double E4406A_remote_control_impl::send_command_and_get_response_double(const char *command, 
        bool protect)
{
    const size_t rcvsize = 64;
    char rcv[rcvsize];
    double value;
    
    size_t bytes_returned = send_command_and_get_response(command, rcv, rcvsize, protect);
    rcv[bytes_returned] = '\0';
    sscanf(rcv, "%lf", &value);
    
    return value;
}

//==================================================================================================
void E4406A_remote_control_impl::set_frequency(float frequency)
{
    uint64_t intfrequency = frequency;
    send_command_ul(":FREQ:CENT", intfrequency); // set central frequency
}

//==================================================================================================
void E4406A_remote_control_impl::key_lock(int lock)
{
    if (lock)
    {
        send_command(":SYST:KLOC 1"); // inhibit keyboard
    }
    else
    {
        send_command(":SYST:KLOC 0"); // unlock keyboard
    }
}

//==================================================================================================
float E4406A_remote_control_impl::get_frequency()
{
    double e4406a_freq;
    return send_command_and_get_response_double(":FREQ:CENT?"); 
}

//==================================================================================================
gr::thread::mutex& E4406A_remote_control_impl::get_mutex()
{
    return d_e4406a_mutex;
}

// ================================================================================================
int E4406A_remote_control_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
{
    gr_complex *out = (gr_complex *) output_items[0];
    
    for (unsigned int i=0; i<noutput_items; i++)
    {
        out[i].real() = 0.0;
        out[i].imag() = 0.0;
    }
    
    return noutput_items;
}

} /* namespace e4406a */
} /* namespace gr */


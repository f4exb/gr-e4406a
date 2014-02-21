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
#include <cstdlib>
#include "E4406A_source_impl.h"

namespace gr {
namespace e4406a { 

// ================================================================================================
E4406A_source::sptr E4406A_source::make(const std::string& ip_addr, uint64_t frequency, uint32_t resbw, uint32_t nb_points)
{
    return gnuradio::get_initial_sptr(new E4406A_source_impl(ip_addr, frequency, resbw, nb_points));
}

// ================================================================================================
E4406A_source_impl::E4406A_source_impl(const std::string& ip_addr, uint64_t frequency, uint32_t resbw, uint32_t nb_points) 
  : gr::sync_block("E4406A_source",
            gr::io_signature::make(0, 0, 0),
            gr::io_signature::make(1, 1, sizeof(gr_complex))),
    d_frequency(frequency),
    d_resbw(resbw),
    d_nb_points(nb_points),
    d_ip_addr(ip_addr)
{
    const size_t MAXSIZE = 256;
    char rcv[MAXSIZE];
    
    int ret = vxi11_open_device(ip_addr.c_str(), &d_vxi_link); // try to establish VXI-11 communication link with E4406A
    
    if (ret != 0) 
    {
        std::cerr << "E4406A: cannot open VXI-11 channel" << std::endl;
        throw std::runtime_error("cannot open VXI-11 channel");
    }
    
    send_command(":INST:NSEL 8;:CONF:WAV"); // config for I/Q output
    send_command(":INIT:CONT 0;:FORM:BORD SWAP;:FORM REAL, 32;:WAV:BWID:TYPE FLAT;:CAL:TCOR ON;:CAL:AUTO ALERT;:STAT:OPER:ENAB 32"); // other settings
    send_command(":SYST:KLOC 1"); // inhibit keyboard
    send_command(":SYST:MESS \"In Use by GNU Radio - front panel disabled\""); // send informatory message to E4406A status line
    send_command(":DISP:ENAB 0"); // inhibit display
    
    send_command_ul(":FREQ:CENT", d_frequency); // set central frequency
    send_command_u(":WAV:BWID", d_resbw);       // set resolution bandwidth
    
    send_command(":WAV:APER?");
    int bytes_returned = vxi11_receive(&d_vxi_link, rcv, MAXSIZE);
    
    if (bytes_returned > 0)
    {
        rcv[bytes_returned] = '\0';
    }
    else
    {
        std::cerr << "E4406A: cannot get effective sample rate" << std::endl;
        throw std::runtime_error("cannot get effective sample rate");
    }
    
    double samp_rate;
    sscanf(rcv, "%lf", &samp_rate);
    double sweep_time = samp_rate * (d_nb_points - 1);
    
    send_command_double(":WAV:SWE:TIME", sweep_time); // set sweep time according to requested I/Q block size
    gr::block::set_output_multiple(d_nb_points); // make sure noutput_items will be a multiple of E4406A I/Q block size
    
    d_e4406a_bufsize_iq = d_nb_points*8;              // I/Q samples buffer size
    d_e4406a_bufsize = d_e4406a_bufsize_iq + 16;      // waveform buffer size to dialog with E4406A
    d_e4406a_buf = (char *) malloc(d_e4406a_bufsize); // allocate memory for I/Q block returned by E4406A
}

// ================================================================================================
E4406A_source_impl::~E4406A_source_impl()
{
    send_command(":DISP:ENAB 1"); // release display
    send_command(":SYST:KLOC 0"); // release keyboard
    send_command(":INIT:CONT 1"); // restore continuous measurement
    send_command(":CAL:AUTO ON"); // restore auto calibration
    
    vxi11_close_device(d_ip_addr.c_str(), &d_vxi_link); // close VXI-11 communication link with E4406A
    
    free(d_e4406a_buf); // release E4406A I/Q buffer
}

// ================================================================================================
int E4406A_source_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
{
    gr_complex *out = (gr_complex *) output_items[0];

    // Fetch data from E4406A and do signal processing
    
    send_command(":READ:WAV0?");
    int bytes_returned_i = vxi11_receive(&d_vxi_link, d_e4406a_buf, d_e4406a_bufsize);
    
    if (bytes_returned_i < 2)
    {
        std::cerr << "E4406A: I/Q data size less than 2 bytes (" << bytes_returned_i << " bytes)" << std::endl;
        throw std::runtime_error("invalid size for I/Q data");
    }
    else
    {
        unsigned int bytes_returned = bytes_returned_i;
        char counter_str[8];
        
        if (d_e4406a_buf[0] != '#')
        {
            std::cerr << "E4406A: invalid magic character for I/Q data (" << d_e4406a_buf[0] << " bytes)" << std::endl;
            throw std::runtime_error("invalid magic character for I/Q data");
        }
        
        strncpy(counter_str, &d_e4406a_buf[1], 1);
        counter_str[1] = '\0';
        
        unsigned int counter_size = strtoul(counter_str, 0, 10);
        
        if (bytes_returned < 2+counter_size)
        {
            std::cerr << "E4406A: I/Q data size less than header data size (" << bytes_returned << " bytes)" << std::endl;
            throw std::runtime_error("invalid size for I/Q data");
        }
        else
        {
            strncpy(counter_str, &d_e4406a_buf[2], counter_size);
            counter_str[counter_size] = '\0';
            
            unsigned int  byte_count = strtoul(counter_str, 0, 10);
            
            if (bytes_returned < 2+counter_size+byte_count)
            {
                std::cerr << "E4406A: I/Q data block truncated to " << bytes_returned-2-counter_size << " bytes" << std::endl;
                throw std::runtime_error("invalid size for I/Q data");
            }
            else if (byte_count < d_e4406a_bufsize_iq)
            {
                std::cerr << "E4406A: I/Q data block too small (" << bytes_returned-2-counter_size << " bytes)" << std::endl;
                throw std::runtime_error("invalid size for I/Q data");
            }
            else
            {
                float *iq_array = (float *) &d_e4406a_buf[2+counter_size];
                
                for (unsigned int i=0; i<d_nb_points; i++)
                {
                    out[i].real() = iq_array[2*i];
                    out[i].imag() = iq_array[2*i+1];
                }
                
                noutput_items = d_nb_points;
            }
        }
    }

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

// ================================================================================================
void E4406A_source_impl::send_command(const char *command)
{
    if (vxi11_send(&d_vxi_link, command) < 0)
    {
        std::cerr << "E4406A: cannot send command (" << command << ")" << std::endl;
        throw std::runtime_error("cannot send command to E4406A");
    }
    
}

// ================================================================================================
void E4406A_source_impl::send_command_double(const char *command, double value)
{    char cmd[64];
    
    sprintf(cmd, "%s %.6lf", command, value);
    
    if (vxi11_send(&d_vxi_link, cmd) < 0)
    {
        std::cerr << "E4406A: cannot send command with double argument (" << cmd << ")" << std::endl;
        throw std::runtime_error("cannot send command to E4406A");
    }
}

// ================================================================================================
void E4406A_source_impl::send_command_u(const char *command, unsigned int value)
{
    char cmd[64];
    
    sprintf(cmd, "%s %u", command, value);
    
    if (vxi11_send(&d_vxi_link, cmd) < 0)
    {
        std::cerr << "E4406A: cannot send command with unsigned int argument (" << cmd << ")" << std::endl;
        throw std::runtime_error("cannot send command to E4406A");
    }
}

// ================================================================================================
void E4406A_source_impl::send_command_ul(const char *command, unsigned long int value)
{
    char cmd[64];
    
    sprintf(cmd, "%s %lu", command, value);
    
    if (vxi11_send(&d_vxi_link, cmd) < 0)
    {
        std::cerr << "E4406A: cannot send command with unsigned long int argument (" << cmd << ")" << std::endl;
        throw std::runtime_error("cannot send command to E4406A");
    }
}

} /* namespace e4406a */
} /* namespace gr */


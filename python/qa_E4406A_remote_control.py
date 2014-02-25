#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2014 Edouard Greiffiths, F4EXB.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import e4406a_swig as e4406a

class qa_E4406A_remote_control (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        vsa = e4406a.E4406A_remote_control("192.168.0.101", 1e9)
        head = blocks.head(gr.sizeof_gr_complex*1, 128)
        dst = blocks.vector_sink_c()
        
        # useful job
        vsa.set_frequency(2.375e9)
        e4406a_freq = vsa.get_frequency()
        
        # connect the crap and let it run miserably like the miserable poor shit it is
        self.tb.connect(vsa, head)
        self.tb.connect(head, dst)
        self.tb.run ()

        # check data
        self.assertAlmostEqual(e4406a_freq/1e6, 2375.0, 3)

if __name__ == '__main__':
    gr_unittest.run(qa_E4406A_remote_control, "qa_E4406A_remote_control.xml")

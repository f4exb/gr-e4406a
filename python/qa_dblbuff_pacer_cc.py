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

class qa_dblbuff_pacer_cc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        src = blocks.null_source(gr.sizeof_gr_complex*1)
        pacer = e4406a.dblbuff_pacer_cc(128, 4)
        head = blocks.head(gr.sizeof_gr_complex*1, 128*4*2)
        dst = blocks.vector_sink_c()
        # set up fg
        self.tb.connect(src, pacer)
        self.tb.connect(pacer, head)
        self.tb.connect(head, dst)
        self.tb.run ()
        # check data
        result_data = dst.data()
        print "# result points:", len(result_data) 
        self.assertEqual(len(result_data), 128*4*2)

if __name__ == '__main__':
    gr_unittest.run(qa_dblbuff_pacer_cc, "qa_dblbuff_pacer_cc.xml")

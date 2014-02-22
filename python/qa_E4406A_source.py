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

class qa_E4406A_source (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        vsa = e4406a.E4406A_source("192.168.0.101", 1000000000, 8000000, 4096, 0)
        head = blocks.head(gr.sizeof_gr_complex*1, 4096)
        dst = blocks.vector_sink_c()
        self.tb.connect(vsa, head)
        self.tb.connect(head, dst)
        self.tb.run ()

        # check data
        result_data = dst.data()

        print "# result I/Q points:", len(result_data) 

        for result_point in result_data:
            if result_point != 0:
                print result_point

        self.assertEqual(len(result_data), 4096)

if __name__ == '__main__':
    gr_unittest.run(qa_E4406A_source, "qa_E4406A_source.xml")

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

class qa_simple_decimator_cc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up data
        # you need exactly decim times the expected result elements (here 9 for 3)
        # (-5.5+0j, -3+0j, 4+0j, 4.5+0j, 2+0j, 3+0j, 7+0j, -2+0j) -> (-5.5+0j, 4.5+0j)
        src_data = (-5.5+0j, -3+0j, 4+0j, 4.5+0j, 2+0j, 3+0j, 7+0j, -2+0j, 1+0j)
        expected_result = (-5.5+0j, 4.5+0j, 7+0j)
        # set up blocks
        src = blocks.vector_source_c(src_data)
        decim = e4406a.simple_decimator_cc(3)
        dst = blocks.vector_sink_c()
        # set up fg
        self.tb.connect(src, decim)
        self.tb.connect(decim, dst)
        self.tb.run ()
        result_data = dst.data()
        # check data
        self.assertEqual(result_data, expected_result)

    def test_002_t (self):
        # set up data
        src_data = (-5.5+0j, -3+0j, 4+0j, 4.5+0j, 2+0j, 3+0j, 7+0j, -2+0j, 1+0j)
        expected_result = (-5.5+0j, 4+0j, 2+0j, 7+0j)
        # set up blocks
        src = blocks.vector_source_c(src_data)
        decim = e4406a.simple_decimator_cc(3)
        dst = blocks.vector_sink_c()
        # set up fg
        self.tb.connect(src, decim)
        self.tb.connect(decim, dst)
        decim.set_decim(2)
        self.tb.run ()
        result_data = dst.data()
        # check data
        self.assertEqual(result_data, expected_result)


if __name__ == '__main__':
    gr_unittest.run(qa_simple_decimator_cc, "qa_simple_decimator_cc.xml")

/* -*- c++ -*- */

#define E4406A_API

%include <stdint.i>   // the inttypes for uint64_t, uint32_t
%include "gnuradio.i" // the common stuff

//load generated python docstrings
%include "e4406a_swig_doc.i"

%{
#include "e4406a/E4406A_source.h"
#include "e4406a/E4406A_remote_control.h"
#include "e4406a/simple_decimator_cc.h"
#include "e4406a/E4406A_buffered_source.h"
%}


%include "e4406a/E4406A_source.h"
%include "e4406a/E4406A_remote_control.h"
%include "e4406a/simple_decimator_cc.h"

GR_SWIG_BLOCK_MAGIC2(e4406a, E4406A_source);
GR_SWIG_BLOCK_MAGIC2(e4406a, E4406A_remote_control);
GR_SWIG_BLOCK_MAGIC2(e4406a, simple_decimator_cc);
%include "e4406a/E4406A_buffered_source.h"
GR_SWIG_BLOCK_MAGIC2(e4406a, E4406A_buffered_source);

/* -*- c++ -*- */

#define E4406A_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "e4406a_swig_doc.i"

%{
#include "e4406a/E4406A_source.h"
%}


%include "e4406a/E4406A_source.h"
GR_SWIG_BLOCK_MAGIC2(e4406a, E4406A_source);

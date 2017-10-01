/* Copyright (C) 2001-2006 Artifex Software, Inc.
   All Rights Reserved.
  
  This file is part of GNU ghostscript

  GNU ghostscript is free software; you can redistribute it and/or
  modify it under the terms of the version 2 of the GNU General Public
  License as published by the Free Software Foundation.

  GNU ghostscript is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with
  ghostscript; see the file COPYING. If not, write to the Free Software Foundation,
  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

*/

/* $Id: zfile1.c,v 1.9 2008/03/23 15:28:00 Arabidopsis Exp $ */
/* Special file operators */

#include "memory_.h"
#include "string_.h"
#include "ghost.h"
#include "gp.h"
#include "ierrors.h"
#include "oper.h"
#include "ialloc.h"
#include "opdef.h"
#include "opcheck.h"
#include "store.h"
#include "gpmisc.h"

/* <string> <string> <bool> .file_name_combine <string> true */
/* <string> <string> <bool> .file_name_combine <string> <string> false */
static int
zfile_name_combine(i_ctx_t *i_ctx_p)
{
    uint plen, flen, blen, blen0;
    const byte *prefix, *fname;
    byte *buffer;
    os_ptr op = osp;
    bool no_sibling;

    check_type(op[ 0], t_boolean);
    check_type(op[-1], t_string);
    check_type(op[-2], t_string);
    plen = r_size(op - 2);
    flen = r_size(op - 1);
    blen = blen0 = plen + flen + 2; /* Inserts separator and ending zero byte. */
    buffer = ialloc_string(blen, "zfile_name_combine");
    if (buffer == 0)
	return_error(e_VMerror);
    prefix = op[-2].value.const_bytes;
    fname =  op[-1].value.const_bytes;
    no_sibling = op[0].value.boolval;
    if (gp_file_name_combine((const char *)prefix, plen, 
			     (const char *)fname, flen, no_sibling,
		             (char *)buffer, &blen) != gp_combine_success) {
	make_bool(op, false);
    } else {
	buffer = iresize_string(buffer, blen0, blen, "zfile_name_combine");
	if (buffer == 0)
	    return_error(e_VMerror);
	make_string(op - 2, a_all | icurrent_space, blen, buffer);
	make_bool(op - 1, true);
	pop(1);
    }
    return 0;
}

/* This is compiled conditionally to let PS library to know
 * whether it works with the new gp_combine_file_name.
 */

/* <string> .file_name_is_absolute <bool> */
static int
zfile_name_is_absolute(i_ctx_t *i_ctx_p)
{   os_ptr op = osp;

    check_type(op[0], t_string);
    make_bool(op, gp_file_name_is_absolute((const char *)op->value.const_bytes, 
					r_size(op)));
    return 0;
}

static int
push_string(i_ctx_t *i_ctx_p, const char *v)
{   os_ptr op = osp;
    int len = strlen(v);

    push(1);
    make_const_string(op, avm_foreign | a_readonly,
		      len, (const byte *)v);
    return 0;
}

/* - .file_name_separator <string> */
static int
zfile_name_separator(i_ctx_t *i_ctx_p)
{   return push_string(i_ctx_p, gp_file_name_separator());
}

/* - .file_name_directory_separator <string> */
static int
zfile_name_directory_separator(i_ctx_t *i_ctx_p)
{   return push_string(i_ctx_p, gp_file_name_directory_separator());
}

/* - .file_name_current <string> */
static int
zfile_name_current(i_ctx_t *i_ctx_p)
{   return push_string(i_ctx_p, gp_file_name_current());
}

/* - .file_name_parent <string> */
static int
zfile_name_parent(i_ctx_t *i_ctx_p)
{   return push_string(i_ctx_p, gp_file_name_parent());
}

const op_def zfile1_op_defs[] =
{
    {"0.file_name_combine", zfile_name_combine},
    {"0.file_name_is_absolute", zfile_name_is_absolute},
    {"0.file_name_separator", zfile_name_separator},
    {"0.file_name_directory_separator", zfile_name_directory_separator},
    {"0.file_name_current", zfile_name_current},
    {"0.file_name_parent", zfile_name_parent},
    op_def_end(0)
};

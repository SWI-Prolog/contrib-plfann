/*
Fast Artificial Neural Network Library (fann)
Copyright (C) 2003 Steffen Nissen (lukesky@diku.dk)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __fann_swi_h__
#define __fann_swi_h__

#ifdef BUILD_FANN_WITH_SWI

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <SWI-Prolog.h>
#include <SWI-Stream.h>


#if !HAVE_EXIT_SUCCESS_AND_FAILURE
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#endif

enum line {

	AT_START = 0,
	NOT_AT_START
};


// Make the library work with both native and
// SWI-Prolog (via messages) console printing.


unsigned int FANN_API fann_swi_printf ( const char * format_string, ... ) {

	unsigned int length, exit = EXIT_SUCCESS;
	enum enum_fann_mode mode = fann_print_mode ( FANN_GET_MODE );
	static enum line next_position = AT_START;
	term_t message_pt, out_pt, type_pt;
	static char buff[ sizeof ( char * ) << 10 ];
	va_list elements;

	va_start ( elements, format_string );

	if ( mode == FANN_NATIVE )

		vprintf ( format_string, elements );

	else {

		buff[0] = '\0';
		length = vsnprintf ( buff, sizeof ( buff ) - sizeof ( char * ),
			                 format_string, elements );

		// Message truncated, extend buffer?
		// But how? No va_copy in M$VC!
		if ( length == -1 )
			exit = EXIT_FAILURE;

		message_pt = PL_new_term_ref ();
		out_pt = PL_new_term_ref ();
		type_pt = PL_new_term_ref ();
		PL_put_string_chars ( message_pt, buff );

		if ( next_position == AT_START )

				PL_cons_functor ( out_pt, 
					              PL_new_functor( PL_new_atom ( "print_message" ), 1 ), 
								  message_pt );
		else
				PL_cons_functor ( out_pt, 
					              PL_new_functor( PL_new_atom ( "print_message_at_same_line" ), 1 ), 
								  message_pt );

		if ( buff[ length - 1 ] == '\n' )
			
			next_position = AT_START;
		else
			next_position = NOT_AT_START;
			
		PL_put_atom_chars ( type_pt, "informational" );
		PL_cons_functor ( message_pt, PL_new_functor( PL_new_atom ( "plfann" ), 1 ), out_pt );
		PL_cons_functor ( out_pt, PL_new_functor( PL_new_atom ( "print_message" ), 2 ), type_pt, message_pt );
		PL_call ( out_pt, NULL );
	}

	va_end ( elements );

	return exit;
}

#define printf fann_swi_printf

#endif /* end BUILD_FANN_WITH_SWI */

#endif

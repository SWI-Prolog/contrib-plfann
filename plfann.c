
/*
  Fast Artificial Neural Network Library (fann)
  Copyright (C) 2003 Steffen Nissen (lukesky@diku.dk)

  Binding for SWI Prolog 5 (http://www.swi-prolog.org)

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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <SWI-Prolog.h>
#include <SWI-Stream.h>
#include "plfann.h"


#ifdef FANN_FLOAT
__inline int PL_get_float32 ( term_t in, float* out ) {

	double tmp;
	int exit = PL_get_float ( in, &tmp );

	out[0] = (float) tmp;

	return exit;
}
#endif


enum fann_activationfunc_enum lookup_activationfunc_enum ( char *type ) {

	if ( !strcmp ( "FANN_ELLIOT", type ) ) return FANN_ELLIOT;
	if ( !strcmp ( "FANN_ELLIOT_SYMMETRIC", type ) ) return FANN_ELLIOT_SYMMETRIC;
	if ( !strcmp ( "FANN_SIGMOID_SYMMETRIC", type ) ) return FANN_SIGMOID_SYMMETRIC;
	if ( !strcmp ( "FANN_SIGMOID_SYMMETRIC_STEPWISE", type ) ) return FANN_SIGMOID_SYMMETRIC_STEPWISE;
	if ( !strcmp ( "FANN_SIGMOID", type ) ) return FANN_SIGMOID;
	if ( !strcmp ( "FANN_SIGMOID_STEPWISE", type ) ) return FANN_SIGMOID_STEPWISE;
	if ( !strcmp ( "FANN_LINEAR", type ) ) return FANN_LINEAR;
	if ( !strcmp ( "FANN_THRESHOLD", type ) ) return FANN_THRESHOLD;
	if ( !strcmp ( "FANN_THRESHOLD_SYMMETRIC", type ) ) return FANN_THRESHOLD_SYMMETRIC;
	if ( !strcmp ( "FANN_GAUSSIAN", type ) ) return FANN_GAUSSIAN;
	if ( !strcmp ( "FANN_GAUSSIAN_SYMMETRIC", type ) ) return FANN_GAUSSIAN_SYMMETRIC;
	if ( !strcmp ( "FANN_GAUSSIAN_STEPWISE", type ) ) return FANN_GAUSSIAN_STEPWISE;
	if ( !strcmp ( "FANN_LINEAR_PIECE", type ) ) return FANN_LINEAR_PIECE;
	if ( !strcmp ( "FANN_LINEAR_PIECE_SYMMETRIC", type ) ) return FANN_LINEAR_PIECE_SYMMETRIC;
	if ( !strcmp ( "FANN_SIN_SYMMETRIC", type ) ) return FANN_SIN_SYMMETRIC;
	if ( !strcmp ( "FANN_COS_SYMMETRIC", type ) ) return FANN_COS_SYMMETRIC;
	if ( !strcmp ( "FANN_SIN", type ) ) return FANN_SIN;
	if ( !strcmp ( "FANN_COS", type ) ) return FANN_COS;

	// Error, needs to be treated
	return (enum fann_activationfunc_enum) -1;
}


#ifndef VERSION220
static char const *const FANN_ERROR_CODES[19] = {
#else
static char const *const FANN_ERROR_CODES[21] = {
#endif

	"FANN_E_NO_ERROR",
	"FANN_E_CANT_OPEN_CONFIG_R",
	"FANN_E_CANT_OPEN_CONFIG_W",
	"FANN_E_WRONG_CONFIG_VERSION",
	"FANN_E_CANT_READ_CONFIG",
	"FANN_E_CANT_READ_NEURON",
	"FANN_E_CANT_READ_CONNECTIONS",
	"FANN_E_WRONG_NUM_CONNECTIONS",
	"FANN_E_CANT_OPEN_TD_W",
	"FANN_E_CANT_OPEN_TD_R",
	"FANN_E_CANT_READ_TD",
	"FANN_E_CANT_ALLOCATE_MEM",
	"FANN_E_CANT_TRAIN_ACTIVATION",
	"FANN_E_CANT_USE_ACTIVATION",
	"FANN_E_TRAIN_DATA_MISMATCH",
	"FANN_E_CANT_USE_TRAIN_ALG",
	"FANN_E_TRAIN_DATA_SUBSET",
	"FANN_E_INDEX_OUT_OF_BOUND",
	"FANN_E_SCALE_NOT_PRESENT"

#ifdef VERSION220
	,
	"FANN_E_INPUT_NO_MATCH",
	"FANN_E_OUTPUT_NO_MATCH"

#endif

};


#ifndef VERSION220
static char const *const FANN_ERROR_STRING[19] = {
#else
static char const *const FANN_ERROR_STRING[21] = {
#endif

	"No error.",
	"Unable to open configuration file for reading.",
	"Unable to open configuration file for writing.",
	"Wrong version of configuration file.",
	"Error reading info from configuration file.",
	"Error reading neuron info from configuration file.",
	"Error reading connections from configuration file.",
	"Number of connections not equal to the number expected.",
	"Unable to open train data file for writing.",
	"Unable to open train data file for reading.",
	"Error reading training data from file.",
	"Unable to allocate memory.",
	"Unable to train with the selected activation function.",
	"Unable to use the selected activation function.",
	"Irreconcilable differences between two struct fann_train_data structures.",
	"Unable to use the selected training algorithm.",
	"Trying to take subset which is not within the training set.",
	"Index is out of bound.",
	"Scaling parameters not present."

#ifdef VERSION220
	,
	"The number of input neurons in the ann and data don’t match.",
	"The number of output neurons in the ann and data don’t match"

#endif

};


// In module plfann.pl: fann_create_standard


foreign_t swi_fann_create_standard_array ( term_t layers_pt, term_t ann_pt ) {

	unsigned int nl = 0;
	unsigned int l[MAXLAYERS];
	term_t layer_pt = PL_new_term_ref ();

	while ( PL_get_list (layers_pt, layer_pt, layers_pt) ) {

		PL_get_integer ( layer_pt, l+nl++ );
		}

	return PL_unify_pointer ( ann_pt, fann_create_standard_array ( nl, l ) );
}


// In module plfann.pl: fann_create_sparse


foreign_t swi_fann_create_sparse_array ( term_t connection_rate_pt, term_t layers_pt, term_t ann_pt ) {

	unsigned int nl = 0;
	unsigned int l[MAXLAYERS];
	double connection_rate;
	term_t layer_pt = PL_new_term_ref ();

	PL_get_float ( connection_rate_pt, &connection_rate );

	while ( PL_get_list ( layers_pt, layer_pt, layers_pt ) ) {

		PL_get_integer ( layer_pt, l+nl++ );
		}

	return PL_unify_pointer ( ann_pt, fann_create_sparse_array( (float) connection_rate, nl, l ) );
}


// In module plfann.pl: create_shortcut


foreign_t swi_fann_create_shortcut_array ( term_t layers_pt, term_t ann_pt ) {

	unsigned int nl = 0;
	unsigned int l[MAXLAYERS];
	term_t layer_pt = PL_new_term_ref ();

	while ( PL_get_list (layers_pt, layer_pt, layers_pt) ) {

		PL_get_integer ( layer_pt, l+nl++ );
		}

	return PL_unify_pointer ( ann_pt, fann_create_shortcut_array( nl, l ) );
}


foreign_t swi_fann_destroy ( term_t ann_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

    fann_destroy ( ann );

	PL_succeed;
}


#ifdef VERSION220
foreign_t swi_fann_copy ( term_t ann1_pt, term_t ann2_pt ) {

	void *ann1;

	if ( !PL_get_pointer ( ann1_pt, &ann1 ) ) PL_fail;

	return PL_unify_pointer ( ann2_pt, fann_copy ( ann1 ) );
}
#endif


foreign_t swi_fann_run ( term_t ann_pt, term_t input_pt, term_t output_pt ) {

	unsigned int i, num_input;
	fann_type *input, *output;
	term_t temp_pt = PL_new_term_ref ();
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	num_input = fann_get_num_input( ann );
	input = PL_malloc ( num_input * sizeof ( fann_type ) );

	for ( i = 0; i < num_input; i++ ) {

		if ( !PL_unify_list ( input_pt, temp_pt, input_pt ) ||
			 !PL_FANN_GET_FANNTYPE(temp_pt,input+i) ) PL_fail;
	}
	PL_unify_nil ( input_pt );

	output = fann_run ( ann, ( fann_type* ) input );

	PL_free ( input );

	for ( i = 0; i < fann_get_num_output( ann ); i++ ) {

		if ( !PL_unify_list ( output_pt, temp_pt, output_pt ) ||
		     !PL_FANN_UNIFY_FANNTYPE(temp_pt,output[i]) ) PL_fail;
	}

	return PL_unify_nil ( output_pt );
}


foreign_t swi_fann_randomize_weights ( term_t ann_pt, term_t min_weight_pt, term_t max_weight_pt ) {

	fann_type min_weight, max_weight;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_FANN_GET_FANNTYPE(min_weight_pt,&min_weight) ||
		 !PL_FANN_GET_FANNTYPE(max_weight_pt,&max_weight) ) PL_fail;

    fann_randomize_weights ( ann, min_weight, max_weight );

	PL_succeed;
}


foreign_t swi_fann_init_weights ( term_t ann_pt, term_t train_data_pt ) {

	void *ann, *train_data;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_pointer ( train_data_pt, &train_data ) ) PL_fail;

	fann_init_weights ( ann, train_data );

	PL_succeed;
}


foreign_t swi_fann_print_connections ( term_t ann_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	fann_print_connections ( ann );

	PL_succeed;
}


foreign_t swi_fann_print_parameters ( term_t ann_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	fann_print_parameters (	ann	);

	PL_succeed;
}


foreign_t swi_fann_get_num_input ( term_t ann_pt, term_t put_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_integer ( put_pt, fann_get_num_input ( ann ) );
}


foreign_t swi_fann_get_num_output ( term_t ann_pt, term_t put_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_integer ( put_pt, fann_get_num_output ( ann ) );
}


foreign_t swi_fann_get_total_neurons ( term_t ann_pt, term_t tn_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_integer ( tn_pt, fann_get_total_neurons ( ann ) );
}


foreign_t swi_fann_get_total_connections ( term_t ann_pt, term_t tn_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_integer ( tn_pt, fann_get_total_connections ( ann ) );
}


foreign_t swi_fann_get_network_type ( term_t ann_pt, term_t type_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_atom_chars ( type_pt, FANN_NETTYPE_NAMES[ fann_get_network_type ( ann ) ] );
}


foreign_t swi_fann_get_connection_rate ( term_t ann_pt, term_t cr_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_float ( cr_pt, fann_get_connection_rate( ann ) );
}


foreign_t swi_fann_get_num_layers ( term_t ann_pt, term_t nl_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_integer ( nl_pt, fann_get_num_layers( ann ) );
}


foreign_t swi_fann_get_layer_array ( term_t ann_pt, term_t lay_arr_pt ) {

	unsigned int i, temp[MAXLAYERS];

	term_t temp_pt = PL_new_term_ref ();
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	fann_get_layer_array ( ann, temp );

	for ( i = 0; i < fann_get_num_layers( ann ); i++ ) {

		if ( !PL_unify_list ( lay_arr_pt, temp_pt, lay_arr_pt ) ||
			 !PL_unify_integer ( temp_pt, temp[i] ) ) PL_fail;
	}

	return PL_unify_nil ( lay_arr_pt );
}


foreign_t swi_fann_get_bias_array ( term_t ann_pt, term_t lay_arr_pt ) {

	unsigned int i, temp[MAXLAYERS];
	term_t temp_pt = PL_new_term_ref ();
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	fann_get_bias_array ( ann, temp );

	for ( i = 0; i < fann_get_num_layers( ann ) - 1; i++ ) {

		if ( !PL_unify_list ( lay_arr_pt, temp_pt, lay_arr_pt ) ||
			 !PL_unify_integer ( temp_pt, temp[i] ) ) PL_fail;
	}

	return PL_unify_nil ( lay_arr_pt );
}


foreign_t swi_fann_get_connection_array ( term_t ann_pt, term_t connections_pt ) {

    unsigned int total_connections, i;
	term_t connection_pt, temp_pt = PL_new_term_ref ();
	void *ann;
	struct fann_connection *connections;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	total_connections = fann_get_total_connections ( ann );

	connections = PL_malloc ( sizeof ( struct fann_connection ) * total_connections );

	fann_get_connection_array ( ann, connections );

	for ( i = 0; i < total_connections; i++ ) {

		PL_unify_list ( connections_pt, connection_pt, connections_pt );

			PL_unify_list ( connection_pt, temp_pt, connection_pt );
			PL_unify_integer ( temp_pt, (connections+i)->from_neuron );

			PL_unify_list ( connection_pt, temp_pt, connection_pt );
			PL_unify_integer ( temp_pt, (connections+i)->to_neuron );

			PL_unify_list ( connection_pt, temp_pt, connection_pt );
			PL_FANN_UNIFY_FANNTYPE( temp_pt, (connections+i)->weight );

			PL_unify_nil ( connection_pt );
	}

	PL_unify_nil ( connections_pt );

	PL_free ( connections );

	PL_succeed;
}


foreign_t swi_fann_set_weight_array ( term_t ann_pt, term_t connections_pt ) {

    unsigned int i;
	term_t connection_pt = PL_new_term_ref ();
	term_t temp_pt = PL_new_term_ref ();
	void *ann;
	struct fann_connection *connections;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	connections = PL_malloc ( sizeof ( struct fann_connection ) * fann_get_total_connections ( ann ) );

	for ( i = 0; PL_unify_list ( connections_pt, connection_pt, connections_pt ); i++ ) {

			PL_unify_list ( connection_pt, temp_pt, connection_pt );
			PL_get_integer ( temp_pt, &connections[i].from_neuron );

			PL_unify_list ( connection_pt, temp_pt, connection_pt );
			PL_get_integer ( temp_pt, &connections[i].to_neuron );

			PL_unify_list ( connection_pt, temp_pt, connection_pt );
			PL_FANN_GET_FANNTYPE(temp_pt,&connections[i].weight);

			PL_unify_nil ( connection_pt );
	}

	PL_unify_nil ( connections_pt );

	fann_set_weight_array ( ann, connections, i );

	PL_free ( connections );

	PL_succeed;
}


foreign_t swi_fann_set_weight ( term_t ann_pt, term_t from_neuron_pt, term_t to_neuron_pt, term_t weight_pt ) {

	void *ann;
    unsigned int from_neuron, to_neuron;
	fann_type weight;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
	     !PL_get_integer ( from_neuron_pt, &from_neuron ) ||
	     !PL_get_integer ( to_neuron_pt, &to_neuron ) ||
		 !PL_FANN_GET_FANNTYPE(weight_pt,&weight) ) PL_fail;

	fann_set_weight ( ann, from_neuron, to_neuron, weight );

	PL_succeed;
}


foreign_t swi_fann_set_user_data ( term_t ann_pt, term_t user_data_pt ) {

	void *ann, *user_data;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_pointer ( user_data_pt, &user_data ) ) PL_fail;

    fann_set_user_data ( ann, user_data );

	PL_succeed;
}


foreign_t swi_fann_get_user_data ( term_t ann_pt, term_t user_data_pt ) {

	void *ann, *user_data;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

    return PL_unify_pointer ( user_data_pt, fann_get_user_data ( ann ) );
}


#ifdef VERSION220
foreign_t swi_fann_get_decimal_point ( term_t ann_pt, term_t decp_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

    return PL_unify_integer ( decp_pt, fann_get_decimal_point ( ann ) );
}


foreign_t swi_fann_get_multiplier ( term_t ann_pt, term_t mul_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

    return PL_unify_integer ( mul_pt, fann_get_multiplier ( ann ) );
}
#endif


foreign_t swi_fann_train ( term_t ann_pt, term_t input_pt, term_t output_pt ) {

	unsigned int i, num_input, num_output;
	fann_type *input, *output;
	term_t temp_pt = PL_new_term_ref ();
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	num_input = fann_get_num_input( ann );
	input = PL_malloc ( num_input * sizeof ( fann_type ) );

	num_output = fann_get_num_output( ann );
	output = PL_malloc ( num_output * sizeof ( fann_type ) );

	for ( i = 0; i < num_input; i++ ) {

		if ( !PL_unify_list ( input_pt, temp_pt, input_pt ) ||
			 !PL_FANN_GET_FANNTYPE(temp_pt,input+i) ) PL_fail;
	}
	PL_unify_nil ( input_pt );

	for ( i = 0; i < num_output; i++ ) {

		if ( !PL_unify_list ( output_pt, temp_pt, output_pt ) ||
			 !PL_FANN_GET_FANNTYPE(temp_pt,output+i) ) PL_fail;
	}
	PL_unify_nil ( output_pt );

	fann_train ( ann, input, output );

	PL_free ( input );
	PL_free ( output );

	PL_succeed;
}


foreign_t swi_fann_test ( term_t ann_pt, term_t input_pt, term_t output_pt ) {

	unsigned int i, num_input, num_output;
	fann_type *input, *output;
	term_t temp_pt = PL_new_term_ref ();
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	num_input = fann_get_num_input( ann );
	input = PL_malloc ( num_input * sizeof ( fann_type ) );

	num_output = fann_get_num_output( ann );
	output = PL_malloc ( num_output * sizeof ( fann_type ) );

	for ( i = 0; i < num_input; i++ ) {

		if ( !PL_unify_list ( input_pt, temp_pt, input_pt ) ||
			 !PL_FANN_GET_FANNTYPE(temp_pt,input+i) ) PL_fail;
	}
	PL_unify_nil ( input_pt );

	for ( i = 0; i < num_output; i++ ) {

		if ( !PL_unify_list ( output_pt, temp_pt, output_pt ) ||
			 !PL_FANN_GET_FANNTYPE(temp_pt,output+i) ) PL_fail;
	}
	PL_unify_nil ( output_pt );

	fann_test ( ann, input, output );

	PL_free ( input );
	PL_free ( output );

	PL_succeed;
}


foreign_t swi_fann_get_MSE ( term_t ann_pt, term_t MSE_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_float ( MSE_pt, fann_get_MSE ( ann ) );
}


foreign_t swi_fann_get_bit_fail ( term_t ann_pt, term_t bf_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_integer ( bf_pt, fann_get_bit_fail ( ann ) );
}


foreign_t swi_fann_reset_MSE ( term_t ann_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	fann_reset_MSE ( ann );

	PL_succeed;
}


foreign_t swi_fann_train_on_data ( term_t ann_pt, term_t data_pt, term_t max_epochs_pt, term_t epochs_between_reports_pt, term_t desired_error_pt ) {

	void *ann, *data;
	unsigned int max_epochs, epochs_between_reports;
	double desired_error;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_pointer ( data_pt, &data ) ||
		 !PL_get_integer ( max_epochs_pt, &max_epochs ) ||
         !PL_get_integer ( epochs_between_reports_pt, &epochs_between_reports ) ||
		 !PL_get_float ( desired_error_pt, &desired_error ) ) PL_fail;

	fann_train_on_data(ann, data, max_epochs, epochs_between_reports, (float) desired_error );

	PL_succeed;
}


foreign_t swi_fann_train_on_file ( term_t ann_pt, term_t file_pt, term_t max_epochs_pt, term_t epochs_between_reports_pt, term_t desired_error_pt ) {

	void *ann;
	char *file;
	unsigned int max_epochs, epochs_between_reports;
	double desired_error;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_file_name ( file_pt, &file, PL_FILE_ABSOLUTE || PL_FILE_SEARCH || PL_FILE_EXIST ) ||
		 !PL_get_integer ( max_epochs_pt, &max_epochs ) ||
         !PL_get_integer ( epochs_between_reports_pt, &epochs_between_reports ) ||
		 !PL_get_float ( desired_error_pt, &desired_error ) ) PL_fail;

    fann_train_on_file ( ann, file, max_epochs, epochs_between_reports, (float) desired_error );

	PL_succeed;
}


foreign_t swi_fann_train_epoch ( term_t ann_pt, term_t data_pt ) {

	void *ann, *data;
	double MSE;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_pointer ( data_pt, &data ) ) PL_fail;

    fann_train_epoch ( ann, data );

	PL_succeed;
}


foreign_t swi_fann_test_data  ( term_t ann_pt, term_t data_pt, term_t MSE_pt ) {

	void *ann, *data;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_pointer ( data_pt, &data ) ) PL_fail;

	return PL_unify_float ( MSE_pt, fann_test_data ( ann, data ) );
}


foreign_t swi_fann_read_train_from_file ( term_t file_pt, term_t data_pt ) {

	char *file;

	if ( !PL_get_file_name ( file_pt, &file, PL_FILE_ABSOLUTE ) ) PL_fail;

	return PL_unify_pointer ( data_pt, fann_read_train_from_file ( file ) );
}


#ifdef VERSION220
foreign_t swi_fann_create_train ( term_t ann_pt, term_t num_data_pt, term_t num_input_pt, term_t num_output_pt ) {

	unsigned int num_data, num_input, num_output;

	if ( !PL_get_integer ( num_data_pt, &num_data ) ||
		 !PL_get_integer ( num_input_pt, &num_input ) ||
         !PL_get_integer ( num_output_pt, &num_output ) ) PL_fail;

    return PL_unify_pointer ( ann_pt, fann_create_train ( num_data, num_input, num_output ) );
}
#endif


/* Not Finished: doesn't seem to make much sense from Prolog, advise me if you see any use for it

   swi_fann_create_train_from_callback
*/


foreign_t swi_fann_destroy_train ( term_t td_pt ) {

	void *td;

	if ( !PL_get_pointer ( td_pt, &td ) ) PL_fail;

    fann_destroy_train ( td );

	PL_succeed;
}


foreign_t swi_fann_shuffle_train_data ( term_t td_pt ) {

	void *td;

	if ( !PL_get_pointer ( td_pt, &td ) ) PL_fail;

    fann_shuffle_train_data ( td );

	PL_succeed;
}


foreign_t swi_fann_scale_train ( term_t ann_pt, term_t data_pt ) {

	void *ann, *data;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_pointer ( data_pt, &data ) ) PL_fail;

	fann_scale_train ( ann, data );

	PL_succeed;
}


foreign_t swi_fann_descale_train ( term_t ann_pt, term_t data_pt ) {

	void *ann, *data;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_pointer ( data_pt, &data ) ) PL_fail;

	fann_descale_train ( ann, data );

	PL_succeed;
}


foreign_t swi_fann_set_input_scaling_params ( term_t ann_pt, term_t data_pt, term_t new_input_min_pt, term_t new_input_max_pt ) {

	void *ann, *data;
	double new_input_min, new_input_max;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_pointer ( data_pt, &data ) ||
		 !PL_get_float ( new_input_min_pt, &new_input_min ) ||
		 !PL_get_float ( new_input_max_pt, &new_input_max ) ) PL_fail;

	if ( !fann_set_input_scaling_params ( ann, data, new_input_min, new_input_max ) ) PL_succeed; else PL_fail;
}


foreign_t swi_fann_set_output_scaling_params ( term_t ann_pt, term_t data_pt, term_t new_output_min_pt, term_t new_output_max_pt ) {

	void *ann, *data;
	double new_output_min, new_output_max;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_pointer ( data_pt, &data ) ||
		 !PL_get_float ( new_output_min_pt, &new_output_min ) ||
		 !PL_get_float ( new_output_max_pt, &new_output_max ) ) PL_fail;

	if ( !fann_set_output_scaling_params ( ann, data, new_output_min, new_output_max ) ) PL_succeed; else PL_fail;
}


foreign_t swi_fann_set_scaling_params ( term_t ann_pt, term_t data_pt, term_t new_input_min_pt, term_t new_input_max_pt, term_t new_output_min_pt, term_t new_output_max_pt ) {

	void *ann, *data;
	double new_input_min, new_input_max, new_output_min, new_output_max;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_pointer ( data_pt, &data ) ||
		 !PL_get_float ( new_input_min_pt, &new_input_min ) ||
		 !PL_get_float ( new_input_max_pt, &new_input_max ) ||
		 !PL_get_float ( new_output_min_pt, &new_output_min ) ||
		 !PL_get_float ( new_output_max_pt, &new_output_max )  ) PL_fail;

	if ( !fann_set_scaling_params ( ann, data, new_input_min, new_input_max, new_output_min, new_output_max ) ) PL_succeed; else PL_fail;
}


foreign_t swi_fann_clear_scaling_params ( term_t ann_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	fann_clear_scaling_params ( ann );

	PL_succeed;
}


foreign_t swi_fann_scale_input ( term_t ann_pt, term_t input_pt ) {

	unsigned int i, num_input;
	fann_type *input;
	term_t temp_pt = PL_new_term_ref ();
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	num_input = fann_get_num_input( ann );
	input = PL_malloc ( num_input * sizeof ( fann_type ) );

	for ( i = 0; i < num_input; i++ ) {

		if ( !PL_unify_list ( input_pt, temp_pt, input_pt ) ||
			 !PL_FANN_GET_FANNTYPE(temp_pt,input+i) ) PL_fail;
	}
	PL_unify_nil ( input_pt );

	fann_scale_input ( ann, input );

	PL_free ( input );

	PL_succeed;
}


foreign_t swi_fann_scale_output ( term_t ann_pt, term_t output_pt ) {

	unsigned int i, num_output;
	fann_type *output;
	term_t temp_pt = PL_new_term_ref ();
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	num_output = fann_get_num_output( ann );
	output = PL_malloc ( num_output * sizeof ( fann_type ) );

	for ( i = 0; i < num_output; i++ ) {

		if ( !PL_unify_list ( output_pt, temp_pt, output_pt ) ||
			 !PL_FANN_GET_FANNTYPE(temp_pt,output+i) ) PL_fail;
	}
	PL_unify_nil ( output_pt );

	fann_scale_output ( ann, output );

	PL_free ( output );

	PL_succeed;
}


foreign_t swi_fann_descale_input ( term_t ann_pt, term_t input_pt ) {

	unsigned int i, num_input;
	fann_type *input;
	term_t temp_pt = PL_new_term_ref ();
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	num_input = fann_get_num_input( ann );
	input = PL_malloc ( num_input * sizeof ( fann_type ) );

	for ( i = 0; i < num_input; i++ ) {

		if ( !PL_unify_list ( input_pt, temp_pt, input_pt ) ||
			 !PL_FANN_GET_FANNTYPE(temp_pt,input+i) ) PL_fail;
	}
	PL_unify_nil ( input_pt );

	fann_descale_input ( ann, input );

	PL_free ( input );

	PL_succeed;
}


foreign_t swi_fann_descale_output ( term_t ann_pt, term_t output_pt ) {

	unsigned int i, num_output;
	fann_type *output;
	term_t temp_pt = PL_new_term_ref ();
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	num_output = fann_get_num_output( ann );
	output = PL_malloc ( num_output * sizeof ( fann_type ) );

	for ( i = 0; i < num_output; i++ ) {

		if ( !PL_unify_list ( output_pt, temp_pt, output_pt ) ||
			 !PL_FANN_GET_FANNTYPE(temp_pt,output+i) ) PL_fail;
	}
	PL_unify_nil ( output_pt );

	fann_descale_output ( ann, output );

	PL_free ( output );

	PL_succeed;
}


foreign_t swi_fann_scale_input_train_data ( term_t data_pt, term_t new_min_pt, term_t new_max_pt ) {

	void *data;
	fann_type new_min, new_max;

	if ( !PL_get_pointer ( data_pt, &data ) ||
		 !PL_FANN_GET_FANNTYPE(new_min_pt,&new_min) ||
		 !PL_FANN_GET_FANNTYPE(new_max_pt,&new_max) ) PL_fail;

	fann_scale_input_train_data ( data, new_min, new_max );

	PL_succeed;
}


foreign_t swi_fann_scale_output_train_data ( term_t data_pt, term_t new_min_pt, term_t new_max_pt ) {

	void *data;
	fann_type new_min, new_max;

	if ( !PL_get_pointer ( data_pt, &data ) ||
		 !PL_FANN_GET_FANNTYPE(new_min_pt,&new_min) ||
		 !PL_FANN_GET_FANNTYPE(new_max_pt,&new_max) ) PL_fail;

	fann_scale_output_train_data ( data, new_min, new_max );

	PL_succeed;
}


foreign_t swi_fann_scale_train_data ( term_t data_pt, term_t new_min_pt, term_t new_max_pt ) {

	void *data;
	fann_type new_min, new_max;

	if ( !PL_get_pointer ( data_pt, &data ) ||
		 !PL_FANN_GET_FANNTYPE(new_min_pt,&new_min) ||
		 !PL_FANN_GET_FANNTYPE(new_max_pt,&new_max) ) PL_fail;

	fann_scale_train_data ( data, new_min, new_max );

	PL_succeed;
}


foreign_t swi_fann_merge_train_data ( term_t data1_pt, term_t data2_pt, term_t data3_pt ) {

	void *data1, *data2;

	if ( !PL_get_pointer ( data1_pt, &data1 ) ||
		 !PL_get_pointer ( data2_pt, &data2 ) ) PL_fail;

	return PL_unify_pointer ( data3_pt, fann_merge_train_data ( data1, data2 ) );
}


foreign_t swi_fann_duplicate_train_data ( term_t data1_pt, term_t data2_pt ) {

	void *data1;

	if ( !PL_get_pointer ( data1_pt, &data1 ) ) PL_fail;

	return PL_unify_pointer ( data2_pt, fann_duplicate_train_data ( data1 ) );
}


foreign_t swi_fann_subset_train_data ( term_t data1_pt, term_t pos_pt, term_t len_pt, term_t data2_pt ) {

	void *data1;
	unsigned int pos, len;

	if ( !PL_get_pointer ( data1_pt, &data1 ) ||
		 !PL_get_integer ( pos_pt, &pos ) ||
		 !PL_get_integer ( len_pt, &len ) ) PL_fail;

	return PL_unify_pointer ( data2_pt, fann_subset_train_data ( data1, pos, len ) );
}


foreign_t swi_fann_length_train_data ( term_t data_pt, term_t len_pt ) {

	void *data;

	if ( !PL_get_pointer ( data_pt, &data ) ) PL_fail;

	return PL_unify_integer ( len_pt, fann_length_train_data ( data ) );
}


foreign_t swi_fann_num_input_train_data ( term_t data_pt, term_t len_pt ) {

	void *data;

	if ( !PL_get_pointer ( data_pt, &data ) ) PL_fail;

	return PL_unify_integer ( len_pt, fann_num_input_train_data ( data ) );
}


foreign_t swi_fann_num_output_train_data ( term_t data_pt, term_t len_pt ) {

	void *data;

	if ( !PL_get_pointer ( data_pt, &data ) ) PL_fail;

	return PL_unify_integer ( len_pt, fann_num_output_train_data ( data ) );
}


foreign_t swi_fann_save_train ( term_t data_pt, term_t file_pt ) {

	void *data;
	char *file;

	if ( !PL_get_pointer ( data_pt, &data ) ||
		 !PL_get_file_name ( file_pt, &file, PL_FILE_ABSOLUTE ) ) PL_fail;

	if ( !fann_save_train ( data, file ) ) PL_succeed; else PL_fail;
}


foreign_t swi_fann_save_train_to_fixed ( term_t data_pt, term_t file_pt, term_t dec_pt ) {

	void *data;
	char *file;
	unsigned int dec;

	if ( !PL_get_pointer ( data_pt, &data ) ||
		 !PL_get_file_name ( file_pt, &file, PL_FILE_ABSOLUTE ) ||
		 !PL_get_integer ( dec_pt, &dec ) ) PL_fail;

	if ( !fann_save_train_to_fixed ( data, file, dec ) ) PL_succeed; else PL_fail;
}


foreign_t swi_fann_get_training_algorithm ( term_t ann_pt, term_t type_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

    return PL_unify_atom_chars ( type_pt, FANN_TRAIN_NAMES[ fann_get_training_algorithm ( ann ) ] );
}


foreign_t swi_fann_set_training_algorithm ( term_t ann_pt, term_t type_pt ) {

	char *type;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_chars ( type_pt, &type, CVT_ATOM ) ) PL_fail;

	if ( strcmp ( "FANN_TRAIN_INCREMENTAL", type ) == 0 ) { fann_set_training_algorithm ( ann, FANN_TRAIN_INCREMENTAL ); PL_succeed; }
	if ( strcmp ( "FANN_TRAIN_BATCH", type ) == 0 ) { fann_set_training_algorithm ( ann, FANN_TRAIN_BATCH ); PL_succeed; }
	if ( strcmp ( "FANN_TRAIN_RPROP", type ) == 0 ) { fann_set_training_algorithm ( ann, FANN_TRAIN_RPROP ); PL_succeed; }
	if ( strcmp ( "FANN_TRAIN_QUICKPROP", type ) == 0 ) { fann_set_training_algorithm ( ann, FANN_TRAIN_QUICKPROP ); PL_succeed; }

	PL_fail;
}


foreign_t swi_fann_get_learning_rate ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_float ( out_pt, fann_get_learning_rate ( ann ) );
}


foreign_t swi_fann_set_learning_rate ( term_t ann_pt, term_t in_pt ) {

	double in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_float ( in_pt, &in ) ) PL_fail;

	fann_set_learning_rate ( ann, (float) in );

	PL_succeed;
}


foreign_t swi_fann_get_learning_momentum ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_float ( out_pt, fann_get_learning_momentum ( ann ) );
}


foreign_t swi_fann_set_learning_momentum ( term_t ann_pt, term_t in_pt ) {

	double in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_float ( in_pt, &in ) ) PL_fail;

	fann_set_learning_momentum ( ann, (float) in );

	PL_succeed;
}


foreign_t swi_fann_get_activation_function ( term_t ann_pt, term_t layer_pt, term_t neuron_pt, term_t type_pt ) {

	unsigned int layer, neuron;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_integer ( layer_pt, &layer ) ||
		 !PL_get_integer ( neuron_pt, &neuron ) ) PL_fail;

	return PL_unify_atom_chars ( type_pt, FANN_ACTIVATIONFUNC_NAMES[ fann_get_activation_function ( ann, layer, neuron ) ] );
}


foreign_t swi_fann_set_activation_function ( term_t ann_pt, term_t type_pt, term_t layer_pt, term_t neuron_pt ) {

	unsigned int layer, neuron;
	char *type;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_chars ( type_pt, &type, CVT_ATOM ) ||
		 !PL_get_integer ( layer_pt, &layer ) ||
		 !PL_get_integer ( neuron_pt, &neuron ) ) PL_fail;

	fann_set_activation_function ( ann, lookup_activationfunc_enum ( type ), layer, neuron );

	PL_succeed;
}


foreign_t swi_fann_set_activation_function_layer ( term_t ann_pt, term_t type_pt, term_t layer_pt ) {

	unsigned int layer;
	char *type;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_chars ( type_pt, &type, CVT_ATOM ) ||
		 !PL_get_integer ( layer_pt, &layer ) ) PL_fail;

	fann_set_activation_function_layer ( ann, lookup_activationfunc_enum ( type ), layer );

	PL_succeed;
}


foreign_t swi_fann_set_activation_function_hidden ( term_t ann_pt, term_t type_pt ) {

	char *type;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_chars ( type_pt, &type, CVT_ATOM ) ) PL_fail;

    fann_set_activation_function_hidden ( ann, lookup_activationfunc_enum ( type ) );

	PL_succeed;
}


foreign_t swi_fann_set_activation_function_output ( term_t ann_pt, term_t type_pt ) {

	char *type;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_chars ( type_pt, &type, CVT_ATOM ) ) PL_fail;

	fann_set_activation_function_output ( ann, lookup_activationfunc_enum ( type ) );

	PL_succeed;
}


foreign_t swi_fann_get_activation_steepness ( term_t ann_pt, term_t layer_pt, term_t neuron_pt, term_t steepness_pt ) {

	unsigned int layer, neuron;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_integer ( layer_pt, &layer ) ||
		 !PL_get_integer ( neuron_pt, &neuron ) ) PL_fail;

	return PL_unify_float ( steepness_pt, fann_get_activation_steepness ( ann, layer, neuron ) );
}


foreign_t swi_fann_set_activation_steepness ( term_t ann_pt, term_t steepness_pt, term_t layer_pt, term_t neuron_pt ) {

	unsigned int layer, neuron;
	fann_type steepness;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_FANN_GET_FANNTYPE(steepness_pt,&steepness) ||
		 !PL_get_integer ( layer_pt, &layer ) ||
		 !PL_get_integer ( neuron_pt, &neuron ) ) PL_fail;

	fann_set_activation_steepness ( ann, steepness, layer, neuron );

	PL_succeed;
}


foreign_t swi_fann_set_activation_steepness_layer( term_t ann_pt, term_t steepness_pt, term_t layer_pt ) {

	unsigned int layer;
	void *ann;
	fann_type steepness;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_FANN_GET_FANNTYPE(steepness_pt,&steepness) ||
		 !PL_get_integer ( layer_pt, &layer ) ) PL_fail;

	fann_set_activation_steepness_layer ( ann, steepness, layer );

	PL_succeed;
}


foreign_t swi_fann_set_activation_steepness_hidden ( term_t ann_pt, term_t steepness_pt ) {

	void *ann;
	fann_type steepness;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_FANN_GET_FANNTYPE(steepness_pt,&steepness) ) PL_fail;


	fann_set_activation_steepness_hidden ( ann, steepness );

	PL_succeed;
}


foreign_t swi_fann_set_activation_steepness_output ( term_t ann_pt, term_t steepness_pt ) {

	void *ann;
	fann_type steepness;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_FANN_GET_FANNTYPE(steepness_pt,&steepness) ) PL_fail;


	fann_set_activation_steepness_output ( ann, steepness );

	PL_succeed;
}


foreign_t swi_fann_get_train_error_function ( term_t ann_pt, term_t type_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_atom_chars ( type_pt, FANN_ERRORFUNC_NAMES[ fann_get_train_error_function ( ann ) ] );
}


foreign_t swi_fann_set_train_error_function ( term_t ann_pt, term_t type_pt ) {

	char *type;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_chars ( type_pt, &type, CVT_ATOM ) ) PL_fail;

	if ( strcmp ( "FANN_ERRORFUNC_LINEAR", type ) == 0 ) { fann_set_train_error_function ( ann, FANN_ERRORFUNC_LINEAR ); PL_succeed; }
	if ( strcmp ( "FANN_ERRORFUNC_TANH", type ) == 0 ) { fann_set_train_error_function ( ann, FANN_ERRORFUNC_TANH ); PL_succeed; }

	PL_fail;
}


foreign_t swi_fann_get_train_stop_function ( term_t ann_pt, term_t type_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_atom_chars ( type_pt, FANN_STOPFUNC_NAMES[ fann_get_train_stop_function ( ann ) ] );
}


foreign_t swi_fann_set_train_stop_function ( term_t ann_pt, term_t type_pt ) {

	char *type;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_chars ( type_pt, &type, CVT_ATOM ) ) PL_fail;

	if ( strcmp ( "FANN_STOPFUNC_MSE", type ) == 0 ) { fann_set_train_stop_function ( ann, FANN_STOPFUNC_MSE ); PL_succeed; }
	if ( strcmp ( "FANN_STOPFUNC_BIT", type ) == 0 ) { fann_set_train_stop_function ( ann, FANN_STOPFUNC_BIT ); PL_succeed; }

	PL_fail;
}


foreign_t swi_fann_get_bit_fail_limit ( term_t ann_pt, term_t bit_fail_limit_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_FANN_UNIFY_FANNTYPE(bit_fail_limit_pt,fann_get_bit_fail_limit(ann));
}


foreign_t swi_fann_set_bit_fail_limit ( term_t ann_pt, term_t bit_fail_limit_pt ) {

	void *ann;
	fann_type bit_fail_limit;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_FANN_GET_FANNTYPE(bit_fail_limit_pt,&bit_fail_limit) ) PL_fail;

	fann_set_bit_fail_limit ( ann, bit_fail_limit );

	PL_succeed;
}


/* Not Finished: doesn't seem to make much sense from Prolog, advise me if you see any use for it

   swi_fann_set_callback
*/


foreign_t swi_fann_get_quickprop_decay ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_float ( out_pt, fann_get_quickprop_decay ( ann ) );
}


foreign_t swi_fann_set_quickprop_decay ( term_t ann_pt, term_t in_pt ) {

	double in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_float ( in_pt, &in ) ) PL_fail;

	fann_set_quickprop_decay ( ann, (float) in );

	PL_succeed;
}


foreign_t swi_fann_get_quickprop_mu ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_float ( out_pt, fann_get_quickprop_mu ( ann ) );
}


foreign_t swi_fann_set_quickprop_mu ( term_t ann_pt, term_t in_pt ) {

	double in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_float ( in_pt, &in ) ) PL_fail;

	fann_set_quickprop_mu ( ann, (float) in );

	PL_succeed;
}


foreign_t swi_fann_get_rprop_increase_factor ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_float ( out_pt, fann_get_rprop_increase_factor ( ann ) );
}


foreign_t swi_fann_set_rprop_increase_factor ( term_t ann_pt, term_t in_pt ) {

	double in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_float ( in_pt, &in ) ) PL_fail;

	fann_set_rprop_increase_factor ( ann, (float) in );

	PL_succeed;
}


foreign_t swi_fann_get_rprop_decrease_factor ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_float ( out_pt, fann_get_rprop_decrease_factor ( ann ) );
}


foreign_t swi_fann_set_rprop_decrease_factor ( term_t ann_pt, term_t in_pt ) {

	double in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_float ( in_pt, &in ) ) PL_fail;

	fann_set_rprop_decrease_factor ( ann, (float) in );

	PL_succeed;
}


foreign_t swi_fann_get_rprop_delta_min ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_float ( out_pt, fann_get_rprop_delta_min ( ann ) );
}


foreign_t swi_fann_set_rprop_delta_min ( term_t ann_pt, term_t in_pt ) {

	double in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_float ( in_pt, &in ) ) PL_fail;

	fann_set_rprop_delta_min ( ann, (float) in );

	PL_succeed;
}


foreign_t swi_fann_get_rprop_delta_max ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_float ( out_pt, fann_get_rprop_delta_max ( ann ) );
}


foreign_t swi_fann_set_rprop_delta_max ( term_t ann_pt, term_t in_pt ) {

	double in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_float ( in_pt, &in ) ) PL_fail;

	fann_set_rprop_delta_max ( ann, (float) in );

	PL_succeed;
}


foreign_t swi_fann_get_rprop_delta_zero ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_float ( out_pt, fann_get_rprop_delta_zero ( ann ) );
}


foreign_t swi_fann_set_rprop_delta_zero ( term_t ann_pt, term_t in_pt ) {

	double in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_float ( in_pt, &in ) ) PL_fail;

	fann_set_rprop_delta_zero ( ann, (float) in );

	PL_succeed;
}

#ifdef VERSION220
foreign_t swi_fann_get_sarprop_weight_decay_shift ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_float ( out_pt, fann_get_sarprop_weight_decay_shift ( ann ) );
}


foreign_t swi_fann_set_sarprop_weight_decay_shift ( term_t ann_pt, term_t in_pt ) {

	double in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_float ( in_pt, &in ) ) PL_fail;

	fann_set_sarprop_weight_decay_shift ( ann, (float) in );

	PL_succeed;
}


foreign_t swi_fann_get_sarprop_step_error_threshold_factor ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_float ( out_pt, fann_get_sarprop_step_error_threshold_factor ( ann ) );
}


foreign_t swi_fann_set_sarprop_step_error_threshold_factor ( term_t ann_pt, term_t in_pt ) {

	double in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_float ( in_pt, &in ) ) PL_fail;

	fann_set_sarprop_step_error_threshold_factor ( ann, (float) in );

	PL_succeed;
}


foreign_t swi_fann_get_sarprop_step_error_shift ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_float ( out_pt, fann_get_sarprop_step_error_shift ( ann ) );
}


foreign_t swi_fann_set_sarprop_step_error_shift ( term_t ann_pt, term_t in_pt ) {

	double in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_float ( in_pt, &in ) ) PL_fail;

	fann_set_sarprop_step_error_shift ( ann, (float) in );

	PL_succeed;
}


foreign_t swi_fann_get_sarprop_temperature ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_float ( out_pt, fann_get_sarprop_temperature ( ann ) );
}


foreign_t swi_fann_set_sarprop_temperature ( term_t ann_pt, term_t in_pt ) {

	double in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_float ( in_pt, &in ) ) PL_fail;

	fann_set_sarprop_temperature ( ann, (float) in );

	PL_succeed;
}
#endif


foreign_t swi_fann_cascadetrain_on_data ( term_t ann_pt, term_t data_pt, term_t max_neurons_pt, term_t neurons_between_reports_pt, term_t desired_error_pt ) {

	void *ann, *data;
	unsigned int max_neurons, neurons_between_reports;
	double desired_error;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_pointer ( data_pt, &data ) ||
		 !PL_get_integer ( max_neurons_pt, &max_neurons ) ||
         !PL_get_integer ( neurons_between_reports_pt, &neurons_between_reports ) ||
		 !PL_get_float ( desired_error_pt, &desired_error ) ) PL_fail;

	fann_cascadetrain_on_data ( ann, data, max_neurons, neurons_between_reports, (float) desired_error );

	PL_succeed;
}


foreign_t swi_fann_cascadetrain_on_file ( term_t ann_pt, term_t file_pt, term_t max_neurons_pt, term_t neurons_between_reports_pt, term_t desired_error_pt ) {

	void *ann;
	char *file;
	unsigned int max_neurons, neurons_between_reports;
	double desired_error;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_file_name ( file_pt, &file, PL_FILE_ABSOLUTE ) ||
		 !PL_get_integer ( max_neurons_pt, &max_neurons ) ||
         !PL_get_integer ( neurons_between_reports_pt, &neurons_between_reports ) ||
		 !PL_get_float ( desired_error_pt, &desired_error ) ) PL_fail;

   fann_cascadetrain_on_file ( ann, file, max_neurons, neurons_between_reports, (float) desired_error );

   PL_succeed;
}


foreign_t swi_fann_get_cascade_output_change_fraction ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_float ( out_pt, fann_get_cascade_output_change_fraction ( ann ) );
}


foreign_t swi_fann_set_cascade_output_change_fraction ( term_t ann_pt, term_t in_pt ) {

	double in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_float ( in_pt, &in ) ) PL_fail;

	fann_set_cascade_output_change_fraction ( ann, (float) in );

	PL_succeed;
}


foreign_t swi_fann_get_cascade_output_stagnation_epochs ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_integer ( out_pt, fann_get_cascade_output_stagnation_epochs ( ann ) );
}


foreign_t swi_fann_set_cascade_output_stagnation_epochs ( term_t ann_pt, term_t in_pt ) {

	unsigned int in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_integer ( in_pt, &in ) ) PL_fail;

	fann_set_cascade_output_stagnation_epochs ( ann, in );

	PL_succeed;
}


foreign_t swi_fann_get_cascade_candidate_change_fraction ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_float ( out_pt, fann_get_cascade_candidate_change_fraction ( ann ) );
}


foreign_t swi_fann_set_cascade_candidate_change_fraction ( term_t ann_pt, term_t in_pt ) {

	double in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_float ( in_pt, &in ) ) PL_fail;

	fann_set_cascade_candidate_change_fraction ( ann, (float) in );

	PL_succeed;
}


foreign_t swi_fann_get_cascade_candidate_stagnation_epochs ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_integer ( out_pt, fann_get_cascade_candidate_stagnation_epochs ( ann ) );
}


foreign_t swi_fann_set_cascade_candidate_stagnation_epochs ( term_t ann_pt, term_t in_pt ) {

	unsigned int in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_integer ( in_pt, &in ) ) PL_fail;

	fann_set_cascade_candidate_stagnation_epochs ( ann, in );

	PL_succeed;
}


foreign_t swi_fann_get_cascade_weight_multiplier ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_FANN_UNIFY_FANNTYPE(out_pt,fann_get_cascade_weight_multiplier(ann));
}


foreign_t swi_fann_set_cascade_weight_multiplier ( term_t ann_pt, term_t in_pt ) {

	fann_type in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_FANN_GET_FANNTYPE(in_pt,&in) ) PL_fail;

	fann_set_cascade_weight_multiplier ( ann, in );

	PL_succeed;
}


foreign_t swi_fann_get_cascade_candidate_limit ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_FANN_UNIFY_FANNTYPE(out_pt,fann_get_cascade_candidate_limit(ann) );
}


foreign_t swi_fann_set_cascade_candidate_limit ( term_t ann_pt, term_t in_pt ) {

	fann_type in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_FANN_GET_FANNTYPE(in_pt,&in) ) PL_fail;

	fann_set_cascade_candidate_limit ( ann, in );

	PL_succeed;
}


foreign_t swi_fann_get_cascade_max_out_epochs ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_integer ( out_pt, fann_get_cascade_max_out_epochs ( ann ) );
}


foreign_t swi_fann_set_cascade_max_out_epochs ( term_t ann_pt, term_t in_pt ) {

	unsigned int in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_integer ( in_pt, &in ) ) PL_fail;

	fann_set_cascade_max_out_epochs ( ann, in );

	PL_succeed;
}

#ifdef VERSION220
foreign_t swi_fann_get_cascade_min_out_epochs ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_integer ( out_pt, fann_get_cascade_min_out_epochs ( ann ) );
}


foreign_t swi_fann_set_cascade_min_out_epochs ( term_t ann_pt, term_t in_pt ) {

	unsigned int in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_integer ( in_pt, &in ) ) PL_fail;

	fann_set_cascade_min_out_epochs ( ann, in );

	PL_succeed;
}


foreign_t swi_fann_get_cascade_max_cand_epochs ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_integer ( out_pt, fann_get_cascade_max_cand_epochs ( ann ) );
}


foreign_t swi_fann_set_cascade_max_cand_epochs ( term_t ann_pt, term_t in_pt ) {

	unsigned int in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_integer ( in_pt, &in ) ) PL_fail;

	fann_set_cascade_max_cand_epochs ( ann, in );

	PL_succeed;
}


foreign_t swi_fann_get_cascade_min_cand_epochs ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_integer ( out_pt, fann_get_cascade_min_cand_epochs ( ann ) );
}


foreign_t swi_fann_set_cascade_min_cand_epochs ( term_t ann_pt, term_t in_pt ) {

	unsigned int in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_integer ( in_pt, &in ) ) PL_fail;

	fann_set_cascade_min_cand_epochs ( ann, in );

	PL_succeed;
}
#endif


foreign_t swi_fann_get_cascade_num_candidates ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_integer ( out_pt, fann_get_cascade_num_candidates ( ann ) );
}


foreign_t swi_fann_get_cascade_activation_functions_count ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_integer ( out_pt, fann_get_cascade_activation_functions_count ( ann ) );
}


foreign_t swi_fann_get_cascade_activation_functions ( term_t ann_pt, term_t type_pt ) {

	unsigned int i;
	void *ann;
	enum fann_activationfunc_enum *functions;
	term_t temp_pt = PL_new_term_ref ();

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	functions = fann_get_cascade_activation_functions ( ann );

	for ( i = 0; i < fann_get_cascade_activation_functions_count ( ann ); i++ ) {

		PL_unify_list ( type_pt, temp_pt, type_pt );
		PL_unify_atom_chars ( temp_pt, FANN_ACTIVATIONFUNC_NAMES[ functions[i] ] );
	}
	return PL_unify_nil ( type_pt );
}


foreign_t swi_fann_set_cascade_activation_functions ( term_t ann_pt, term_t functions_pt ) {

	unsigned int i, number_functions;
	enum fann_activationfunc_enum *function_list;
	term_t function_pt = PL_new_term_ref ();
	char *function;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	number_functions = fann_get_cascade_activation_functions_count ( ann );

	for ( i = 0; i < number_functions; i++ ) {

		PL_get_list ( functions_pt, function_pt, functions_pt );
		PL_get_chars ( function_pt, &function, CVT_ATOM );
        function_list[i] = lookup_activationfunc_enum ( function );
	}

	fann_set_cascade_activation_functions ( ann, function_list, number_functions );

	PL_succeed;
}


foreign_t swi_fann_get_cascade_activation_steepnesses_count ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_integer ( out_pt, fann_get_cascade_activation_steepnesses_count ( ann ) );
}


foreign_t swi_fann_get_cascade_activation_steepnesses ( term_t ann_pt, term_t type_pt ) {

	unsigned int i;
	term_t temp_pt = PL_new_term_ref ();
	fann_type *type_list;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	type_list = fann_get_cascade_activation_steepnesses ( ann );

	for ( i = 0; i < fann_get_cascade_activation_steepnesses_count ( ann ); i++ ) {

		if ( !PL_unify_list ( type_pt, temp_pt, type_pt ) ||
			 !PL_FANN_UNIFY_FANNTYPE(temp_pt,type_list[i]) ) PL_fail;

	}
	PL_unify_nil ( type_pt );

	PL_succeed;
}


foreign_t swi_fann_set_cascade_activation_steepnesses ( term_t ann_pt, term_t type_list_pt ) {

	unsigned int i, steepnesses_count;
	term_t temp_pt = PL_new_term_ref ();
	fann_type temp, *type_list;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	steepnesses_count = fann_get_cascade_activation_steepnesses_count ( ann );

	for ( i = 0; i < steepnesses_count; i++ ) {

		if ( !PL_get_list ( type_list_pt, temp_pt, type_list_pt ) ||
			 !PL_FANN_GET_FANNTYPE(temp_pt,&temp) ) PL_fail;

		type_list[i++] = temp;
		}

	fann_set_cascade_activation_steepnesses ( ann, type_list, steepnesses_count );

	PL_succeed;
}


foreign_t swi_fann_get_cascade_num_candidate_groups ( term_t ann_pt, term_t out_pt ) {

	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ) PL_fail;

	return PL_unify_integer ( out_pt, fann_get_cascade_num_candidate_groups ( ann ) );
}


foreign_t swi_fann_set_cascade_num_candidate_groups ( term_t ann_pt, term_t in_pt ) {

	unsigned int in;
	void *ann;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_integer ( in_pt, &in ) ) PL_fail;

	fann_set_cascade_num_candidate_groups ( ann, in );

	PL_succeed;
}


foreign_t swi_fann_create_from_file ( term_t file_pt, term_t ann_pt ) {

	char *file;

    if ( !PL_get_file_name ( file_pt, &file, PL_FILE_ABSOLUTE || PL_FILE_SEARCH || PL_FILE_EXIST ) ) PL_fail;

	return PL_unify_pointer ( ann_pt, fann_create_from_file ( file ) );
}


foreign_t swi_fann_save ( term_t ann_pt, term_t file_pt ) {

	void *ann;
	char *file;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
	     !PL_get_file_name ( file_pt, &file, PL_FILE_ABSOLUTE ) ) PL_fail;

	if ( !fann_save ( ann, file ) ) PL_succeed; else PL_fail;
}


foreign_t swi_fann_save_to_fixed ( term_t ann_pt, term_t file_pt ) {

	void *ann;
	char *file;

	if ( !PL_get_pointer ( ann_pt, &ann ) ||
		 !PL_get_file_name ( file_pt, &file, PL_FILE_ABSOLUTE ) ) PL_fail;

	if ( !fann_save_to_fixed ( ann, file ) ) PL_succeed; else PL_fail;
}


foreign_t swi_fann_set_error_log ( term_t error_data_pt, term_t file_pt ) {

	void *error_data;
	char *file, *null;


	if ( PL_get_chars ( file_pt, &null, CVT_ATOM ) && !strcmp ( "NULL", null ) ) {

		PL_get_pointer ( error_data_pt, &error_data );
		fann_set_error_log ( error_data, NULL );
		PL_succeed;
	}

	if ( PL_get_chars ( error_data_pt, &null, CVT_ATOM ) && !strcmp ( "NULL", null ) ) {

		PL_get_file_name ( file_pt, &file, PL_FILE_ABSOLUTE );
		fann_set_error_log ( NULL, ( FILE * ) file );
		PL_succeed;
	}

	if ( !PL_get_pointer ( error_data_pt, &error_data ) ||
		 !PL_get_file_name ( file_pt, &file, PL_FILE_ABSOLUTE ) ) PL_fail;

	fann_set_error_log ( error_data, ( FILE * ) file );

	PL_succeed;
}


foreign_t swi_fann_get_errno ( term_t error_data_pt, term_t last_error_pt ) {

	void *error_data;
	char *log_file;

	if ( !PL_get_pointer ( error_data_pt, &error_data ) ) PL_fail;

	return PL_unify_atom_chars ( last_error_pt, FANN_ERROR_CODES[ fann_get_errno ( error_data ) ] );
}


foreign_t swi_fann_reset_errno ( term_t error_data_pt ) {

	void *error_data;

	if ( !PL_get_pointer ( error_data_pt, &error_data ) ) PL_fail;

	fann_reset_errno ( error_data );

	PL_succeed;
}


foreign_t swi_fann_reset_errstr ( term_t error_data_pt ) {

	void *error_data;

	if ( !PL_get_pointer ( error_data_pt, &error_data ) ) PL_fail;

	fann_reset_errstr ( error_data );

	PL_succeed;
}


foreign_t swi_fann_get_errstr ( term_t error_data_pt, term_t last_error_pt ) {

	void *error_data;
	char *log_file;

	if ( !PL_get_pointer ( error_data_pt, &error_data ) ) PL_fail;

	return PL_unify_atom_chars ( last_error_pt, FANN_ERROR_STRING[ fann_get_errno ( error_data ) ] );
}


foreign_t swi_fann_print_error ( term_t error_data_pt ) {

	void *error_data;

	if ( !PL_get_pointer ( error_data_pt, &error_data ) ) PL_fail;

	fann_print_error ( error_data );

	PL_succeed;
}


install_t install() {

	// Creation, Destruction & Execution (12)

	// fann_create_standard: Implemented in plfann.pl, Creates a standard fully connected backpropagation neural network.
	PL_register_foreign ( "fann_create_standard_array", 2, swi_fann_create_standard_array, 0); // Just like fann_create_standard, but with an array of layer sizes instead of individual parameters.
	// fann_create_spares: Implemented in plfann.pl, Creates a standard backpropagation neural network, which is not fully connected.
	PL_register_foreign ( "fann_create_sparse_array", 3, swi_fann_create_sparse_array, 0); // Just like fann_create_sparse, but with an array of layer sizes instead of individual parameters.
	// fann_create_shortcut: Implemented in plfann.pl, Creates a standard backpropagation neural network, which is not fully connected and which also has shortcut connections.
	PL_register_foreign ( "fann_create_shortcut_array", 2, swi_fann_create_shortcut_array, 0); // Just like fann_create_shortcut, but with an array of layer sizes instead of individual parameters.
	PL_register_foreign ( "fann_destroy", 1, swi_fann_destroy, 0); // Destroys the entire network and properly freeing all the associated memmory.
#ifdef VERSION220
	PL_register_foreign ( "fann_copy", 2, swi_fann_copy, 0); // Creates a copy of a fann structure.
#endif
	PL_register_foreign ( "fann_run", 3, swi_fann_run, 0); // Will run input through the neural network, returning an array of outputs, the number of which being equal to the number of neurons in the output layer.
	PL_register_foreign ( "fann_randomize_weights", 3, swi_fann_randomize_weights, 0); // Give each connection a random weight between min_weight and max_weight
	PL_register_foreign ( "fann_init_weights", 2, swi_fann_init_weights, 0); // Initialize the weights using Widrow + Nguyen’s algorithm.
	PL_register_foreign ( "fann_print_connections", 1, swi_fann_print_connections, 0); // Will print the connections of the ann in a compact matrix, for easy viewing of the internals of the ann.

	// Parameters (16)

	PL_register_foreign ( "fann_print_parameters", 1, swi_fann_print_parameters, 0); // Prints all of the parameters and options of the ANN
	PL_register_foreign ( "fann_get_num_input", 2, swi_fann_get_num_input, 0); // Get the number of input neurons.
	PL_register_foreign ( "fann_get_num_output", 2, swi_fann_get_num_output, 0); // Get the number of output neurons.
	PL_register_foreign ( "fann_get_total_neurons", 2, swi_fann_get_total_neurons, 0); // Get the total number of neurons in the entire network.
	PL_register_foreign ( "fann_get_total_connections", 2, swi_fann_get_total_connections, 0); // Get the total number of connections in the entire network.
	PL_register_foreign ( "fann_get_network_type", 2, swi_fann_get_network_type, 0); // Get the type of neural network it was created as.
	PL_register_foreign ( "fann_get_connection_rate", 2, swi_fann_get_connection_rate, 0); // Get the connection rate used when the network was created
	PL_register_foreign ( "fann_get_num_layers", 2, swi_fann_get_num_layers, 0); // Get the number of layers in the network
	PL_register_foreign ( "fann_get_layer_array", 2, swi_fann_get_layer_array, 0); // Get the number of neurons in each layer in the network.
	PL_register_foreign ( "fann_get_bias_array", 2, swi_fann_get_bias_array, 0); // Get the number of bias in each layer in the network.
	PL_register_foreign ( "fann_get_connection_array", 2, swi_fann_get_connection_array, 0); // Get the connections (a pointer to) in the network.
	PL_register_foreign ( "fann_set_weight_array", 2, swi_fann_set_weight_array, 0); // Set connections in the network.
	PL_register_foreign ( "fann_set_weight", 4, swi_fann_set_weight, 0); // Set a connection in the network.
	PL_register_foreign ( "fann_set_user_data", 2, swi_fann_set_user_data, 0); // Store a pointer to user defined data.
	PL_register_foreign ( "fann_get_user_data", 2, swi_fann_get_user_data, 0); // Get a pointer to user defined data that was previously set with fann_set_user_data.
#ifdef VERSION220
	PL_register_foreign ( "fann_get_decimal_point", 2, swi_fann_get_decimal_point, 0); // Returns the position of the decimal point in the ann.
	PL_register_foreign ( "fann_get_multiplier", 2, swi_fann_get_multiplier, 0); // Returns the multiplier that fix point data is multiplied with.
#endif

	// Training (5)

	PL_register_foreign ( "fann_train", 3, swi_fann_train, 0); // Train one iteration with a set of inputs, and a set of desired outputs.
	PL_register_foreign ( "fann_test", 3, swi_fann_test, 0); // Test with a set of inputs, and a set of desired outputs.
	PL_register_foreign ( "fann_get_MSE", 2, swi_fann_get_MSE, 0); // Reads the mean square error from the network.
	PL_register_foreign ( "fann_get_bit_fail", 2, swi_fann_get_bit_fail, 0); // The number of fail bits; means the number of output neurons which differ more than the bit fail limit (see fann_get_bit_fail_limit, fann_set_bit_fail_limit).
	PL_register_foreign ( "fann_reset_MSE", 1, swi_fann_reset_MSE, 0); // Resets the mean square error from the network.

	// Training Data Training (4)

	PL_register_foreign ( "fann_train_on_data", 5, swi_fann_train_on_data, 0); // Trains on an entire dataset, for a period of time.
	PL_register_foreign ( "fann_train_on_file", 5, swi_fann_train_on_file, 0); // Does the same as fann_train_on_data, but reads the training data directly from a file.
	PL_register_foreign ( "fann_train_epoch", 2, swi_fann_train_epoch, 0); // Train one epoch with a set of training data.
	PL_register_foreign ( "fann_test_data", 3, swi_fann_test_data, 0); // Test a set of training data and calculates the MSE for the training data.

	// Training Data Manipulation (25)

	PL_register_foreign ( "fann_read_train_from_file", 2, swi_fann_read_train_from_file, 0); // Reads a file that stores training data.
#ifdef VERSION220
	PL_register_foreign ( "fann_create_train", 4, swi_fann_create_train, 0); // Creates an empty training data struct.
#endif
	// PL_register_foreign ( "fann_create_train_from_callback", na, swi_fann_create_train_from_callback, 0); // Creates the training data struct from a user supplied function.
	PL_register_foreign ( "fann_destroy_train", 1, swi_fann_destroy_train, 0); // Destructs the training data and properly deallocates all of the associated data.
	PL_register_foreign ( "fann_shuffle_train_data", 1, swi_fann_shuffle_train_data, 0); // Shuffles training data, randomizing the order.
	PL_register_foreign ( "fann_scale_train", 2, swi_fann_scale_train, 0); // Scale input and output data based on previously calculated parameters.
	PL_register_foreign ( "fann_descale_train", 2, swi_fann_descale_train, 0); // Descale input and output data based on previously calculated parameters.
	PL_register_foreign ( "fann_set_input_scaling_params", 4, swi_fann_set_input_scaling_params, 0); // Calculate input scaling parameters for future use based on training data.
	PL_register_foreign ( "fann_set_output_scaling_params", 4, swi_fann_set_output_scaling_params, 0); // Calculate output scaling parameters for future use based on training data.
	PL_register_foreign ( "fann_set_scaling_params", 6, swi_fann_set_scaling_params, 0); // Calculate input and output scaling parameters for future use based on training data.
	PL_register_foreign ( "fann_clear_scaling_params", 1, swi_fann_clear_scaling_params, 0); // Clears scaling parameters.
	PL_register_foreign ( "fann_scale_input", 2, swi_fann_scale_input, 0); // Scale data in input vector before feed it to ann based on previously calculated parameters.
	PL_register_foreign ( "fann_scale_output", 2, swi_fann_scale_output, 0); // Scale data in output vector before feed it to ann based on previously calculated parameters.
	PL_register_foreign ( "fann_descale_input", 2, swi_fann_descale_input, 0); // Scale data in input vector after get it from ann based on previously calculated parameters.
	PL_register_foreign ( "fann_descale_output", 2, swi_fann_descale_output, 0); // Scale data in output vector after get it from ann based on previously calculated parameters.
	PL_register_foreign ( "fann_scale_input_train_data", 3, swi_fann_scale_input_train_data, 0); // Scales the inputs in the training data to the specified range.
	PL_register_foreign ( "fann_scale_output_train_data", 3, swi_fann_scale_output_train_data, 0); // Scales the outputs in the training data to the specified range.
	PL_register_foreign ( "fann_scale_train_data", 3, swi_fann_scale_train_data, 0); // Scales the inputs and outputs in the training data to the specified range.
	PL_register_foreign ( "fann_merge_train_data", 3, swi_fann_merge_train_data, 0); // Merges the data from data1 and data2 into a new struct fann_train_data.
	PL_register_foreign ( "fann_duplicate_train_data", 2, swi_fann_duplicate_train_data, 0); // Returns an exact copy of a struct fann_train_data.
	PL_register_foreign ( "fann_subset_train_data", 4, swi_fann_subset_train_data, 0); // Returns an copy of a subset of the struct fann_train_data, starting at position pos and length elements forward.
	PL_register_foreign ( "fann_length_train_data", 2, swi_fann_length_train_data, 0); // Returns the number of training patterns in the struct fann_train_data.
	PL_register_foreign ( "fann_num_input_train_data", 2, swi_fann_num_input_train_data, 0); // Returns the number of inputs in each of the training patterns in the struct fann_train_data.
	PL_register_foreign ( "fann_num_output_train_data", 2, swi_fann_num_output_train_data, 0); // Returns the number of outputs in each of the training patterns in the struct fann_train_data.
	PL_register_foreign ( "fann_save_train", 2, swi_fann_save_train, 0); // Save the training structure to a file, with the format as specified in fann_read_train_from_file
	PL_register_foreign ( "fann_save_train_to_fixed", 3, swi_fann_save_train_to_fixed, 0); // Saves the training structure to a fixed point data file.

	// Parameters (44)

	PL_register_foreign ( "fann_get_training_algorithm", 2, swi_fann_get_training_algorithm, 0); // Return the training algorithm as described by fann_train_enum.
	PL_register_foreign ( "fann_set_training_algorithm", 2, swi_fann_set_training_algorithm, 0); // Set the training algorithm.
	PL_register_foreign ( "fann_get_learning_rate", 2, swi_fann_get_learning_rate, 0); // Return the learning rate.
	PL_register_foreign ( "fann_set_learning_rate", 2, swi_fann_set_learning_rate, 0); // Set the learning rate.
	PL_register_foreign ( "fann_get_learning_momentum", 2, swi_fann_get_learning_momentum, 0); // Get the learning momentum.
	PL_register_foreign ( "fann_set_learning_momentum", 2, swi_fann_set_learning_momentum, 0); // Set the learning momentum.
	PL_register_foreign ( "fann_get_activation_function", 4, swi_fann_get_activation_function, 0); // Get the activation function for neuron number neuron in layer number layer, counting the input layer as layer 0.
	PL_register_foreign ( "fann_set_activation_function", 4, swi_fann_set_activation_function, 0); // Set the activation function for neuron number neuron in layer number layer, counting the input layer as layer 0.
	PL_register_foreign ( "fann_set_activation_function_layer", 3, swi_fann_set_activation_function_layer, 0); // Set the activation function for all the neurons in the layer number layer, counting the input layer as layer 0.
	PL_register_foreign ( "fann_set_activation_function_hidden", 2, swi_fann_set_activation_function_hidden, 0); // Set the activation function for all of the hidden layers.
	PL_register_foreign ( "fann_set_activation_function_output", 2, swi_fann_set_activation_function_output, 0); // Set the activation function for the output layer.
	PL_register_foreign ( "fann_get_activation_steepness", 4, swi_fann_get_activation_steepness, 0); // Get the activation steepness for neuron number neuron in layer number layer, counting the input layer as layer 0.
	PL_register_foreign ( "fann_set_activation_steepness", 4, swi_fann_set_activation_steepness, 0); // Set the activation steepness for neuron number neuron in layer number layer, counting the input layer as layer 0.
	PL_register_foreign ( "fann_set_activation_steepness_layer", 3, swi_fann_set_activation_steepness_layer, 0); // Set the activation steepness all of the neurons in layer number layer, counting the input layer as layer 0.
	PL_register_foreign ( "fann_set_activation_steepness_hidden", 2, swi_fann_set_activation_steepness_hidden, 0); // Set the steepness of the activation steepness in all of the hidden layers.
	PL_register_foreign ( "fann_set_activation_steepness_output", 2, swi_fann_set_activation_steepness_output, 0); // Set the steepness of the activation steepness in the output layer.
	PL_register_foreign ( "fann_get_train_error_function", 2, swi_fann_get_train_error_function, 0); // Returns the error function used during training.
	PL_register_foreign ( "fann_set_train_error_function", 2, swi_fann_set_train_error_function, 0); // Set the error function used during training.
	PL_register_foreign ( "fann_get_train_stop_function", 2, swi_fann_get_train_stop_function, 0); // Returns the the stop function used during training.
	PL_register_foreign ( "fann_set_train_stop_function", 2, swi_fann_set_train_stop_function, 0); // Set the stop function used during training.
	PL_register_foreign ( "fann_get_bit_fail_limit", 2, swi_fann_get_bit_fail_limit, 0); // Returns the bit fail limit used during training.
	PL_register_foreign ( "fann_set_bit_fail_limit", 2, swi_fann_set_bit_fail_limit, 0); // Set the bit fail limit used during training.
	// PL_register_foreign ( "fann_set_callback", na, swi_fann_set_callback, 0); // Sets the callback function for use during training.
	PL_register_foreign ( "fann_get_quickprop_decay", 2, swi_fann_get_quickprop_decay, 0); // The decay is a small negative valued number which is the factor that the weights should become smaller in each iteration during quickprop training.
	PL_register_foreign ( "fann_set_quickprop_decay", 2, swi_fann_set_quickprop_decay, 0); // Sets the quickprop decay factor.
	PL_register_foreign ( "fann_get_quickprop_mu", 2, swi_fann_get_quickprop_mu, 0); // The mu factor is used to increase and decrease the step-size during quickprop training.
	PL_register_foreign ( "fann_set_quickprop_mu", 2, swi_fann_set_quickprop_mu, 0); // Sets the quickprop mu factor.
	PL_register_foreign ( "fann_get_rprop_increase_factor", 2, swi_fann_get_rprop_increase_factor, 0); // The increase factor is a value larger than 1, which is used to increase the step-size during RPROP training.
	PL_register_foreign ( "fann_set_rprop_increase_factor", 2, swi_fann_set_rprop_increase_factor, 0); // The increase factor used during RPROP training.
	PL_register_foreign ( "fann_get_rprop_decrease_factor", 2, swi_fann_get_rprop_decrease_factor, 0); // The decrease factor is a value smaller than 1, which is used to decrease the step-size during RPROP training.
	PL_register_foreign ( "fann_set_rprop_decrease_factor", 2, swi_fann_set_rprop_decrease_factor, 0); // The decrease factor is a value smaller than 1, which is used to decrease the step-size during RPROP training.
	PL_register_foreign ( "fann_get_rprop_delta_min", 2, swi_fann_get_rprop_delta_min, 0); // The minimum step-size is a small positive number determining how small the minimum step-size may be.
	PL_register_foreign ( "fann_set_rprop_delta_min", 2, swi_fann_set_rprop_delta_min, 0); // The minimum step-size is a small positive number determining how small the minimum step-size may be.
	PL_register_foreign ( "fann_get_rprop_delta_max", 2, swi_fann_get_rprop_delta_max, 0); // The maximum step-size is a positive number determining how large the maximum step-size may be.
	PL_register_foreign ( "fann_set_rprop_delta_max", 2, swi_fann_set_rprop_delta_max, 0); // The maximum step-size is a positive number determining how large the maximum step-size may be.
	PL_register_foreign ( "fann_get_rprop_delta_zero", 2, swi_fann_get_rprop_delta_zero, 0); // The initial step-size is a positive number determining the initial step size.
	PL_register_foreign ( "fann_set_rprop_delta_zero", 2, swi_fann_set_rprop_delta_zero, 0); // The initial step-size is a positive number determining the initial step size.
#ifdef VERSION220
	PL_register_foreign ( "fann_get_sarprop_weight_decay_shift", 2, swi_fann_get_sarprop_weight_decay_shift, 0); // The sarprop weight decay shift.
	PL_register_foreign ( "fann_set_sarprop_weight_decay_shift", 2, swi_fann_set_sarprop_weight_decay_shift, 0); // Set the sarprop weight decay shift.
	PL_register_foreign ( "fann_get_sarprop_step_error_threshold_factor", 2, swi_fann_get_sarprop_step_error_threshold_factor, 0); // The sarprop step error threshold factor.
	PL_register_foreign ( "fann_set_sarprop_step_error_threshold_factor", 2, swi_fann_set_sarprop_step_error_threshold_factor, 0); // Set the sarprop step error threshold factor.
	PL_register_foreign ( "fann_get_sarprop_step_error_shift", 2, swi_fann_get_sarprop_step_error_shift, 0); // The get sarprop step error shift.
	PL_register_foreign ( "fann_set_sarprop_step_error_shift", 2, swi_fann_set_sarprop_step_error_shift, 0); // Set the sarprop step error shift.
	PL_register_foreign ( "fann_get_sarprop_temperature", 2, swi_fann_get_sarprop_temperature, 0); // The sarprop weight decay shift.
	PL_register_foreign ( "fann_set_sarprop_temperature", 2, swi_fann_set_sarprop_temperature, 0); // Set the sarprop_temperature.
#endif

	// Cascade Training (2)

	PL_register_foreign ( "fann_cascadetrain_on_data", 5, swi_fann_cascadetrain_on_data, 0); // Trains on an entire dataset, for a period of time using the Cascade2 training algorithm.
	PL_register_foreign ( "fann_cascadetrain_on_file", 5, swi_fann_cascadetrain_on_file, 0); // Does the same as fann_cascadetrain_on_data, but reads the training data directly from a file.

	// Parameters (28)

	PL_register_foreign ( "fann_get_cascade_output_change_fraction", 2, swi_fann_get_cascade_output_change_fraction, 0); // The cascade output change fraction is a number between 0 and 1 determining how large a fraction the fann_get_MSE value should change within fann_get_cascade_output_stagnation_epochs during training of the output connections, in order for the training not to stagnate.
	PL_register_foreign ( "fann_set_cascade_output_change_fraction", 2, swi_fann_set_cascade_output_change_fraction, 0); // Sets the cascade output change fraction.
	PL_register_foreign ( "fann_get_cascade_output_stagnation_epochs", 2, swi_fann_get_cascade_output_stagnation_epochs, 0); // The number of cascade output stagnation epochs determines the number of epochs training is allowed to continue without changing the MSE by a fraction of fann_get_cascade_output_change_fraction.
	PL_register_foreign ( "fann_set_cascade_output_stagnation_epochs", 2, swi_fann_set_cascade_output_stagnation_epochs, 0); // Sets the number of cascade output stagnation epochs.
	PL_register_foreign ( "fann_get_cascade_candidate_change_fraction", 2, swi_fann_get_cascade_candidate_change_fraction, 0); // The cascade candidate change fraction is a number between 0 and 1 determining how large a fraction the fann_get_MSE value should change within fann_get_cascade_candidate_stagnation_epochs during training of the candidate neurons, in order for the training not to stagnate.
	PL_register_foreign ( "fann_set_cascade_candidate_change_fraction", 2, swi_fann_set_cascade_candidate_change_fraction, 0); // Sets the cascade candidate change fraction.
	PL_register_foreign ( "fann_get_cascade_candidate_stagnation_epochs", 2, swi_fann_get_cascade_candidate_stagnation_epochs, 0); // The number of cascade candidate stagnation epochs determines the number of epochs training is allowed to continue without changing the MSE by a fraction of fann_get_cascade_candidate_change_fraction.
	PL_register_foreign ( "fann_set_cascade_candidate_stagnation_epochs", 2, swi_fann_set_cascade_candidate_stagnation_epochs, 0); // Sets the number of cascade candidate stagnation epochs.
	PL_register_foreign ( "fann_get_cascade_weight_multiplier", 2, swi_fann_get_cascade_weight_multiplier, 0); // The weight multiplier is a parameter which is used to multiply the weights from the candidate neuron before adding the neuron to the neural network.
	PL_register_foreign ( "fann_set_cascade_weight_multiplier", 2, swi_fann_set_cascade_weight_multiplier, 0); // Sets the weight multiplier.
	PL_register_foreign ( "fann_get_cascade_candidate_limit", 2, swi_fann_get_cascade_candidate_limit, 0); // The candidate limit is a limit for how much the candidate neuron may be trained.
	PL_register_foreign ( "fann_set_cascade_candidate_limit", 2, swi_fann_set_cascade_candidate_limit, 0); // Sets the candidate limit.
	PL_register_foreign ( "fann_get_cascade_max_out_epochs", 2, swi_fann_get_cascade_max_out_epochs, 0); // The maximum out epochs determines the maximum number of epochs the output connections may be trained after adding a new candidate neuron.
	PL_register_foreign ( "fann_set_cascade_max_out_epochs", 2, swi_fann_set_cascade_max_out_epochs, 0); // Sets the maximum out epochs.
#ifdef VERSION220
	PL_register_foreign ( "fann_get_cascade_min_out_epochs", 2, swi_fann_get_cascade_min_out_epochs, 0); // The minimum out epochs determines the minimum number of epochs the output connections must be trained after adding a new candidate neuron.
	PL_register_foreign ( "fann_set_cascade_min_out_epochs", 2, swi_fann_set_cascade_min_out_epochs, 0); // Sets the minimum out epochs.
	PL_register_foreign ( "fann_get_cascade_max_cand_epochs", 2, swi_fann_get_cascade_max_cand_epochs, 0); // The maximum candidate epochs determines the maximum number of epochs the input connections to the candidates may be trained before adding a new candidate neuron.
	PL_register_foreign ( "fann_set_cascade_max_cand_epochs", 2, swi_fann_set_cascade_max_cand_epochs, 0); // Sets the max candidate epochs.
	PL_register_foreign ( "fann_get_cascade_min_cand_epochs", 2, swi_fann_get_cascade_min_cand_epochs, 0); // The minimum candidate epochs determines the minimum number of epochs the input connections to the candidates may be trained before adding a new candidate neuron.
	PL_register_foreign ( "fann_set_cascade_min_cand_epochs", 2, swi_fann_set_cascade_min_cand_epochs, 0); // Sets the min candidate epochs.
#endif
	PL_register_foreign ( "fann_get_cascade_num_candidates", 2, swi_fann_get_cascade_num_candidates, 0); // The number of candidates used during training (calculated by multiplying fann_get_cascade_activation_functions_count, fann_get_cascade_activation_steepnesses_count and fann_get_cascade_num_candidate_groups).
	PL_register_foreign ( "fann_get_cascade_activation_functions_count", 2, swi_fann_get_cascade_activation_functions_count, 0); // The number of activation functions in the fann_get_cascade_activation_functions array.
	PL_register_foreign ( "fann_get_cascade_activation_functions", 2, swi_fann_get_cascade_activation_functions, 0); // The cascade activation functions array is an array of the different activation functions used by the candidates.
	PL_register_foreign ( "fann_set_cascade_activation_functions", 2, swi_fann_set_cascade_activation_functions, 0); // Sets the array of cascade candidate activation functions.
	PL_register_foreign ( "fann_get_cascade_activation_steepnesses_count", 2, swi_fann_get_cascade_activation_steepnesses_count, 0); // The number of activation steepnesses in the fann_get_cascade_activation_functions array.
	PL_register_foreign ( "fann_get_cascade_activation_steepnesses", 2, swi_fann_get_cascade_activation_steepnesses, 0); // The cascade activation steepnesses array is an array of the different activation functions used by the candidates.
	PL_register_foreign ( "fann_set_cascade_activation_steepnesses", 2, swi_fann_set_cascade_activation_steepnesses, 0); // Sets the array of cascade candidate activation steepnesses.
	PL_register_foreign ( "fann_get_cascade_num_candidate_groups", 2, swi_fann_get_cascade_num_candidate_groups, 0); // The number of candidate groups is the number of groups of identical candidates which will be used during training.
	PL_register_foreign ( "fann_set_cascade_num_candidate_groups", 2, swi_fann_set_cascade_num_candidate_groups, 0); // Sets the number of candidate groups.

	// File Input and Output (3)

	PL_register_foreign ( "fann_create_from_file", 2, swi_fann_create_from_file, 0); // Constructs a backpropagation neural network from a configuration file, which have been saved by fann_save.
	PL_register_foreign ( "fann_save", 2, swi_fann_save, 0); // Save the entire network to a configuration file.
	PL_register_foreign ( "fann_save_to_fixed", 2, swi_fann_save_to_fixed, 0); // Saves the entire network to a configuration file.

	// Error Handling (6)

	PL_register_foreign ( "fann_set_error_log", 2, swi_fann_set_error_log, 0); // Change where errors are logged to.
	PL_register_foreign ( "fann_get_errno", 2, swi_fann_get_errno, 0); // Returns the last error number.
	PL_register_foreign ( "fann_reset_errno", 1, swi_fann_reset_errno, 0); // Resets the last error number.
	PL_register_foreign ( "fann_reset_errstr", 1, swi_fann_reset_errstr, 0); // Resets the last error string.
    PL_register_foreign ( "fann_get_errstr", 2, swi_fann_get_errstr, 0); // Returns the last errstr.
	PL_register_foreign ( "fann_print_error", 1, swi_fann_print_error, 0); // Prints the last error to stderr.
}

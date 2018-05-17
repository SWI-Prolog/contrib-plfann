/** <module> SWI-Prolog for "Fast Artificial Neural Network Library"

The  enclosed files plfann.h, plfann.c and plfann.pl form the SWI-Prolog
bindings  to the Fast Artificial Neural Network  library. fann is an open
source ANN-library and is released under the LGPL.

The version of the library for which these bindings are intended to be used with
is  version  2.1.0.  Functions foreseen  to be  available in  release 2.2.0  are
equally included, but have been commented out.

On the  fann home page, links  are provided to  the Source  Code, the  Reference
Manual and additionally, more general, materials related to ANN's.

fann home page: http://leenissen.dk/fann/

All available functions are implemented, with the exception of:

     1. fann_create_train_from_callback
     2. fann_set_callback

In total 150 public predicates are defined.


The  predicate names, used in  this library,  are the  same as the  ones in  the
reference manual. C-Function-return-arguments  are added  as an additional  (and
last) argument  to the  predicates concerned.  In a small  number of  functions,
having length values, like  f.e. the length of  an input array, these  arguments
have been removed, as these can be obtained in the prolog interface at relative-
ly low cost.

fann can use floats,  doubles or fixed point  (not for training)  representation
internally, uncomment the appropriate variable definition in plfann.h.


Three additional functions/predicates have been added:

    1. fann_type(-Type)

        Unifies Type with 'FANN_FLOAT', 'FANN_DOUBLE' or 'FANN_FIXED', depending
        on the compilation of the library.

    2. fann_error(+ErrorData)

        Is true if an error occurred. ErrorData refers to the ErrorData set by
        fann_set_error_log/2, or if no ErrorData is specifically set, to 'NULL'.
        The latter is equivalent to calling fann_print_error/0.

    3. fann_print_mode(?Mode)

        Sets  the printing  of the  library to  either  standard  output  device
        (Mode = 'FANN_NATIVE') or  the  SWI-Prolog  console (Mode = 'FANN_SWI').
        In the latter case, the SWI_Prolog message system is used to this end.

        The loading of the plfann-library  (i.e. on startup of SWI-Prolog) auto-
        matically sets the printing mode to 'FANN_SWI'.

        When called with a free  variable, Mode unifies  with the current  print
        mode, either 'FANN_SWI' or 'FANN_NATIVE'.


The idea of the  above is to ALSO be able  to build just one library (as opposed
to fann and plfann) with  the SWI-Prolog bindings included  and still be able to
use the same library both directly from C/C++ and SWI-Prolog.

In order  for this to work, the header-file  fann_swi.h will need to be included
in fann.h, by adding the line '#include "fann_swi.h"' (without the single quotes)
after the other #include statements (around line 140).

The macro BUILD_FANN_WITH_SWI will  need to be defined, when  building the  fann
library.

The header-file fann_swi.h will need to be copied/moved to the "../src/include/"
directory of the source distribution, this is also where "fann.h" is located.

The result is that iff the library is loaded through SWI-Prolog, the printing of
all output of the library will be directed to the SWI-Prolog console. If  loaded
by another application or library, the standard output is used by default. It is
possible to override this behaviour, both from c and from prolog, see fann_swi.h
for details.

fann can use floats,  doubles or fixed point  (not for training)  representation
internally, uncomment the appropriate macro definition in plfann.h.

There  are some issues  with saving networks  to file. See post "Patch to ensure
locale independancy", http://leenissen.dk/fann/forum/viewtopic.php?f=2&t=595 . A
patch is posted.

dated: 18.12.2009

@author Degski
@author PiotrLi

@license LGPL
*/
:- module( plfann, [

        % When using version 2.2.0, uncomment the
        % functions in the interface definition

        % Specific to plfann (3)

        fann_type/1,
        fann_set_type/1,
        fann_error/1,
        fann_swi_mode/0,
        fann_print_mode/1,

        % Creation, Destruction & Execution (24[25])

        fann_create_standard/4,
        fann_create_standard/5,
        fann_create_standard/6,
        fann_create_standard/7,
        fann_create_standard_array/2,
        fann_create_standard_array/3,
        fann_create_sparse/5,
        fann_create_sparse/6,
        fann_create_sparse/7,
        fann_create_sparse/8,
        fann_create_sparse_array/3,
        fann_create_sparse_array/4,
        fann_create_shortcut/4,
        fann_create_shortcut/5,
        fann_create_shortcut/6,
        fann_create_shortcut/7,
        fann_create_shortcut_array/2,
        fann_create_shortcut_array/3,
        fann_destroy/1,
        % fann_copy/2,
        fann_run/3,
        fann_run_unsafe/3,
        fann_randomize_weights/3,
        fann_init_weights/2,
        fann_print_connections/1,

        % Parameters (16)

        fann_print_parameters/1,
        fann_get_num_input/2,
        fann_get_num_output/2,
        fann_get_total_neurons/2,
        fann_get_total_connections/2,
        fann_get_network_type/2,
        fann_get_connection_rate/2,
        fann_get_num_layers/2,
        fann_get_layer_array/2,
        fann_get_bias_array/2,
        fann_get_connection_array/2,
        fann_set_weight_array/2,
        fann_set_weight/4,
        fann_set_user_data/2,
        fann_get_user_data/2,
        fann_get_decimal_point/2,
        fann_get_multiplier/2,

        % Training (5)

        fann_train/3,
        fann_test/3,
        fann_get_MSE/2,
        fann_get_bit_fail/2,
        fann_reset_MSE/1,

        % Training Data Training (4)

        fann_train_on_data/5,
        fann_train_on_file/5,
        fann_train_epoch/2,
        fann_test_data/3,

        % Training Data Manipulation (24[26])

        fann_read_train_from_file/2,
        % fann_create_train/4,
        % fann_create_train_from_callback/na,
        fann_destroy_train/1,
        fann_shuffle_train_data/1,
        fann_scale_train/2,
        fann_descale_train/2,
        fann_set_input_scaling_params/4,
        fann_set_output_scaling_params/4,
        fann_set_scaling_params/6,
        fann_clear_scaling_params/1,
        fann_scale_input/2,
        fann_scale_output/2,
        fann_descale_input/2,
        fann_descale_output/2,
        fann_scale_input_train_data/3,
        fann_scale_output_train_data/3,
        fann_scale_train_data/3,
        fann_merge_train_data/3,
        fann_duplicate_train_data/2,
        fann_subset_train_data/4,
        fann_length_train_data/2,
        fann_num_input_train_data/2,
        fann_num_output_train_data/2,
        fann_save_train/2,
        fann_save_train_to_fixed/3,

        % Parameters (36[45])

        fann_get_training_algorithm/2,
        fann_set_training_algorithm/2,
        fann_get_learning_rate/2,
        fann_set_learning_rate/2,
        fann_get_learning_momentum/2,
        fann_set_learning_momentum/2,
        fann_get_activation_function/4,
        fann_set_activation_function/4,
        fann_set_activation_function_layer/3,
        fann_set_activation_function_hidden/2,
        fann_set_activation_function_output/2,
        fann_get_activation_steepness/4,
        fann_set_activation_steepness/4,
        fann_set_activation_steepness_layer/3,
        fann_set_activation_steepness_hidden/2,
        fann_set_activation_steepness_output/2,
        fann_get_train_error_function/2,
        fann_set_train_error_function/2,
        fann_get_train_stop_function/2,
        fann_set_train_stop_function/2,
        fann_get_bit_fail_limit/2,
        fann_set_bit_fail_limit/2,
        % fann_set_callback/na,
        fann_get_quickprop_decay/2,
        fann_set_quickprop_decay/2,
        fann_get_quickprop_mu/2,
        fann_set_quickprop_mu/2,
        fann_get_rprop_increase_factor/2,
        fann_set_rprop_increase_factor/2,
        fann_get_rprop_decrease_factor/2,
        fann_set_rprop_decrease_factor/2,
        fann_get_rprop_delta_min/2,
        fann_set_rprop_delta_min/2,
        fann_get_rprop_delta_max/2,
        fann_set_rprop_delta_max/2,
        fann_get_rprop_delta_zero/2,
        fann_set_rprop_delta_zero/2,
        % fann_get_sarprop_weight_decay_shift/2,
        % fann_set_sarprop_weight_decay_shift/2,
        % fann_get_sarprop_step_error_threshold_factor/2,
        % fann_set_sarprop_step_error_threshold_factor/2,
        % fann_get_sarprop_step_error_shift/2,
        % fann_set_sarprop_step_error_shift/2,
        % fann_get_sarprop_temperature/2,
        % fann_set_sarprop_temperature/2,

        %  Cascade Training (2)

        fann_cascadetrain_on_data/5,
        fann_cascadetrain_on_file/5,

        % Parameters (25[29])

        fann_get_cascade_output_change_fraction/2,
        fann_set_cascade_output_change_fraction/2,
        fann_get_cascade_output_stagnation_epochs/2,
        fann_set_cascade_output_stagnation_epochs/2,
        fann_get_cascade_candidate_change_fraction/2,
        fann_set_cascade_candidate_change_fraction/2,
        fann_get_cascade_candidate_stagnation_epochs/2,
        fann_set_cascade_candidate_stagnation_epochs/2,
        fann_get_cascade_weight_multiplier/2,
        fann_set_cascade_weight_multiplier/2,
        fann_get_cascade_candidate_limit/2,
        fann_set_cascade_candidate_limit/2,
        fann_get_cascade_max_out_epochs/2,
        fann_set_cascade_max_out_epochs/2,
        % fann_get_cascade_min_out_epochs/2,
        % fann_set_cascade_min_out_epochs/2,
        fann_get_cascade_max_cand_epochs/2,
        fann_set_cascade_max_cand_epochs/2,
        % fann_get_cascade_min_cand_epochs/2,
        % fann_set_cascade_min_cand_epochs/2,
        fann_get_cascade_num_candidates/2,
        fann_get_cascade_activation_functions_count/2,
        fann_get_cascade_activation_functions/2,
        fann_set_cascade_activation_functions/2,
        fann_get_cascade_activation_steepnesses_count/2,
        fann_get_cascade_activation_steepnesses/2,
        fann_set_cascade_activation_steepnesses/2,
        fann_get_cascade_num_candidate_groups/2,
        fann_set_cascade_num_candidate_groups/2,

        % File Input and Output (3)

        fann_create_from_file/2,
        fann_save/2,
        fann_save_to_fixed/2,

        % Error Handling (8)

        fann_set_error_log/2,
        fann_get_errno/2,
        fann_reset_errno/1,
        fann_reset_errstr/1,
        fann_get_errstr/2,
        fann_print_error/0,
        fann_print_error/1,
        fann_print_error_core/1
    ]).


:- load_foreign_library( foreign( plfann ) ).

%!	fann_type(-Type) is det
%
%	Unifies Type with 'FANN_FLOAT', 'FANN_DOUBLE' or 'FANN_FIXED', depending
%	on the compilation of the library.

%!	fann_set_type(+Type) is det
%
%	Sets the library depending type of compilation.
%
%	You can choose 'FANN_FLOAT', 'FANN_DOUBLE' or 'FANN_FIXED'
%
%	@see fann_type/1

fann_set_type(X):-
	var(X),!,fail.
fann_set_type('FANN_FLOAT'):-
	ignore(unload_foreign_library(foreign(plfann_double))),
	ignore(unload_foreign_library(foreign(plfann_fixed))),
	load_foreign_library(foreign(plfann)).
fann_set_type('FANN_DOUBLE'):-
	ignore(unload_foreign_library(foreign(plfann))),
	ignore(unload_foreign_library(foreign(plfann_fixed))),
	load_foreign_library(foreign(plfann_double)).
fann_set_type('FANN_FIXED'):-
	ignore(unload_foreign_library(foreign(plfann_double))),
	ignore(unload_foreign_library(foreign(plfann))),
	load_foreign_library(foreign(plfann_fixed)).

%!	fann_swi_mode is det
%
%	Sets the printing of the library to the SWI-Prolog console.

fann_swi_mode :- fann_print_mode( 'FANN_SWI' ).

%!	fann_print_mode(?Mode) is det
%
%	Sets  the printing  of the  library to  either  standard  output  device
%	(Mode = 'FANN_NATIVE') or  the  SWI-Prolog  console (Mode = 'FANN_SWI').
%	In the latter case, the SWI_Prolog message system is used to this end.
%
%	The loading of the plfann-library  (i.e. on startup of SWI-Prolog) auto-
%	matically sets the printing mode to 'FANN_SWI'.
%
%	When called with a free  variable, Mode unifies  with the current  print
%	mode, either 'FANN_SWI' or 'FANN_NATIVE'.

:- fann_swi_mode.

% Wrapper predicates defined in prolog.
% -------------------------------------

%!	fann_create_standard(_,A,B,X)
%	@bug First argument is the number of layers, but is ignored

fann_create_standard(_, A, B, X) :-
        fann_create_standard_array([ A, B], X), !.
fann_create_standard(_, _, _, _) :- !, fail.
fann_create_standard(_, A, B, C, X) :-
        fann_create_standard_array([ A, B, C], X), !.
fann_create_standard(_, _, _, _, _) :- !, fail.
fann_create_standard(_, A, B, C, D, X) :-
        fann_create_standard_array([ A, B, C, D], X), !.
fann_create_standard(_, _, _, _, _, _) :- !, fail.
fann_create_standard(_, A, B, C, D, E, X) :-
        fann_create_standard_array([ A, B, C, D, E], X), !.
fann_create_standard(_, _, _, _, _, _, _) :- !, fail.

%!	fann_create_standard_array(X,Y)

%!	fann_create_standard_array(_,X,Y)
%	@bug First argument is the number of layers, but is ignored

fann_create_standard_array(_, X, Y) :-
        fann_create_standard_array(X, Y), !.
fann_create_standard_array(_, _, _) :- !, fail.

%!	fann_create_sparse(Y,_,A,B,X)
%	@bug First argument is the number of layers, but is ignored

fann_create_sparse(Y, _, A, B, X) :-
        fann_create_sparse_array(Y, [ A, B], X), !.
fann_create_sparse(_, _, _, _, _) :- !, fail.
fann_create_sparse(Y, _, A, B, C, X) :-
        fann_create_sparse_array(Y, [ A, B, C], X), !.
fann_create_sparse(_, _, _, _, _, _) :- !, fail.
fann_create_sparse(Y, _, A, B, C, D, X) :-
        fann_create_sparse_array(Y, [ A, B, C, D], X), !.
fann_create_sparse(_, _, _, _, _, _, _) :- !, fail.
fann_create_sparse(Y, _, A, B, C, D, E, X) :-
        fann_create_sparse_array(Y, [ A, B, C, D, E], X), !.
fann_create_sparse(_, _, _, _, _, _, _, _) :- !, fail.

%!	fann_create_sparse_array(X,_,Y,Z)
%	@bug First argument is the number of layers, but is ignored

fann_create_sparse_array(X, _, Y, Z) :-
        fann_create_sparse_array(X, Y, Z), !.
fann_create_sparse_array(_, _, _, _) :- !, fail.

%!	fann_create_shortcut(_,A,B,X)
%	@bug First argument is the number of layers, but is ignored

fann_create_shortcut(_, A, B, X) :-
        fann_create_shortcut_array([ A, B], X), !.
fann_create_shortcut(_, _, _, _) :- !, fail.
fann_create_shortcut(_, A, B, C, X) :-
        fann_create_shortcut_array([ A, B, C], X), !.
fann_create_shortcut(_, _, _, _, _) :- !, fail.
fann_create_shortcut(_, A, B, C, D, X) :-
        fann_create_shortcut_array([ A, B, C, D], X), !.
fann_create_shortcut(_, _, _, _, _, _) :- !, fail.
fann_create_shortcut(_, A, B, C, D, E, X) :-
        fann_create_shortcut_array([ A, B, C, D, E], X), !.
fann_create_shortcut(_, _, _, _, _, _, _) :- !, fail.

%!	fann_create_shortcut_array(+X:list,-Y:var) is det

%!	fann_create_shortcut_array(_ ,X,Y)
%	@bug First argument is the number of layers, but is ignored

fann_create_shortcut_array(_, X, Y) :-
        fann_create_shortcut_array( X, Y), !.
fann_create_shortcut_array(_, _, _) :- !, fail.

% Error Printing through the SWI-Prolog Message system.
% -----------------------------------------------------

%! fann_error(+ErrorData)
%
%	Is true if an error occurred. ErrorData refers to the ErrorData set by
%	fann_set_error_log/2, or if no ErrorData is specifically set, to 'NULL'.
%	The latter is equivalent to calling fann_print_error/0.

%!	fann_print_error(ErrorData)

fann_print_error( ErrorData ) :-
		fann_error( ErrorData ),
		fann_get_errno( ErrorData, LastErrorNo ),
		fann_get_errstr( ErrorData, LastErrorStr ),
		ErrorMessage =.. [ error, LastErrorNo, LastErrorStr ],
		print_message( error, plfann( ErrorMessage ) ), !.
fann_print_error( _ ) :- !.

%!	fann_print_error

fann_print_error :-
        fann_print_error( 'NULL' ), !.


% Messages.
% ---------

:- multifile
	prolog:message/3.

% print message (called directly from c).
% ---------------------------------------

prolog:message( plfann( print_message( Message ) ) ) -->
        [ '~s'-[Message], flush ].

prolog:message( plfann( print_message_at_same_line( Message ) ) ) -->
        [ at_same_line, '~s'-[Message], flush ].


% print errors.
% -------------

prolog:message( plfann( error( No, Str ) ) ) -->
        [ 'FANN Error ~d: ~s'-[No, Str] ].

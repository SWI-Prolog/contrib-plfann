
/*

#include "fann.h"

int main()
{
        const unsigned int num_input = 2;
        const unsigned int num_output = 1;
        const unsigned int num_layers = 3;
        const unsigned int num_neurons_hidden = 3;
        const float desired_error = (const float) 0.001;
        const unsigned int max_epochs = 500000;
        const unsigned int epochs_between_reports = 1000;

        struct fann *ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);

        printf("\n");
        fann_print_connections(ann);
        printf("\n");
        fann_print_parameters(ann);
        printf("\n");

        fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
        fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

        fann_train_on_file(ann, "xor.data", max_epochs, epochs_between_reports, desired_error);
        printf("\n");

        fann_save(ann, "xor_float.net");

        fann_destroy(ann);

        return 0;
}

*/

:- use_module(library(plfann)).

% Learning the XOR Function.
% --------------------------

main:-
    training,
    execution.

training:-
    fann_create_standard( 3, 2, 3, 1, Ann ),

    % More economical would be:
    % fann_create_standard( [2, 3, 1], Ann ),

    nl,
    fann_print_connections( Ann ),
    nl,
    fann_print_parameters( Ann ),
    nl,

    fann_set_activation_function_hidden( Ann, 'FANN_SIGMOID_SYMMETRIC' ),
    fann_set_activation_function_output( Ann, 'FANN_SIGMOID_SYMMETRIC' ),

    % Insert the proper paths below

    fann_train_on_file( Ann, "xor.data", 500000, 1000, 0.00001 ),
    nl,
    fann_save( Ann, "xor_float.net" ),
    fann_destroy( Ann ).

execution:-
    fann_create_from_file("xor_float.net",Ann),
    I1= -1,
    I2= 1,
    fann_run(Ann,[I1,I2],Out),
    format('xor test (~f,~f) -> ',[I1,I2]),
    write(Out),nl,
    fann_destroy(Ann).










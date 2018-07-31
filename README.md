# plfann

This is [fann](http://leenissen.dk/fann/wp/) for [swipl](http://www.swi-prolog.org/).

## Documentation

To find out how to use this library follow [Documentation of FANN](http://leenissen.dk/fann/wp/).

## Installation

1. First [install fann](http://leenissen.dk/fann/wp/help/installing-fann/)!

1. If you already have fann, compile code by `make` and install in swipl:

```bash
?- pack_install('plfann-*.tgz').
```

## Getting Started

If the installation went well, you can load the plfann library with the following command:

```bash
?- [library(plfann)].
```

For general help information about FANN, please go the main FANN [Help page](http://leenissen.dk/fann/wp/help/).

An ANN is normally run in two different modes, a training mode and an execution mode. Although it is possible to do this in the same program, using different programs is recommended.

There are several reasons to why it is usually a good idea to write the training and execution in two different programs, but the most obvious is the fact that a typical ANN system is only trained once, while it is executed many times.

### Training

The following is a simple program which trains an ANN with a data set and then saves the ANN to a file.

#### Simple training example

```Prolog
:- use_module(library(plfann)).

training:-
	%fann_create_standard(+Num_layers,+Num_input,+Num_neurons_hidden,+Num_output,-Ann) is det
	fann_create_standard( 3, 2, 3, 1, Ann ),

	nl,
	fann_print_connections( Ann ),
	nl,
	fann_print_parameters( Ann ),
	nl,

	fann_set_activation_function_hidden( Ann, 'FANN_SIGMOID_SYMMETRIC' ),
	fann_set_activation_function_output( Ann, 'FANN_SIGMOID_SYMMETRIC' ),

	% Insert the proper paths below

	%fann_train_on_file(+Ann,+File,+Max_epochs,+Epochs_between_reports,+Desired_error) is det
	fann_train_on_file( Ann, "xor.data", 500000, 1000, 0.00001 ),
	nl,
	fann_save( Ann, "xor_float.net" ),
	fann_destroy( Ann ).
```

#### The file xor.data, used to train the xor function

```
4 2 1
-1 -1
-1
-1 1
1
1 -1
1
1 1
-1
```

The first line consists of three numbers: The first is the number of training pairs in the file, the second is the number of inputs and the third is the number of outputs. The rest of the file is the actual training data, consisting of one line with inputs, one with outputs etc.

This example introduces several fundamental functions, namely fann_create_standard/5, fann_train_on_file/5, fann_save/2, and fann_destroy/1.

### Execution

The following example shows a simple program which executes a single input on the ANN. The program introduces two new functions (fann_create_from_file/2 and fann_run/3) which were not used in the training procedure, as well as the fann_type type.

#### Simple execution example

```Prolog
execution:-
	fann_create_from_file("xor_float.net",Ann),
	fann_run(Ann,[-1,1],Out),
	format('xor test (-1,1) -> ~f',Out),
	nl,
	fann_destroy(Ann).
```

If after reading the documentation you are still having problems, or have a question that is not covered in the documentation, please consult the [FANN forum](http://leenissen.dk/fann/forum).

## Contribute

* Source Code of FANN: github.com/libfann/fann
* Issue Tracker of FANN: github.com/libfann/fann/issues
* Source Code of plfann: github.com/SWI-Prolog/contrib-plfann/issues
* Issue Tracker of plfann: github.com/SWI-Prolog/contrib-plfann/issues

## License

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

## Author

Steffen Nissen
Degski
PiotrLi

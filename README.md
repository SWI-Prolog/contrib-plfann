# plfann

This is [fann](http://leenissen.dk/fann/wp/) for [swipl](http://www.swi-prolog.org/).

Simple code example 

```Prolog
:- use_module(library(plfann)).

% Learning the XOR Function.
% --------------------------
main:- 
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
```

```Prolog
?- main.
```
## Documentation

To find out how to use this library follow [Documentation of FANN](http://leenissen.dk/fann/wp/).

## Installation

1. First [instal fann](http://leenissen.dk/fann/wp/help/installing-fann/)!

1. If you already have fann, compile code by `make` and install in swipl:

```bash
?- pack_install('plfann-*.tgz').
```

## Usage
If the installation went well, you can load the plfann library with the following command:

```bash
?- [library(plfann)].
```

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

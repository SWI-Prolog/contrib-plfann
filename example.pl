
:- fann_create_standard_array([5,6,2], X),
   fann_randomize_weights(X, -1.9, 1.9),
   fann_run(X, [0.7,-0.4,0.8,0.2,-0.1], Y),
   fann_destroy(X),
   write(Y).
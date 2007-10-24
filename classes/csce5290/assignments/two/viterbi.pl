#!/usr/bin/perl -Wall

use strict;

sub Viterbi {
	# Initialization
	
	# N is the number of vertices in the state graph
	# for state s: 1 to N
	for (my $s = 1; $s < $N; $s++) {
		# viterbi is the matrix
		# a is the transition probability
		# b is the observation likelihood
		
		$viterbi[$s][1] = $a[0][$s] * $b{"$s"}{'.'};
		# backpointer will store the vertex we came from
		$backpointer[$s][1] = 0;
	}

	# Recursion
	
	# T is the number of observations
	# for time t: 2 < T
	for (my $t = 2; $t < $T; $t++) {
		# for state s: 1 < N
		for (my $s = 1; $s < $N; $s++) {
			my $s_prime = ;
			# the position in the matrix (s,t) is the max of the path probabilities
			$viterbi[$s][$t] = max $viterbi[$s_prime][$t - 1] * $a[$s_prime][$s] * $b{"$s"}{"$o_t"};
			$backpointer[$s][$t] = arg max $viterbi[$s_prime][$T] * $a[$s_prime][$s];
		}
	}

	# Termination and path-readout
	$viterbi[$q_F][$T] = max $viterbi[$s][$T] * $a[$s][$q_F];
	$backpointer[$q_F][$T] = arg max $viterbi[$s][$T] * $a[$s][$q_F];
}
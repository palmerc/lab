package bigramModel;

use strict;
use vars qw($VERSION);
$VERSION = '0.01';

# Add one smoothing, Good Turing
#P(W|T) = C(T W) + 1 / C(Tprefix) + Vocab(W)  
sub new {
	my $package = shift;
	return bless({}, $package);
}

1;
__END__

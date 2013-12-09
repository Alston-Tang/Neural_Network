#!/usr/bin/perl -w

use strict;

my $fraw = shift;
exit unless defined($fraw);

my $fdata = shift;
exit unless defined($fdata);

my $fans = shift;
exit unless defined($fans);

my %dataTrans;
for (my $i = 0; $i < 3; $i++) {
	$dataTrans{$i} = shift;
	$dataTrans{$i} = $i unless defined($dataTrans{$i});
}
$dataTrans{'?'} = 3;

my %ansTrans;
for (my $i = 0; $i < 3; $i++) {
	$ansTrans{$i} = shift;
	$ansTrans{$i} = $i unless defined($ansTrans{$i});
}
$ansTrans{'?'} = '?';

open(RAW, '<', $fraw) || exit;
open(DATA, '>', $fdata) || exit;
open(ANS, '>', $fans) || exit;

while (my $line = <RAW>) {
	my @data = ($line =~ /([012\?]),?/g);
	for (my $i = 0; $i < $#data; $i++) {
		print DATA $dataTrans{$data[$i]}, $i == $#data - 1 ? "\n" : ' ';
	}
	print ANS $ansTrans{$data[$#data]}, "\n";
}

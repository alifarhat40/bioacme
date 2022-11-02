#!/usr/bin/perl -w
#
# Youfang Cao
# Shanghai Center for Systems Biomedicine
# 2008-05-21
# 
# Parse the results of steady state probability distribution
#


use vars qw($USAGE);

#use strict;
use Getopt::Long;

$USAGE = "parseResults [arguments] --rs resultfile --ss statespacefile --sc scale\nDetailed help:\nparseResults --help\n";

my ($resfile,$ssfile,$scale,$help) =
   (undef,   undef,  200, undef);

&GetOptions('rs|r=s'             => \$resfile,
            'ss|s=s'             => \$ssfile,
            'sc|c=n'             => \$scale,
            'help|h'             => \$help,
            );

#print "$upcutoff\n$downcutoff\n";

if ($help) {
   exec('perldoc', $0);
   die;
}

if (!defined $resfile) {
    die($USAGE . "\nPlease specify an input steady state probability filename.\n");
}

if (!defined $ssfile) {
    die($USAGE . "\nPlease specify an input state space filename.\n");
}

open RES, "$resfile" or die "Cannot open file $!";
open SS, "$ssfile" or die "Cannot open file $!";

my @StateSpace = ();
my @SteadyProb = ();
my @Species = ();
my $N_Species = 0;
my $N_States = 0;

# READ DATA
my @ary = ();
my $n = 0;
while (<SS>)
{
	chop;
	if ($n == 0)
	{
		@ary = split (" ", $_);
		$N_Species = $ary[0];
		$N_States = $ary[1];
		$n ++;
	}
	elsif ($n == 1)
	{
		@Species = split (" ", $_);
		$n ++;
	}
	else
	{
		@ary = split (" ", $_);
		push @StateSpace, [ @ary ];
	}
}

$n = 0;
while (<RES>)
{
	chop;
	$SteadyProb[$n] = $_;
	$n ++;
}

# BEGIN TO HANDLE IT
print "X Y Prob\n";

my @ProbLandscape = ();
my ($i,$j) = (0,0);
for ($i=0;$i<$scale;$i++)
{
	for ($j=0;$j<$scale;$j++)
	{
		$ProbLandscape[$i][$j] = 0.0;
	}
}

for($n=0;$n<$N_States;$n++)
{
	#print "[ @{$StateSpace[$n]} ]\n";
	#DEBUG
	#if ($StateSpace[$n][3] == 0 && $StateSpace[$n][4] == 1)
	#{
	#	my $m = $n+1;
	#	print "$m $SteadyProb[$m]\n";
	#}
	#
	if ($StateSpace[$n][0] < $scale and $StateSpace[$n][1] < $scale)
	{
		$ProbLandscape[$StateSpace[$n][0]][$StateSpace[$n][1]] += $SteadyProb[$n+1];
	}
}

for ($i=0;$i<$scale;$i++)
{
	for ($j=0;$j<$scale;$j++)
	{
		print "$i $j $ProbLandscape[$i][$j]\n";
	}
}



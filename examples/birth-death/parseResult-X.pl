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

$USAGE = "parseResults [arguments] --rs resultfile --ss statespacefile\nDetailed help:\nparseResults --help\n";

my ($resfile,$ssfile,$help) =
   (undef,   undef,  undef);

my $scale = 250;

&GetOptions('rs|r=s'             => \$resfile,
            'ss|s=s'             => \$ssfile,
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
my $line = 0;

$line = <SS>;
chomp($line);
@ary = split (" ", $line);
$N_Species = $ary[0];
$N_States = $ary[1];
$line = <SS>;
chomp($line);
@Species = split (" ", $line);
$line = <RES>;

# BEGIN TO HANDLE IT
print "X Prob\n";

my @ProbLandscape = ();
my ($i,$j) = (0,0);
for ($i=0;$i<$scale;$i++)
{
	$ProbLandscape[$i] = 0.0;
}

my $ares = 0;
for($n=0;$n<$N_States;$n++)
{
	$line = <SS>;
	chomp($line);
	@ary = split (" ", $line);
	$line = <RES>;
	chomp($line);
	$ares = $line;
	$ProbLandscape[$ary[0]] += $ares;
}

for ($i=0;$i<$scale;$i++)
{
	printf ("%d %.8E\n", $i, $ProbLandscape[$i]);
}



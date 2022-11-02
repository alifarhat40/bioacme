#!/usr/bin/perl -w
#
# Youfang Cao
# Shanghai Center for Systems Biomedicine
# 2009-06-01
# 
# Parse the results of steady state probability distribution
#


use File::Glob;
use vars qw($USAGE);

#use strict;
use Getopt::Long;

$USAGE = "parseResults [arguments] --ss [statespacefile] --dir [directory of result files] --sc scale\nDetailed help:\nparseResults --help\n";

my ($resdir,$ssfile,$scale,$help) =
   (undef,   undef, 1000,  undef);

&GetOptions('dr|d=s'             => \$resdir,
            'ss|s=s'             => \$ssfile,
            'sc|c=n'             => \$scale,
            'help|h'             => \$help,
            );

#print "$upcutoff\n$downcutoff\n";

if ($help) {
   exec('perldoc', $0);
   die;
}

if (!defined $ssfile) {
    die($USAGE . "\nPlease specify an input state space filename.\n");
}

if (!defined $resdir) {
    $resdir = "./";
    print "Search all .txt file in current directory.\n";
}

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

print "$N_Species $N_States\n";

my @globfiles = ();
my $resfile;
my $tabfile;
my $tabESfile;
my ($i,$j) = (0,0);

$dir = $resdir."/";

opendir (TXTFILES, $dir) or die "no no no...";
@globfiles = grep {/.txt/} readdir TXTFILES;
close TXTFILES;
#print "@globfiles\n";

for ($n=0; $n<$#globfiles; $n++)
{
	$resfile = $globfiles[$n];
	print "$resfile\n";

	$tabfile = $resfile;
	$tabESfile = $resfile;
	$tabfile =~ s/.txt/.tab/g;
	$tabESfile =~ s/.txt/.tabES/g;

	open RES, "$dir$resfile" or die "Cannot open file $!";
	open TAB, ">$dir$tabfile" or die "Cannot open file $!";
	open TABES, ">$dir$tabESfile" or die "Cannot open file $!";

	$i = 0;
	while (<RES>)
	{
		chop;
		$SteadyProb[$i] = $_;
		$i ++;
	}

	# BEGIN TO HANDLE IT
	print TAB "X Y Prob\n";
	print TABES "NPa Prob\n";

	my $scaleES = $scale;
	my @ProbLandscape = ();
	my @ProbLandscapeES = ();
	for ($i=0;$i<$scale;$i++)
	{
		for ($j=0;$j<$scale;$j++)
		{
			$ProbLandscape[$i][$j] = 0.0;
		}
	}
	for ($i=0;$i<$scaleES;$i++)
	{
		$ProbLandscapeES[$i] = 0.0;
	}

	for($i=0;$i<$N_States;$i++)
	{
		$ProbLandscape[$StateSpace[$i][0]][$StateSpace[$i][1]] += $SteadyProb[$i+1];
		$ProbLandscapeES[$StateSpace[$i][2]] += $SteadyProb[$i+1];
	}

	for ($i=0;$i<$scale;$i++)
	{
		for ($j=0;$j<$scale;$j++)
		{
			print TAB "$i $j $ProbLandscape[$i][$j]\n";
		}
	}
	for ($i=0;$i<$scaleES;$i++)
	{
		print TABES "$i $ProbLandscapeES[$i]\n";
	}

	close TAB;
	close TABES;
	close RES;
}



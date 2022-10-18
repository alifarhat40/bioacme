import os, sys, subprocess

__version__ = "0.1.0"


def _bin_path(program):
    return os.path.join(os.path.dirname(os.path.realpath(__file__)), "bin", program)


def _run(program):
    sys.exit(subprocess.call([_bin_path(program), *sys.argv[1:]]))


def stater_space_scheme20():
    _run("stater_space_scheme20")


def prop_mx_scheme20():
    _run("prop_mx_scheme20")


def ss_prob_dist_scheme20():
    _run("ss_prob_dist_scheme20")


def mxexpo():
    _run("mxexpo")

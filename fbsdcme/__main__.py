import sys
import subprocess

from fbsdcme import _bin_path

def _run(name, args):
    sys.exit(subprocess.call([_bin_path(name), *args]))

def build_state_space_bfs():
    _run("build_state_space_bfs", sys.argv[1:])

def build_state_space_dfs():
    _run("build_state_space_dfs", sys.argv[1:])

def mxexp():
    _run("mxexp", sys.argv[1:])

def net_2_matrix():
    _run("net_2_matrix", sys.argv[1:])

def ssor():
    _run("ssor", sys.argv[1:])

def main():
    name = sys.argv[1]
    args = sys.argv[2:]
    if name in ("build_state_space_bfs", "build_state_space_dfs", "mxexp", "net_2_matrix", "ssor"):
        _run(name, args)
    else:
        raise ValueError("Unknown command: " + name)

if __name__ == "__main__":
    main()

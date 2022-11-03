import subprocess
import sys

from bioacme import (
    _BUILD_STATE_SPACE_BFS,
    _BUILD_STATE_SPACE_DFS,
    _MXEXP,
    _NET2MATRIX,
    _SSOR,
    _bin_path,
)

sanatize_name = lambda name: name.replace("_", "").lower()

FUNC_NAMES = {
    sanatize_name(name): name
    for name in (
        _BUILD_STATE_SPACE_BFS,
        _BUILD_STATE_SPACE_DFS,
        _MXEXP,
        _NET2MATRIX,
        _SSOR,
    )
}


def _run(name, args):
    sys.exit(subprocess.call([_bin_path(name), *args]))


def build_state_space_bfs():
    _run(_BUILD_STATE_SPACE_BFS, sys.argv[1:])


def build_state_space_dfs():
    _run(_BUILD_STATE_SPACE_DFS, sys.argv[1:])


def mxexp():
    _run(_MXEXP, sys.argv[1:])


def net_2_matrix():
    _run(_NET2MATRIX, sys.argv[1:])


def ssor():
    _run(_SSOR, sys.argv[1:])


def main():
    name = sanatize_name(sys.argv[1])
    try:
        sanatized_name = FUNC_NAMES[name]
    except KeyError:
        print(f"Unknown command: {name}", file=sys.stderr)
        exit(1)
    args = sys.argv[2:]
    _run(sanatized_name, args)


if __name__ == "__main__":
    main()

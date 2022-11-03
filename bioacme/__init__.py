import subprocess
from pathlib import Path

__version__ = "0.1.0"

_BUILD_STATE_SPACE_BFS = "buildStateSpace_BFS"
_BUILD_STATE_SPACE_DFS = "buildStateSpace_DFS"
_MXEXP = "mxexp"
_NET2MATRIX = "net2matrix"
_SSOR = "ssor"


def _bin_path(program):
    return Path(__file__).resolve().parent / "bin" / program


def _run(program, *pargs, input=None, **pkwargs):
    args = [_bin_path(program)]
    args.extend(pargs)
    for param, value in pkwargs.items():
        args.extend(("--" + param if len(param) > 1 else "-" + param, str(value)))
    return subprocess.run(
        args, input=input, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE
    )


def build_state_space_bfs(*args, **kwargs):
    return _run(_BUILD_STATE_SPACE_BFS, *args, **kwargs)


def build_state_space_dfs(*args, **kwargs):
    return _run(_BUILD_STATE_SPACE_DFS, *args, **kwargs)


def mxexp(*args, **kwargs):
    return _run(_MXEXP, *args, **kwargs)


def net_2_matrix(*args, **kwargs):
    return _run(_NET2MATRIX, *args, **kwargs)


def ssor(*args, **kwargs):
    return _run(_SSOR, *args, **kwargs)

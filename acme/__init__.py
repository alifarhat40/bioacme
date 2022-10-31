import subprocess
from pathlib import Path

__version__ = "0.1.0"


def _bin_path(program):
    return Path(__file__).resolve().parent / "bin" / program


def _run(program, *pargs, input=None, **pkwargs):
    args = [_bin_path(program)]
    args.extend(pargs)
    for param, value in pkwargs.items():
        args.append("--" + param if len(param) > 1 else "-" + param)
        args.append(str(value))
    return subprocess.run(args, input=input, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)


def build_state_space_bfs(*args, **kwargs):
    return _run("build_state_space_bfs", *args, **kwargs)


def build_state_space_dfs(*args, **kwargs):
    return _run("build_state_space_dfs", *args, **kwargs)


def mxexp(*args, **kwargs):
    return _run("mxexp", *args, **kwargs)


def net_2_matrix(*args, **kwargs):
    return _run("net_2_matrix", *args, **kwargs)

def ssor(*args, **kwargs):
    return _run("ssor", *args, **kwargs)

"""Build script."""

import os
import setuptools
import shutil
import subprocess
from distutils import log as distutils_log
from distutils.util import get_platform
from pathlib import Path
from typing import Any, Dict

__all__ = ("build",)

CMAKE_DEFAULT_EXECUTABLE = "cmake"
"""Default path to CMake executable."""

PROJECT_SOURCE = Path(__file__).resolve().parent

PROJECT_BUILD_SOURCE = PROJECT_SOURCE / "build"


def configure_cmake(*args) -> None:
    """Configure CMake."""
    environment_variable = dict(list(os.environ.items()))
    cmd = [CMAKE_DEFAULT_EXECUTABLE, "-S", str(PROJECT_SOURCE), "-B", "build", *args]
    subprocess.call(cmd, cwd=PROJECT_SOURCE, env=environment_variable)


def run_make(*args) -> None:
    """Run CMake build."""
    env = dict(list(os.environ.items()))
    cmd = [CMAKE_DEFAULT_EXECUTABLE, "--build", "build", *args]
    subprocess.call(cmd, cwd=PROJECT_SOURCE, env=env)


def build(setup_kwargs: Dict[str, Any]) -> None:
    """Build executables."""

    configure_cmake()

    run_make()

    src_dir = PROJECT_BUILD_SOURCE / "bin"
    dest_dir = PROJECT_SOURCE / "bioacme" / "bin"

    # Delete C-extensions copied in previous runs, just in case.
    remove_files(dest_dir, "*")

    # Copy built C-extensions back to the project.
    copy_files(src_dir, dest_dir, "*")

    # Run targeted build separately as poetry does not support target builds yet
    try:
        setuptools.setup(
            **setup_kwargs,
            script_args=["bdist_wheel"],
            options={
                "bdist_wheel": {
                    "plat_name": os.getenv("PP_PYTHON_TARGET", get_platform())
                },
                "egg_info": {"egg_base": "./dist/"},
            },
        )
    except BaseException as e:
        distutils_log.error(f"Failed to create targeted wheel: {e}")


def remove_files(target_dir: Path, pattern: str) -> None:
    """Delete files matched with a glob pattern in a directory tree."""
    for path in target_dir.glob(pattern):
        if path.is_dir():
            shutil.rmtree(path)
        else:
            path.unlink()
        distutils_log.info(f"removed {path}")


def copy_files(src_dir: Path, dest_dir: Path, pattern: str) -> None:
    """Copy files matched with a glob pattern in a directory tree to another."""
    for src in src_dir.glob(pattern):
        dest = dest_dir / src.relative_to(src_dir)
        if src.is_dir():
            # NOTE: inefficient if subdirectories also match to the pattern.
            copy_files(src, dest, "*")
        else:
            dest.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(src, dest)
            distutils_log.info(f"copied {src} to {dest}")


if __name__ == "__main__":
    build({})

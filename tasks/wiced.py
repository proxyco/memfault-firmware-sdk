#
# Copyright (c) 2019-Present Memfault, Inc.
# See License.txt for details
#
import os
from glob import glob

from invoke import Collection, task
from .gdb import gdb_build_cmd


TASKS_DIR = os.path.dirname(__file__)
MEMFAULT_SDK_ROOT = os.path.join(TASKS_DIR, "..")
WICED_ROOT = os.path.join(MEMFAULT_SDK_ROOT, "platforms", "wiced")
WICED_DEMO_APP_ROOT = os.path.join(WICED_ROOT, "memfault_demo_app")
WICED_DEMO_APP_MAKEFILE = os.path.join(WICED_DEMO_APP_ROOT, "Makefile")
WICED_SDK_ROOT = os.path.join(WICED_ROOT, "wiced_sdk")
WICED_SDK_43X_ROOT = os.path.join(WICED_SDK_ROOT, "WICED-SDK-6.2.0", "43xxx_Wi-Fi")
WICED_MAKE = os.path.join(WICED_SDK_43X_ROOT, "make")
WICED_MAKEFILE = os.path.join(WICED_SDK_43X_ROOT, "Makefile")
WICED_GDBINIT = os.path.join(WICED_SDK_43X_ROOT, ".gdbinit")

DEMO_APP_TARGET = "memfault_demo_app-BCM943364WCD1-SDIO-debug"
DEMO_APP_ELF = os.path.join(
    WICED_SDK_43X_ROOT, "build", DEMO_APP_TARGET, "binary", DEMO_APP_TARGET + ".elf"
)


def _wiced_guess_console_port():
    def _wiced_find_console_port():
        usb_paths = glob("/dev/cu.usbserial-*1")
        if usb_paths:
            return usb_paths[0]

        print(
            "Cannot find WICED console /dev/... nor ftdi:// path, please specify it manually using --port"
        )
        exit(1)

    port = _wiced_find_console_port()
    print("No --port specified, using console port {port}".format(port=port))
    return port


def _wiced_make(ctx, *args, **kwargs):
    with ctx.cd(WICED_SDK_43X_ROOT):
        ctx.run("{make} {args}".format(make=WICED_MAKE, args=" ".join(args)), **kwargs)


@task
def wiced_build(ctx):
    """Build WICED demo app"""
    _wiced_make(ctx, DEMO_APP_TARGET)


@task
def wiced_clean(ctx):
    """Clean WICED demo app"""
    _wiced_make(ctx, "clean")


@task
def wiced_flash(ctx):
    """Flashes WICED demo app"""

    # See doc/make_target_examples.txt:
    # "* For the BCM943364WCD1, BCM943438WCD1, and BCM9433634WCD1 platforms you must also add "download_apps" to the end of
    #    the target string to download the WLAN firmare to the external flash":
    _wiced_make(ctx, DEMO_APP_TARGET, "download", "download_apps")


@task
def wiced_debug(ctx):
    """Runs GDB using WICED's debug Makefile target -- this runs OpenOCD inside a GDB shell"""
    _wiced_make(ctx, DEMO_APP_TARGET, "debug", pty=True)


@task
def wiced_gdb(ctx, elf=DEMO_APP_ELF, gdb=3333):
    """Runs GDB, loads the demo app elf and attaches it to openocd"""
    with ctx.cd(WICED_SDK_43X_ROOT):
        # Remove the generated .gdbinit -- running openocd using "shell start" within gdb doesn't seem to work very
        # reliably across board resets for some reason...
        try:
            os.unlink(WICED_GDBINIT)
        except FileNotFoundError:
            pass
        cmd = gdb_build_cmd(None, elf, gdb, reset=False)
        ctx.run(cmd, pty=True)


@task
def wiced_openocd(ctx):
    """Runs openocd"""
    with ctx.cd(WICED_SDK_43X_ROOT):
        ctx.run(
            "./tools/OpenOCD/OSX/openocd-all-brcm-libftdi"
            " -s ./tools/OpenOCD/scripts"
            " -f ./tools/OpenOCD/CYW9WCD1EVAL1.cfg"
            " -f ./tools/OpenOCD/stm32f4x.cfg"
            " -f ./tools/OpenOCD/stm32f4x_gdb_jtag.cfg"
        )


@task
def wiced_console(ctx, port=None):
    """Attach debug console"""
    if port is None:
        port = _wiced_guess_console_port()
    ctx.run("miniterm.py --raw {port} 115200".format(port=port), pty=True)


WICED = Collection("wiced")
WICED.add_task(wiced_console, name="console")
WICED.add_task(wiced_build, name="build")
WICED.add_task(wiced_clean, name="clean")
WICED.add_task(wiced_flash, name="flash")
WICED.add_task(wiced_debug, name="debug")
WICED.add_task(wiced_gdb, name="gdb")
WICED.add_task(wiced_openocd, name="openocd")

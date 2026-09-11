import os
import sys


def get_name():
    return "Haiku"


def can_build():
    try:
        return sys.platform == "haiku" or os.uname().sysname.lower() == "haiku"
    except AttributeError:
        return False


def get_opts():
    from SCons.Variables import BoolVariable, EnumVariable
    return [
        EnumVariable("arch", "CPU architecture", "x86_64", ["x86_64", "x86_32", "arm64", "rv64"]),
        BoolVariable("use_haiku_audio", "Use the Haiku Media Kit audio driver", True),
    ]


def get_flags():
    return {"supported": ["library", "editor"]}


def get_doc_classes():
    return ["EditorExportPlatformHaiku"]


def get_doc_path():
    return "doc_classes"


def configure(env):
    arch = env["arch"]
    if arch == "x86_64":
        env.Append(CCFLAGS=["-m64"])
        env.Append(LINKFLAGS=["-m64"])
    elif arch == "x86_32":
        env.Append(CCFLAGS=["-m32"])
        env.Append(LINKFLAGS=["-m32"])
    elif arch == "rv64":
        env.Append(CCFLAGS=["-march=rv64gc"])

    env.Append(CPPDEFINES=["HAIKU_ENABLED", "UNIX_ENABLED", "OPENGL_ENABLED", "GLES3_ENABLED"])
    env.Append(LIBS=["be", "game", "interface", "media", "network", "translation", "GL"])
    if env["use_haiku_audio"]:
        env.Append(CPPDEFINES=["HAIKU_AUDIO_ENABLED"])

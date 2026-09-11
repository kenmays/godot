# Godot Haiku platform

Native Haiku OS platform layer for Godot 4.7.2.

## Design

- `OS_Haiku` derives from `OS_Unix`.
- `DisplayServerHaiku` owns native Haiku windows.
- `BGLView` supplies the desktop OpenGL context used by the Compatibility renderer.
- Haiku Media Kit is the native audio backend.
- x86_64 is the first supported architecture.

## Build

This directory is intended to be overlaid on a Godot 4.7.2 source checkout. Build with:

    scons platform=haiku arch=x86_64

The repository must contain the corresponding Godot 4.7.2 core sources; this repository currently stores the Haiku platform implementation separately from upstream Godot history.

# Godot Engine — Haiku OS Port

This repository contains the Godot Engine source tree and the native Haiku OS platform port.

## Haiku

The Haiku port lives under `platform/haiku/` and targets x86_64 first. It uses Haiku's native `BApplication`, `BWindow`, `BGLView`, and Media Kit APIs and Godot's Compatibility/OpenGL renderer.

Build on Haiku with:

    scons platform=haiku arch=x86_64

The port is maintained as a native Godot platform implementation; no changes to unrelated platform backends are required.

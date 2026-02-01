# Phantom-Stresser
**Network Traffic Simulator & Performance Testing Tool**

A lightweight, multi-threaded application for generating high-volume UDP traffic to a specified target for network performance testing, load simulation, and stress evaluation purposes.

**Educational / Research Use Only**

This tool is provided strictly for:
- Controlled lab environments
- Self-hosted network testing
- Academic research on network behavior under load
- Authorized performance evaluation of services you own/control

**Important Legal Notice**

Do NOT use this software against any system, network, or service without explicit written permission from the owner/operator. Unauthorized use may violate local, national, and international laws (including but not limited to computer fraud/abuse statutes). The author(s) are not responsible for any misuse.

## Features

- Graphical interface (using raylib)
- Adjustable packet rate (PPS)
- Configurable test duration
- Target IP and port selection
- Basic packet statistics display
- Multi-threaded sending for higher throughput

## Requirements

- Linux / macOS / Windows
- raylib library installed
  - Ubuntu/Debian: `sudo apt install libraylib-dev`
  - macOS (brew): `brew install raylib`
  - Windows: see raylib documentation
- Root/admin privileges (required for high-performance socket options)

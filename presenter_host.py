#!/usr/bin/env python3
"""SerPresenter host — reads serial commands from D1 mini and simulates key presses."""

import argparse
import serial
import serial.tools.list_ports
import subprocess
import sys
import time

VID = 0x1A86   # QinHeng (CH340)
PID = 0x7523   # CH340


def find_port():
    """Auto-detect the D1 mini serial port by USB VID/PID."""
    for port in serial.tools.list_ports.comports():
        if port.vid == VID and port.pid == PID:
            return port.device
    return None


def check_xdotool():
    """Verify xdotool is available."""
    try:
        subprocess.run(["xdotool", "--version"], capture_output=True, check=True)
        return True
    except (FileNotFoundError, subprocess.CalledProcessError):
        return False


def main():
    parser = argparse.ArgumentParser(description="SerPresenter Host")
    parser.add_argument(
        "-p", "--port",
        help="Serial port (auto-detected if omitted)",
    )
    parser.add_argument(
        "-b", "--baudrate",
        type=int, default=115200,
        help="Baud rate (default: 115200)",
    )
    parser.add_argument(
        "-l", "--list",
        action="store_true",
        help="List available serial ports and exit",
    )
    parser.add_argument(
        "-n", "--dry-run",
        action="store_true",
        help="Don't simulate keys, just print received commands",
    )
    args = parser.parse_args()

    if args.list:
        print("Available serial ports:")
        for port in serial.tools.list_ports.comports():
            extra = ""
            if port.vid == VID and port.pid == PID:
                extra = "  <-- D1 mini (CH340)"
            print(f"  {port.device}  {port.description}{extra}")
        return

    if not args.dry_run and not check_xdotool():
        print("ERROR: xdotool not found. Install it with:")
        print("  sudo apt install xdotool")
        print("Or run with --dry-run to test without key simulation.")
        sys.exit(1)

    port = args.port or find_port()
    if not port:
        print("ERROR: D1 mini not found. Plug it in and try again,")
        print("or specify --port /dev/ttyUSB0")
        print("Use --list to see available ports.")
        sys.exit(1)

    print(f"Opening {port} at {args.baudrate} baud...")
    ser = serial.Serial(port, args.baudrate, timeout=1)
    print("Ready. Waiting for commands...")
    print("  (Press Ctrl+C to exit)")

    try:
        while True:
            line = ser.readline().decode("utf-8").strip()
            if not line:
                continue

            if line == "NEXT":
                print("→ next")
                if not args.dry_run:
                    subprocess.run(["xdotool", "key", "Right"], check=True)
            elif line == "PREV":
                print("← prev")
                if not args.dry_run:
                    subprocess.run(["xdotool", "key", "Left"], check=True)
            else:
                print(f"  unknown: {line}")

    except KeyboardInterrupt:
        print("\nDone.")
    finally:
        ser.close()


if __name__ == "__main__":
    main()

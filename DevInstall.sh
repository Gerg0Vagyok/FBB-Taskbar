#!/usr/bin/env bash
# Development asset install script

if [ "$(id -u)" -ne 0 ]; then
	echo "Must be ran with root prevliges!"
	exit 1
fi

mkdir -p /usr/local/share/fbb-taskbar
cp assets/MissingIcon.png /usr/local/share/fbb-taskbar/MissingIcon.png

echo "Finished!"

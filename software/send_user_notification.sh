#!/bin/bash
# Helper script to send notifications from the user's session context

# Source user environment first
export HOME="/home/pi"
export USER="pi"
export XDG_RUNTIME_DIR="/run/user/1000"
export WAYLAND_DISPLAY="wayland-0"
export XDG_SESSION_TYPE="wayland"
export XDG_CURRENT_DESKTOP="labwc:wlroots"
export DBUS_SESSION_BUS_ADDRESS="unix:path=/run/user/1000/bus"

# Now send the notification
if [ "$1" = "critical" ]; then
    wfpanelctl critical "$2"
else
    wfpanelctl notify "$2"
fi
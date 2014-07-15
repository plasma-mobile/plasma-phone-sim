plasma-phone-sim
================

Device simulator to make developing Plasma Phone components easier on laptop/desktop systems

plasma-phone-sim [--qml-package] [--shell-package] [--dev-res WIDTHxHEIGHT[xDPI]]

== Roadmap

=== Version 0.1
Release Target: July 2014

This version will be a simple single-window application based on Qt5/KF5 that loads a single
QML Plasma::Package or a Plasma Shell Package (also a Plasma::Package type).

=== Version 0.2
Release Target: September 2014

Addition of system emulation controls (network available, e.g.) will be added on an
as-needed basis driven by the needs of the Plasma Phone project in the first six month cycle.

=== Version 0.3
Release Target: ???

With completion of the first full OS image (MerOS based?) near the end of the 2014, this
device simulator will be built around a VM image of that OS with either the sim application
running in it or plasma-shell from Plasma 5. As this is still a fair ways off in the future
and the immediate utility of plasma-shell has not been fully vetted, these decisions are
waylayed until then

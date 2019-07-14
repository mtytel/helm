#!/bin/bash
sudo dnf install jack-audio-connection-kit-devel libcurl-devel
make standalone
sudo make install_standalone
#!/usr/bin/python

import os, sys, subprocess

UBUNTU = [
	#'libsdl2-dev',
	'libsdl2-2.0-0',
]

FEDORA = [
	'jack-audio-connection-kit-devel',
	'libcurl-devel',
]

subprocess.check_call(['make', 'DESTDIR=/tmp/helm', 'install_standalone'])

cmd = [
	'fpm', '-s', 'dir', '-C', '/tmp/helm',
	'--name', 'helm', '--version', '0.9.1', '--iteration', '1',
	'--description', 'helm midi synth'
]

if '--ubuntu' in sys.argv or 'ubuntu' in sys.argv:
	cmd.extend(['-t', 'deb'])
	for dep in UBUNTU:
		cmd.extend(['--depends', dep])
elif '--fedora' in sys.argv or 'fedora' in sys.argv:
	cmd.extend(['-t', 'rpm'])
	for dep in FEDORA:
		cmd.extend(['--depends', dep])
else:
	raise RuntimeError('you must give an output type: --feodra or --ubuntu')

print(cmd)

subprocess.check_call(cmd)
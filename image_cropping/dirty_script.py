import subprocess
import os

diroot = os.getcwd()

for root, dir, files in os.walk(diroot):
	continue

imfiles = [file for file in files if file.endswith(".jpg")]

for im in imfiles:
	# Program input of form [program, file, threshold]
	# Feel free to change threshold to taste
	args = ['./mesurf', im, '3000']
	p = subprocess.Popen(args)
	p.wait()

	try:
		p.kill()
	except OSError:
		pass

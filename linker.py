#!/usr/bin/env python3
import subprocess
import numpy
import random
import time
import tkinter as bitch
import threading
import os

VALUES: int = 7360;
INIT: float = 0.1;
PATH: str = "./nets/best.net";
BATCHSIZE: int = 7;
ITERATIONS: int = 8;
RNG: int = 0;

def Run():
	for C1 in range(ITERATIONS):
		Processes: list[subprocess.Popen] = [];
		for C2 in range(BATCHSIZE):
			Seed: int = int.from_bytes(os.urandom(2), 'little');
			Index: int = (BATCHSIZE * C1) + C2;
			Mutating: str = "n";
			if (Index > 2):
				Mutating = "y";
			Process: subprocess.Popen = subprocess.Popen([
				"./yield/arson",
				str(Index),
				Mutating,
				"n",
				str(RNG),
				str(Seed)
			]);
			Processes.append(Process);
		for C2 in range(BATCHSIZE):
			Processes[C2].wait();
	Victor: numpy.ndarray = numpy.zeros(VALUES, numpy.float32);
	Record: float = 0;
	for C1 in range(ITERATIONS * BATCHSIZE):
		Score: float = float(numpy.fromfile(f"./nets/score_{C1}.net", dtype=numpy.float32, count=1)[0]);
		if (Score > Record):
			Record = Score;
			Victor = numpy.fromfile(f"./nets/candidate_{C1}.net", numpy.float32, count=VALUES);
	print(f"Victor score: {Record}")
	Victor.astype(numpy.float32).tofile(PATH);

while True:
	RNG = random.randint(0, 32767);
	Network: numpy.ndarray = numpy.zeros(VALUES, numpy.float32);
	try:
		Network = numpy.fromfile(PATH, numpy.float32, count=VALUES);
	except FileNotFoundError:
		for C1 in range(VALUES):
			Network[C1] = numpy.float32(random.uniform(-INIT, INIT));
		Network.astype(numpy.float32).tofile(PATH);
	Display: bitch.Tk = bitch.Tk();
	Display.title("cmd panel");
	Subdisplay: bitch.Canvas = bitch.Canvas(Display, width="1160", height="650");
	for C1 in range(115):
		for C2 in range(64):
			X: int = (C1 * 10) + 10;
			Y: int = (C2 * 10) + 10;
			Radius: int = abs(float(Network[C1 * 64 + C2])) * 4;
			Subdisplay.create_oval(X - Radius, Y - Radius, X + Radius, Y + Radius, fill="black");
	Subdisplay.pack();
	Thread: threading.Thread = threading.Thread(target=Run);
	Thread.start();
	while Thread.is_alive():
		Display.update();
		time.sleep(1);
	Display.destroy();
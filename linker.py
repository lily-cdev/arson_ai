#!/usr/bin/env python3
import os
import random
import tkinter
NONSENSOR_INPUTS: int = 9;
SENSORS: int = 32;
SENSORINPUTS: int = 2;
NODE_CAP: int = 500;

class Link:
	Destination: int;
	Weight: float;
	def __init__(self):
		self.Destination: int = 0;
		self.Weight: float = 0;

class Node:
	ID: int;
	Layer: int;
	Links: list[Link];
	def __init__(self):
		self.ID: int = 0;
		self.Layer: int = 0;
		self.Links: list[Link] = [];

class Net:
	Layers: int;
	Nodes: list[Node];
	def __init__(self):
		self.Layers: int = 0;
		self.Nodes: list[Node] = [];

class Pos:
	X: int;
	Y: int;
	def __init__(self):
		self.X: int = 0;
		self.Y: int = 0;

def Init_Net():
	Yield: Net = Net();
	Inputs: int = (SENSORS * SENSORINPUTS) + NONSENSOR_INPUTS;
	Outputs: int = 10; #// TMP!!!
	Yield.Layers = 3;
	for C1 in range(Inputs):
		Carrier: Node = Node();
		Carrier.ID = C1 + 1;
		Carrier.Layer = 0;
		Yield.Nodes.append(Carrier);
	for C1 in range(Outputs):
		Carrier: Node = Node();
		Carrier.ID = Inputs + C1 + 1;
		Carrier.Layer = 2;
		Yield.Nodes.append(Carrier);
	for C1 in range(2):
		Carrier: Node = Node();
		Carrier.ID = Inputs + Outputs + C1 + 1;
		Carrier.Layer = 1;
		Yield.Nodes.append(Carrier);
	return Yield;

def Connect_Net(Input: Net):
	for C1 in range(len(Input.Nodes)):
		for C2 in range(len(Input.Nodes)):
			if Input.Nodes[C1].Layer + 1 == Input.Nodes[C2].Layer:
				Sublink: Link = Link();
				Sublink.Destination = Input.Nodes[C2].ID;
				Sublink.Weight = random.uniform(-0.1, 0.1);
				Input.Nodes[C1].Links.append(Sublink);

Nodes: list[Node] = [];
Network: Net;
try:
	Best = open("./nets/best.net", "rb");
	Best.close();
except:
	Network = Init_Net();
	Connect_Net(Network);
Display: tkinter.Tk = tkinter.Tk();
Display.title("cmd panel");
Subdisplay: tkinter.Canvas = tkinter.Canvas(Display, width="1080", height="690");
Offset: int = 6;
Positions: list[int] = [0] * Network.Layers;
Nodepos: list[Pos] = [Pos()] * (len(Network.Nodes) + 1);
for C1 in range(len(Network.Nodes)):
	X: int = Positions[Network.Nodes[C1].Layer] * 14 + Offset;
	Y: int = Network.Nodes[C1].Layer * 150 + Offset;
	Subdisplay.create_oval(X, Y, X + 6, Y + 6);
	Positions[Network.Nodes[C1].Layer] += 1;
	Subpos: Pos = Pos();
	Subpos.X = X + 3;
	Subpos.Y = Y + 3;
	Nodepos[Network.Nodes[C1].ID] = Subpos;
for C1 in range(len(Network.Nodes)):
	for C2 in range(len(Network.Nodes[C1].Links)):
		for C3 in range(len(Network.Nodes)):
			if Network.Nodes[C1].Links[C2].Destination == Network.Nodes[C3].ID:
				Subdisplay.create_line(Nodepos[Network.Nodes[C3].ID].X, Nodepos[Network.Nodes[C3].ID].Y,
					Nodepos[Network.Nodes[C1].ID].X, Nodepos[Network.Nodes[C1].ID].Y, width=0.5)
Subdisplay.pack();
Display.mainloop();
#os.system("seq 4 | xargs -P 4 -I {} ./yield/arson");
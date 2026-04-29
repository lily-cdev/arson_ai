#!/usr/bin/env python3
import os
import random
import tkinter

INPUTS = 73
LAYER1 = 64
LAYER2 = 32
LAYER3 = 16
OUTPUTS = 8

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
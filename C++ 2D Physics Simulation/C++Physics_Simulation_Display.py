import tkinter as tk
import time
import sys

maxRecursion = sys.getrecursionlimit()

zoom = 16

file = open("output.txt", "r")

silly = file.read()

partList = silly.splitlines()

temp = []

for i in range(len(partList)):
    temp.append(eval(partList[i]))

partList = temp

root = tk.Tk()

canvas = tk.Canvas(root, width=512, height=512)
canvas.pack()

def update(i, recursionDepth=0):
    
    if(i >= len(partList[0]) or recursionDepth >= maxRecursion-6):
        return
    
    canvas.delete("all");
    
    for j in partList:
        x = j[i][0] * zoom + canvas.winfo_reqwidth() / 2;
        y = -j[i][1] * zoom + canvas.winfo_reqheight() / 2;
        size = j[0][0] * zoom
        canvas.create_oval(x-size,y-size,x+size,y+size,fill="red",outline="black")
    
    canvas.update_idletasks()
    time.sleep(0.01)#set this to Particle::timeStep in particle.h
    update(i+1, recursionDepth+1)

update(1)
update(1,maxRecursion-6)
root.mainloop()

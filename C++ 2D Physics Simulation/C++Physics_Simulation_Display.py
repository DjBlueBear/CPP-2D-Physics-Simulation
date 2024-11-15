import tkinter as tk
import random
import time

zoom = 6

file = open("output.txt", "r")

silly = file.read()

partList = silly.splitlines()

temp = []

for i in range(len(partList)):
    temp.append(eval(partList[i]))

partList = temp

root = tk.Tk()



canvas = tk.Canvas(root, width=1000, height=1000)
canvas.pack()

def update(i):
    
    if(i >= len(partList[0])):
        return
    
    canvas.delete("all");
    
    for j in partList:
        x = j[i][0] * zoom + 256
        y = j[i][1] * zoom + 256
        size = j[0][0] * zoom
        if(j[0][0] < 1):
            canvas.create_oval(x-size,y-size,x+size,y+size,fill="black",outline="pink")
        else:
            canvas.create_oval(x-size,y-size,x+size,y+size,fill="red")
    canvas.update_idletasks()
    time.sleep(0.01)
    update(i+1)

update(1)

root.mainloop()
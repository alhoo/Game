import torch
from torch.autograd import Variable
#dtype = torch.cuda.FloatTensor
#%s/numpy/cpu().numpy/g
#%s/cpu().numpy/numpy/g
dtype = torch.FloatTensor

"""
Here we calculate the distance to everything the map offers. Then we use an
exotic mean-type of function together with the demand-values to find which
areas of the map are most attractive.

We define happiness as the sum of supply attractions, so that we are more 
happy the closer we are to what themap offers for our needs. To find out were
we should go, we calculate the gradient with regards to our current position
and then we move that way.
"""
powermean = lambda arr, k: (arr).pow(k).sum(0)/(arr).pow(k - 1).sum(0)

def updateDemand(demand, supplydistances, verbose=False):
  """
  To make the actor more complete, we should look into making the actor
  satisfy its needs when it is close enought to the supplies. So when the actor
  is close to water and it is thirsty, after spending a moment there the
  thirsty-value starts to go to 0.
  """
  v, idxs = supplydistances.min(0)
  v, jdx = v.min(0)
  if v.lt(0.25).data.numpy()[0]:
    idx = idxs[jdx]
    if verbose:
      print(jdx.data.numpy())
    demand[jdx] -= 0.1
  demand += 0.01
  demand *= 0.999
  return demand

def simulate():
  """
  food = 10*torch.randn(10, 2).type(dtype)
  water = 10*torch.randn(10, 2).type(dtype)
  enemies = 10*torch.randn(10, 2).type(dtype)
  shelter = 10*torch.randn(10, 2).type(dtype)
  emptySpace = 10*torch.randn(10, 2).type(dtype)
  materials = 10*torch.randn(10, 2).type(dtype)
  people = 10*torch.randn(10, 2).type(dtype)
  culture = 10*torch.randn(10, 2).type(dtype)

  supply = np.array([food, water, enemies, shelter, materials, emptySpace, people, culture])
  """
  supply0 = 10*torch.randn(8, 3, 2).type(dtype)
  pos0 = torch.Tensor([0,0]).type(dtype)
  pos = Variable(pos0, requires_grad=True)
  supply = Variable(supply0, requires_grad=False)


  hunger = 0.01
  thirst = 0.01
  peace = 0.51
  heat = 0.2
  activity = 0.01
  space = 0.01
  friends = 0.21
  knowledge = 0.3
#demand = np.array([hunger, thirst, peace, heat, activity, space, friends, knowledge])
  demand0 = torch.Tensor([hunger, thirst, peace, heat, activity, space, friends, knowledge]).type(dtype)
  demand = Variable(demand0, requires_grad=False)
  while True:
    supplydistances = (supply - pos).pow(2).sum(2).t().sqrt()
    supplyattraction = demand*powermean(1/(1+supplydistances), 4)
    happiness = supplyattraction.sum()
    happiness.backward(retain_graph=True)
    move = pos.grad.data/pos.grad.data.norm()/10 #Normalize move distance
    pos.data += move
    null = pos.grad.data.zero_()
    demand = updateDemand(demand, supplydistances)
    yield (pos.data.numpy(), supply.data.numpy())

"""
Next we should reduce the actors knowledge by hiding everything that are not
in straight line of sight and what the actor doesn't remember. Then we should
generalize the above code to run on multiple actors. We should also reduce
calculations by disregarding everything outside some bounding box.
"""

import pygame
from math import sin, cos
from pygame.locals import *
from OpenGL.GL import *
from OpenGL.GLU import *
import numpy as np

def drawDots(data):
    """
    Draw dots that are arranged in the data input
    """
    #The resources are in data[1]
    for i in range(len(data[1])):
      glColor((sin(i),i/10,1-i/10))
      glBegin(GL_POINTS)
      for p in data[1][i]:
        glVertex3f(p[0], p[1], 0);
      glEnd()
    #The actor is in data[0]
    glColor((1,0,0))
    glBegin(GL_POINTS)
    glVertex3f(data[0][0], data[0][1], 0);
    glEnd()

def main():
  """
  Run the simulation and display the data on the screen
  """
  pygame.init()
  display = (800,600)
  pygame.display.set_mode(display, DOUBLEBUF|OPENGL)
  gluPerspective(45, (display[0]/display[1]), 0.1, 100.0)
  glTranslatef(0.0,0.0, -70)
  glPointSize(5.0)

  frame=0
  for data in simulate():
    for event in pygame.event.get():
      if event.type == pygame.QUIT:
        pygame.quit()
        quit()
      if event.type == KEYUP and event.key == K_ESCAPE:
        return                
    
    #glRotatef(1, 3, 1, 1)
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
    glMatrixMode(GL_MODELVIEW)

    frame = frame + 1
    drawDots(data)

    pygame.display.flip()
    pygame.time.wait(10)


if __name__ == '__main__':
  main()

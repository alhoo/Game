import torch
from torch.autograd import Variable
import numpy as np
dtype = torch.cuda.FloatTensor
#%s/numpy/cpu().cpu().numpy/g
#%s/cpu().cpu().numpy/numpy/g
#dtype = torch.FloatTensor
import torch.cuda as cutorch

"""
Here we calculate the distance to everything the map offers. Then we find which
areas of the map are most attractive.

We define happiness as the sum of supply attractions, so that we are more 
happy the closer we are to what themap offers for our needs. To find out were
we should go, we calculate the gradient with regards to our current position
and then we move that way.
"""

def updateDemandN(demand, supplydistances, verbose=False):
  """
  To make the actor more complete, we should look into making the actor
  satisfy its needs when it is close enought to the supplies. So when the actor
  is close to water and it is thirsty, after spending a moment there the
  thirsty-value starts to go to 0.
  """
  return 0.999*(demand - (0.1*supplydistances.lt(0.25).sum(2).type(dtype) - 0.01))

def simulateN(N=10000):
  """
  supply = np.array([food, water, enemies, shelter, materials, emptySpace, people, culture])
  demand = np.array([hunger, thirst, peace, heat, activity, space, friends, knowledge])
  """
  supply0dim = (8,5,2)
  shape1 = int(np.multiply(*supply0dim[:-1]))

  supply0 = 10*torch.randn(*supply0dim).type(dtype)
  demand0 = torch.randn(N, 8).type(dtype)
  pos0 = 10*torch.randn(N, 2).type(dtype)

  supply = Variable(supply0, requires_grad=False)
  demand = Variable(demand0, requires_grad=False)
  pos = Variable(pos0, requires_grad=True)
  
  n=0
  while True:
    supplydistances = (supply.repeat(N,1,1,1).view(N,-1,2) - \
                       pos.repeat(1,shape1).view(N,-1,2))\
                       .pow(2).sum(2).sqrt()
    # demand is detached to fix a bug with growing computation costs
    supplyattraction = demand.detach().view(N,supply0dim[0],1)\
                             .expand(N,supply0dim[0],supply0dim[1])\
                             *(1/supplydistances.view(N,supply0dim[0],supply0dim[1]).pow(1.5))
                             #*torch.exp(-supplydistances.view(N,supply0dim[0],supply0dim[1]))
    happiness = supplyattraction.sum()
    happiness.backward()
    move = (pos.grad.data.t() / pos.grad.data.pow(2).sum(1).sqrt()/10).t() #Normalize move distance
    pos.data += move
    pos.grad.data.zero_()
    demand = updateDemandN(demand, supplydistances.view(N, -1, supply0dim[1]))
    yield (pos.data.cpu().numpy(), supply.data.cpu().numpy())
    n = n + 1


"""
Next we should reduce the actors knowledge by hiding everything that are not
in straight line of sight and what the actor doesn't remember. We should also
reduce calculations by disregarding everything outside some bounding box.
"""

import time
import pygame
from math import sin, cos

from pygame.locals import *

from OpenGL.GL import *
from OpenGL.GLU import *

from ctypes import *
import numpy as np

def drawDots(data, vbo):
    """
    Draw dots that are arranged in the data input
    """
    # The actors is in data[0]
    glPointSize(1.0)
    vertices = (data[0].reshape(-1))
    glBufferData (GL_ARRAY_BUFFER,
                  4*len(vertices),
                  (c_float*len(vertices))(*vertices),
                  GL_STATIC_DRAW)
    
    glVertexPointer (2, GL_FLOAT, 0, None)
    glColor((1,0,0))
    glDrawArrays (GL_POINTS, 0, len(data[0]))
    # The resources are in data[1]
    glPointSize(5.0)
    for i in range(len(data[1])):
      glColor((sin(i),i/10,1-i/10))
      glBegin(GL_POINTS)
      for p in data[1][i]:
        glVertex3f(p[0], p[1], 0);
      glEnd()
    # Draw 10 people white and above the resources
    glPointSize(3.0)
    glColor((1,1,1))
    for i in range(min(len(data[0]), max(4,len(data[0]) - 10)), len(data[0])):
      glBegin(GL_POINTS)
      glVertex3f(data[0][i][0], data[0][i][1], 0);
      glEnd()

def main():
  import sys
  """
  Run the simulation and display the data on the screen
  """
  pygame.init()
  display = (800,600)
  pygame.display.set_mode(display, DOUBLEBUF|OPENGL)
  gluPerspective(45, (display[0]/display[1]), 0.1, 100.0)
  glTranslatef(0.0,0.0, -70)
  glPointSize(1.0)
  glEnableClientState (GL_VERTEX_ARRAY)

  vbo = glGenBuffers (1)
  glBindBuffer (GL_ARRAY_BUFFER, vbo)
  N=100
  if len(sys.argv)>1:
    N=int(sys.argv[1])
  
  glBindBuffer (GL_ARRAY_BUFFER, vbo)

  t0 = 1000*time.time()
  t4p = t0

  frame=0
  for data in simulateN(N):
    t1 = 1000*time.time()
    for event in pygame.event.get():
      if (event.type == KEYUP and event.key == K_ESCAPE) or \
          event.type == pygame.QUIT:
        pygame.quit()
        quit()
    
    #glRotatef(1, 3, 1, 1)
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
    glMatrixMode(GL_MODELVIEW)

    frame = frame + 1
    t2 = 1000*time.time()
    drawDots(data, vbo)
    t3 = 1000*time.time()

    pygame.display.flip()
    t4 = 1000*time.time()
    if frame % 30 == 0:
      sys.stdout.write(
             "simulation: %.1fms inputs: %.1fms draw: %.1fms flipscreen: %.1fms fps: %.1f\r" % (
             (t1 - t4p),
             (t2 - t1),
             (t3 - t2),
             (t4 - t3),
             1000/(t4 - t4p)))
    t4p = t4


if __name__ == '__main__':
  main()

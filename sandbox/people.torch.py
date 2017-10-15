import torch
dtype = torch.FloatTensor
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
from torch.autograd import Variable
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

for i in range(10000):
  supplydistances = (supply - pos).pow(2).sum(2).t().sqrt()
  supplyattraction = demand*powermean(1/(1+supplydistances), 4)
  happiness = supplyattraction.sum()
  happiness.backward(retain_graph=True)
  pos.data += pos.grad.data
  null = pos.grad.data.zero_()
  demand = updateDemand(demand, supplydistances)
  print(pos.data.numpy())

"""
Next we should reduce the actors knowledge by hiding everything that are not
in straight line of sight and what the actor doesn't remember. Then we should
generalize the above code to run on multiple actors. We should also reduce
calculations by disregarding everything outside some bounding box.

To make the actor more complete, we should look into making the actor satisfie
its needs when it is close enought to the supplies. So when the actor is close
to water and it is thirsty, after spending a moment there the thirsty-value
starts to go to 0.
"""

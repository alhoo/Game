import numpy as np

"""
 oon yrittäny tehdä tohon peliin semmosen people-matriisin, jossa olis

P = [joonas, joni, lasse, ... ukkeli_(N - 1), ukkeli_N]
07:31

 ja jokainen ukkeli olis siis vektori, joka olis esim: [x, y, vx, vy, going_to_x, going_to_y, energy, ..., alertness, happiness, food_knowledge, terrain_knowledge, ..., fame]
07:34

 ja joka ruutupäivityksellä päivitetään vähintään tota sijaintia ja luetaan se ja piirretään tohon ruutuun
07:34
"""

A = np.matrix(
  [[1,0,1,0,0],
   [0,1,0,1,0],
   [0,0,1,0,0],
   [0,0,0,1,0],
   [0,0,0,0,1]])
I = np.matrix(
  [[1,0,0,0,0],
   [0,1,0,0,0],
   [0,0,1,0,0],
   [0,0,0,1,0],
   [0,0,0,0,1]])
B = np.matrix(
  [[1,0,0,0,0],
   [0,1,0,0,0],
   [0,0,1,0,1],
   [0,0,0,1,1],
   [0,0,0,0,1]])
P = np.matrix(
  [[1, 3, 5, 7],
   [4, 1, 7, 4],
   [1,-1, 0, 0],
   [0, 0,-1, 1],
   [0, 1, 0, 1]])
F1 = np.matrix(
  [[1, 0, 0, 0, 0],
   [0, 1, 0, 0, 0],
   [0, 0, 0, 0, 0],
   [0, 0, 0, 0, 0],
   [0, 0, 0, 0, 0]])

"""
 kehitin nyt tohon matriisilaskentaan "uuden" operaation, joka on vähän ku matriisitulo, mut toimii silleen potensseilla ja kerto operaatioilla, ku normaali matriisitulo on kerto ja plus operaatioilla.
07:36

 sen avulla saa vähän kätevämmin rakennettua tota logiikkaa miten toi liikkumis-suunta tulee lopulta kaikesta tosta muusta tiedosta
07:37
"""
def muldot(A, B):
  ret = B.copy()
  for i in range(B.shape[0]):
    for j in range(B.shape[1]):
      bij = 1
      for k in range(A.shape[0]):
        bij = bij*np.power(B[k,j], A[i,k])
      ret[i,j] = bij
  return ret

for i in range(4):
  P = F1.dot(A.dot(muldot(B, P))) + (I - F1).dot(P)
  print()
  print(P)

"""
 mun pitää ehkä vielä tehdä joku toinen 3-ulotteinen matriisi, jossa olis noiden ukkeleiden tietämys siitä kartasta, eli jos ukkeli liikkuu jossain missä se tai kukaan sen tuttu ei oo koskaan ennen liikkunu niin sen food_knowledge ja terrain_knowledge menee lähelle 0 ja jos on "kotona" niin noi menee melkein yhteen
07:39

 ja pitää keksiä miten saan ton mun matriisipotenssin suoritettua näytönohjaimella ton viennaCL:n läpi
07:40

 voi olla että sen sais muutettua logaritmilla tavalliseks matriisituloks, mut vähän hankala ku log(0) = undefined ja jos matriiseissa on joku arvo 0 niin hajoo
07:42

 mut ehkä kaikkeen voi lisätä pienen epsilonin niin tulee sit log(epsilon) = -vitusti
07:42

 tai ainakin -20
07:42

 salee sen takia ihminenkään ei pysty olee täysin paikallaan
07:44

 On kyl siisti rakennella tommosta simulaatioo
07:59

 vaik ei kauheen montaa tuntia viikossa ehdikään
08:00


HUOM:
log(0) lisäksi ongelma on myös kaikki negatiiviset luvut. Onneksi
log(abs(x) + eps) on aina määritelty ja sign(x) kertoo oliko kyseessä
negatiivinen. Voidaan siis määritellä että

A_ij = pow(b_kj, a_ik), kaikilla i, j
A_ij = sign(b_ik)*pow(abs(b_kj), a_ik), kaikilla i, j, kun a_ik % 2 != 0
=> A_ij = sign(b_kj)*exp(sum(a_ik*log(b_kj)))


"""
def muldot2(A, B):
  """
  ret = B
  ret = np.log(abs(ret) + eps)
  ret = A.dot(ret)
  ret = np.exp(ret)
  ret = np.multiply(np.sign(B + eps), ret)
  return ret
  """
  eps = 10e-10
  return np.multiply(np.sign(B),
         np.exp(A.dot(np.log(abs(B) + eps))))


"""
 jos hahmotan oikein niin tosta kartasta ja siinä olevista mineraaleista / muista tyypeistä / esteistä tulee semmosia heighmappeja, josta pystyy laskemaan gradientin mikä kertoo mihin suuntaan noiden ukkeleiden pitää liikkua että niiden hyvä olo kasvaa kaikkeista eniten. Hyvä olo sit riippuu siitä että onko niillä tarpeeks ruokaa, pelottaako niitä, onko ne uteliaita vai mitä
09:59

 eli lasken vaan mihin suuntaan kartalla tommosen yhden ukkelin tilavektori muuttuu parhaiten siten että joku tommonen staattinen hyvä mieli -funktio kasvaa eniten
09:59

 ja sit asetan tilavektorin silleen että se ukkeli liikkuu siihen suuntaan
10:00

 ja ukkelin knowledge kertoo mikä on kohinan ja maailman tiedon suhde tossa laskussa
10:02

 eli jos ukkelilla ei oo knowledgee niin se kun laskee miten sen onnellisuus kasvais nii siinä osa tiedoista perustuu vaan kohinaan
10:03

 eli tekee vähän randomisti päätöksiä
10:03

 sit jos jollain on taas hyvä knowledge siinä paikassa missä se on niin toi kohina on melkein 0 ja se löytää suoraan mistä saa vettä tai ruokaa tai mistä löytyy vaarat.
10:04
"""

#powmean = lambda arr, k: reduce(lambda x, y: y + x, map(lambda x: pow(x, k), arr))/len(arr)
powmean = lambda arr, k=4: np.average(np.power(np.array(arr), k))
powermean = lambda arr, k=4: powmean(arr, k)/powmean(arr, k-1)
powermean((1,2,3))

minerals = np.array([[2,3], [7,5], [4, 4], [0, 9], [7,1]])
me = np.array([1,3])

"""
Not like this

dist2 = lambda a, b: a.dot(b)
powermean(minerals.dot(me))
"""
dists = np.linalg.norm((minerals - me), axis=1)
want = powermean(1/(1 + np.linalg.norm(minerals - (0,9), axis=1)))

"""
Now lets do it better with theano
"""

import numpy as np
import theano
import theano.tensor as T
from theano import pp

# Define variables for making calculations
x = T.dvector('human')
m = T.dmatrix('minerals')

# Define where the minerals and the human is
minerals = np.array([[2,3], [7,5], [4, 4], [0, 9], [7,1]])
me = np.array([1,3])

# Define a average function that emphasizes the largest values
Tpowmean = lambda x, k: T.mean(T.power(x, k))
Tpowermean = lambda x, k: Tpowmean(x, k)/Tpowmean(x, k-1)

# Distances to all the minerals
mineraldistances = (m - x).norm(2, axis=1)
"""
Define a function which indicates how close we are to a mineral as the 
Nth power if the inverse of 1 plus distance
"""
mineralcloseness = Tpowermean(1/(1 + mineraldistances), 4)

# Go to the direction where the closeness to minerals increase
godirection = T.jacobian(mineralcloseness, x)
# Scale the go direction somewhat arbitrarily
movefunc = theano.function([x,m], godirection / mineralcloseness)


def findClosest(p0, minerals, speed=1, k=2):
  """Finds the closest mineral starting from current position

  Find the closest minerals using gradient descent styled movement.
  This function is made just for testing purposes and a better function
  should be used instead for real usage

  Args:
    p0 (vector): starting position vector
    minerals (matrix): position of minerals
    speed (float): how fast we should be moving
    k (float): speed scaling factor

  Returns:
    vector where the search ended
  """

  pos = p0
  for i in range(1000):
    dpos = movefunc(pos, minerals)
    dposnorm = np.linalg.norm(dpos)
    if dposnorm > 1:
      break
    pos = pos + dpos*speed/np.power(dposnorm, k)
    print(np.round(pos, 1))
  return pos

# Try out the searching function
findClosest((0, 0), minerals, 0.065, 3)

"""
Tuohon pystyy nyt hyvin lisäämään kaikki kartan esteet ja viholliset ja eri tyyppiset mineraalit ja sit lisätä noi ukkelin tietämykset, pelot ja uteliaisuuden niin sen saa liikkumaan järkevästi tuolla
19:17
"""

hardterrain = np.array([[2,3.5], [2.5,3], [2.5, 3.5], [0.5, 7], [7,3]])
enemies = np.array([[5,4], [5,3], [7,3]])
food = np.array([[1,1], [5,4], [3,6], [7,6]])

"""
Tohon on helppo varmaan rakentaa joku neuroverkko vielä päälle ku tolla theanolla on helppo rakennella niitäkin.
19:19
"""

from random import random
randomposition = lambda: (10*random(), 10*random())

food = np.array([randomposition() for i in range(10)])
water = np.array([randomposition() for i in range(10)])
enemies = np.array([randomposition() for i in range(10)])
shelter = np.array([randomposition() for i in range(10)])
emptySpace = np.array([randomposition() for i in range(10)])
materials = np.array([randomposition() for i in range(10)])
people = np.array([randomposition() for i in range(10)])
culture = np.array([randomposition() for i in range(10)])

supply = np.array([food, water, enemies, shelter, materials, emptySpace, people, culture])

hunger = 0.01
thirst = 0.01
peace = 0.51
heat = 0.2
activity = 0.01
space = 0.01
friends = 0.21
knowledge = 0.3
demand = np.array([hunger, thirst, peace, heat, activity, space, friends, knowledge])

# Define a average function that emphasizes the largest values
Tpowmean = lambda x, k: T.mean(T.power(x, k))
Tpowermean = lambda x, k: Tpowmean(x, k)/Tpowmean(x, k-1)

# Distances to all the minerals
supplydistances = (m - x).norm(2, axis=1)
"""
Define a function which indicates how close we are to a mineral as the 
Nth power if the inverse of 1 plus distance
"""
supplyattraction = d*Tpowermean(1/(1 + supplydistances), 4)

# Go to the direction where the closeness to minerals increase
godirection = T.jacobian(supplyattraction, x)
# Scale the go direction somewhat arbitrarily
movefunc = theano.function([x,m], godirection)

#supply*demand.reshape(8,1,1)
Tsupply = T.dmatrix('minerals')


godirection = T.jacobian(Tsupply*demand.reshape(8,1,1), x)


#Switching to torch
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
for i in range(10):
  supplydistances = (supply - pos).pow(2).sum(2).t().sqrt()
  supplyattraction = demand*powermean(1/(1+supplydistances), 4)
  happiness = supplyattraction.sum()
  happiness.backward(retain_graph=True)
  pos.data += pos.grad.data
  null = pos.grad.data.zero_()
  print(pos)
  print(happiness)


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

import numpy as np
import random
import noise
import matplotlib.pyplot as plt

def_n = 10
def_speed = 10
def_copies = 9

def gen_path(n,speed):
    keys = np.random.uniform(0,1,size=(n,2))
    pts = []

    j=0
    for i in range(n-1):
        a,b = keys[i],keys[i+1]
        diff = b-a
        dab = np.dot(diff,diff)

        mm = int(dab * speed + 1)
        print(mm)

        tt = np.linspace(0,1, mm)
        way = np.array([a*way + b*(1-way) for way in tt])


        for k in range(way.shape[0]):
            x,y = way[k,0],way[k,1]
            noiz = [noise.pnoise3(x-4.2,y,j/100),
                    noise.pnoise3(x,y-9.1,j/100)]
            way[k] += np.array(noiz)
            j += 1

            pts.append(way[k])

    return np.array(pts)

''' Add noise to a path '''
def noisify(pts, factor=.5, off=None):
    if off is None: off = np.zeros(2)
    pts2 = np.copy(pts)
    for i,pt2 in enumerate(pts2):
        x,y = pt2+off
        pt2 += factor * np.array([
                    noise.pnoise3(x+22,y-1,i/100),
                    noise.pnoise3(x+92,y-21,i/100)])
    return pts2

''' copy orig_path many times, each with more noise '''
def noisify_many(orig_path, m=9, different_noise=True):
    if different_noise:
        off = np.random.uniform(size=[m,orig_path.shape[-1]])*.8
    else:
        off = None
    pts = np.zeros([m+1,*orig_path.shape])
    pts[0] = orig_path
    factors = np.linspace(.5,3,m)
    for j,fac in enumerate(factors):
        pts[j+1] = noisify(orig_path, fac, off[j])
    return pts

def get_paths_increasing_badness(n=def_n,speed=def_speed, m=def_copies):
    p = gen_path(n,speed)
    return noisify_many(p,m)


def draw(ps):
    for p in ps:
        plt.scatter(p[:,0],p[:,1])
    plt.show()

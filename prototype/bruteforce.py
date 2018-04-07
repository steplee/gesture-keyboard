import numpy as np

'''
Try all alignments, so long as a < b < c satisfied.
Does not interpolate.
'''

def compare(a,b):
    if len(a) == 0 or len(b) == 0:
        return 0.
    
    best = 99999.

    for i in range(len(a)):
        s = compare


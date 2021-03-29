#!/usr/bin/env python3
import numpy as np
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("n",
                    help="Sizes of matrix and vectors",
                    type=int)
parser.add_argument("prop_0",
                    help="Approximate proportion of 0's",
                    type=float)
args = parser.parse_args()

# This is the current accepted best practice
# rng = np.random.default_rng()
# A = rng.choice((0, 1), size=(args.n, args.n),
#               p=(args.prop_0, 1 - args.prop_0)).astype(np.int32)

A = np.random.choice((0, 1), size=(args.n, args.n),
                     p=(args.prop_0, 1 - args.prop_0)).astype(np.int32)
x = np.arange(1, args.n + 1, dtype=np.int32).reshape((-1, 1))
y = np.arange(args.n + 1, 2 * args.n + 1, dtype=np.int32).reshape((-1, 1))
Axy = A @ x + y

# print(A)
# print(x)
# print(y)
# print(Axy)

#A.tofile(f'A_{args.n}_{args.prop_0}.dat')
#x.tofile(f'x_{args.n}_{args.prop_0}.dat')
#y.tofile(f'y_{args.n}_{args.prop_0}.dat')
#Axy.tofile(f'Axy_{args.n}_{args.prop_0}.dat')

A.tofile(f'A_{args.n}.dat')
x.tofile(f'x_{args.n}.dat')
y.tofile(f'y_{args.n}.dat')
Axy.tofile(f'Axy_{args.n}.dat')

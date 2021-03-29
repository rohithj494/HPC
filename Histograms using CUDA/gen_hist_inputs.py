#!/usr/bin/env python3
import numpy as np
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("n",
                    help="Number of samples to generate for each distribution",
                    type=int)
args = parser.parse_args()

rng = np.random.default_rng()

norm_vals = rng.standard_normal(args.n, dtype=np.float32)
norm_vals.tofile(f"normal_random_{args.n}.dat")

norm_vals.sort()
norm_vals.tofile(f"normal_sorted_{args.n}.dat")

uniform_vals = np.linspace(-4, 4, args.n, endpoint=False, dtype=np.float32)
uniform_vals.tofile(f"uniform_sorted_{args.n}.dat")

rng.shuffle(uniform_vals)
uniform_vals.tofile(f"uniform_random_{args.n}.dat")

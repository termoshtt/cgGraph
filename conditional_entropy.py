#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
import msgpack
import argparse


def count_transition(indices):
    p = {}
    cp = {}
    N = len(indices)
    for i in range(N - 1):
        now = indices[i]
        nex = indices[i + 1]
        t = (now, nex)
        if now not in p:
            p[now] = 0.0
        p[now] += 1.0
        if t not in cp:
            cp[t] = 0.0
        cp[t] += 1.0
    cp = {k: val / p[k[0]] for k, val in cp.items()}
    invN = 1.0 / (N - 1)
    p = {k: v * invN for k, v in p.items()}
    return p, cp


def conditional_entropy(p, cp):
    def _xlogx(x):
        return x * np.log(x) if x > 0 else 0.0
    s = 0.0
    for k, v in cp.items():
        f, _ = k
        s -= p[f] * _xlogx(v)
    return s


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("filename")
    args = parser.parse_args()

    with open(args.filename, "rb") as f:
        val = msgpack.unpack(f)
    p, cp = count_transition(val)
    print(conditional_entropy(p, cp))

if __name__ == '__main__':
    main()

# -*- coding: utf-8 -*-

import cgGraph_pb2
import networkx as nx
from sklearn.decomposition import PCA


def _load(t):
    def func(filename):
        val = t()
        with open(filename, "rb") as f:
            val.ParseFromString(f.read())
        return val
    return func

load_TimeLine = _load(cgGraph_pb2.TimeLine)
load_Omega = _load(cgGraph_pb2.Omega)
load_TransitionMap = _load(cgGraph_pb2.TransitionMap)


def get_network(tmap, n_components=2, **kwds):
    """
    export networkX.Graph

    Parameters
    ----------
    tmap : cgGraph_pb2.TransitionMap
        Transition map

    Returns
    -------
    (networkx.DiGraph, sklearn.decomposition.PCA)
        Generated graph and fitted PCA object
    """
    G = nx.DiGraph()
    pca = PCA(n_components=n_components, **kwds)
    comp = pca.fit_transform([st.coordinate.x for st in tmap.state])
    for i, st in enumerate(tmap.state):
        G.add_node(
            st.index,
            duration=st.duration,
            count=st.count,
            prob=st.prob,
            entropy=st.entropy,
            rate=st.rate,
            coordinate=st.coordinate.x,
            comp=comp[i]
        )
    for t in tmap.transit:
        G.add_edge(
            t.from_index,
            t.to_index,
            count=t.count,
            prob=t.prob
        )
    return G, pca

#!/usr/bin/env python
# -*- coding: utf-8 -*-

import cgGraph
import networkx as nx
import matplotlib.pyplot as plt
from numpy import array


if __name__ == '__main__':
    tmap = cgGraph.load_TransitionMap("sample/pp.bin")
    G, _ = cgGraph.get_network(tmap)
    pos = nx.get_node_attributes(G, "comp")
    prob = nx.get_node_attributes(G, "prob")
    entropy = nx.get_node_attributes(G, "entropy")
    color = array(prob.values()) * array(entropy.values())
    nodes = nx.draw_networkx_nodes(G, pos, node_size=30, node_color=color, cmap=plt.cm.Blues)
    edges = nx.draw_networkx_edges(G, pos)
    plt.colorbar(nodes, label="entropy generation")
    plt.xlabel("PCA1")
    plt.ylabel("PCA2")
    plt.axis("tight")
    plt.savefig("network.pdf")

import matplotlib.pyplot as plt
import networkx as nx

def add_edges_binary_tree(G, root, left, right):
    """Helper function to add edges for a binary tree"""
    if left:
        G.add_edge(root, left)
    if right:
        G.add_edge(root, right)

def create_binary_tree():
    """Creates a binary tree and returns it"""
    G = nx.DiGraph()

    # Adding edges to create a binary tree
    add_edges_binary_tree(G, 'A', 'B', 'C')
    add_edges_binary_tree(G, 'B', 'D', 'E')
    add_edges_binary_tree(G, 'C', 'F', 'G')
    add_edges_binary_tree(G, 'D', 'H',  'I')
    add_edges_binary_tree(G, 'E', None, None)
    add_edges_binary_tree(G, 'F', None, None)
    add_edges_binary_tree(G, 'G', None, None)

    return G

def hierarchy_pos(G, root=None, width=2., vert_gap=0.1, vert_loc=0, xcenter=0.5):
    """
    Custom function to position nodes in a hierarchical layout with shorter edge lengths.
    G: the graph (must be a tree)
    root: the root node of current branch
    - returns: positions in a dictionary
    """
    pos = _hierarchy_pos(G, root, width, vert_gap, vert_loc, xcenter)
    return pos

def _hierarchy_pos(G, root, width=1., vert_gap=0.1, vert_loc=0, xcenter=0.5, pos=None, parent=None, parsed=None):
    if pos is None:
        pos = {root: (xcenter, vert_loc)}
    else:
        pos[root] = (xcenter, vert_loc)
    if parsed is None:
        parsed = {root}
    else:
        parsed.add(root)
    neighbors = list(G.neighbors(root))
    if not isinstance(G, nx.DiGraph) and parent is not None:
        neighbors.remove(parent)  # this is the crucial trick to keep directed edges pointing correctly
    if len(neighbors) != 0:
        dx = width / len(neighbors) 
        nextx = xcenter - width / 2 - dx / 2
        for neighbor in neighbors:
            nextx += dx
            pos = _hierarchy_pos(G, neighbor, width=dx, vert_gap=vert_gap, vert_loc=vert_loc - vert_gap, xcenter=nextx,
                                 pos=pos, parent=root, parsed=parsed)
    return pos

def plot_binary_tree(G):
    """Plots the binary tree with the root at the top"""
    pos = hierarchy_pos(G, 'A')  # positions for all nodes

    plt.figure(figsize=(10, 10))  # Adjust figure size if needed
    nx.draw(G, pos, with_labels=True, node_size=6000, node_color="lightgreen", font_size=40, font_weight="bold", arrows=False)

    # Save plot to a file
    plt.savefig("binary_tree_plot_short_edges.png")

    # Show plot
    #plt.show()

# Create the binary tree
G = create_binary_tree()

# Plot the binary tree
plot_binary_tree(G)

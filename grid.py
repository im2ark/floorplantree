import matplotlib.pyplot as plt
import numpy as np

def create_colored_grid(rows, cols):
    """Create a grid of rectangles with random colors."""
    # Create a figure and axis
    fig, ax = plt.subplots(figsize=(10, 10))

    # Generate random colors
    colors = np.random.rand(rows, cols, 3)  # RGB colors

    # Create the grid
    for r in range(rows):
        for c in range(cols):
            rect = plt.Rectangle((c, r), 1, 1, color=colors[r, c])
            ax.add_patch(rect)

    # Set the limits and aspect ratio
    ax.set_xlim(0, cols)
    ax.set_ylim(0, rows)
    ax.set_aspect('equal')

    # Hide the axes
    ax.axis('off')

    # Save and show the plot
    plt.savefig("colored_grid_plot.png")
    #plt.show()

# Define the grid size
rows = 10
cols = 15

# Create and plot the colored grid
create_colored_grid(rows, cols)

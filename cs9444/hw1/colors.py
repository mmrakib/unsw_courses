import numpy as np
import matplotlib.pyplot as plt

# Given activation values for each state
activation_values = {
    1: 100.000000, 2: 50.000000, 3: 25.000000, 4: 10.000000, 5: 5.000000,
    6: 2.000000, 7: 1.000000, 8: 0.500000, 9: 0.250000, 10: 0.100000,
    11: 0.050000, 12: 0.025000, 13: 0.010000, 14: 0.005000, 15: 0.002000,
    16: 0.001000, 17: 0.000500, 18: 0.000250, 19: 0.000100, 20: 0.000050,
    21: 0.000025, 22: 0.000010, 23: 0.000005, 24: 0.000002, 25: 0.000001,
    26: 0.0000005, 27: 0.00000025, 28: 0.0000001, 29: 0.00000005, 30: 0.000000025
}

# Normalize the activation values
max_activation = max(activation_values.values())
min_activation = min(activation_values.values())
normalized_activations = {k: (v - min_activation) / (max_activation - min_activation) for k, v in activation_values.items()}

# Map normalized values to colors using the jet colormap
colormap = plt.cm.jet
colors = {k: colormap(v) for k, v in normalized_activations.items()}

# Display the results
import pandas as pd
activation_df = pd.DataFrame.from_dict(colors, orient='index', columns=['R', 'G', 'B', 'A'])
activation_df.index.name = 'State'
activation_df.reset_index(inplace=True)

import ace_tools as tools; tools.display_dataframe_to_user(name="Activation Colors by State", dataframe=activation_df)

"""
   check.py
   COMP9444, CSE, UNSW
"""

import torch
import torch.nn as nn
import matplotlib.pyplot as plt

class MLP(torch.nn.Module):
    def __init__(self, hid=4, act='sig'):
        super(MLP, self).__init__()
 
        self.act = act
        self.in_hid  = nn.Linear(2, hid)
        self.hid_out = nn.Linear(hid, 1)
        self.hid = None

    def forward(self, input):
        self.hid = torch.sigmoid(self.in_hid(input))
        if self.act == 'step':
            self.hid = (self.in_hid(input) >= 0).float()
            return (self.hid_out(self.hid) >= 0).float()
        else:         # sigmoid
            self.hid = torch.sigmoid(self.in_hid(input))
            return torch.sigmoid(self.hid_out(self.hid))

    def set_weights(self):
        # Unscaled weights:
        #in_hid_weight  = [[1, 1], [1, 1], [1, 1], [1, 1]]
        #hid_bias       = [-1.5, -0.5, 0.5, 1.5]
        #hid_out_weight = [[1, -1, 1, -1]]
        #out_bias       = [0]

        # Scaled weights:
        in_hid_weight  = [[500, 500], [500, 500], [500, 500], [500, 500]]
        hid_bias       = [-750, -250, 250, 750]
        hid_out_weight = [[500, -500, 500, -500]]
        out_bias       = [0]

        self.in_hid.weight.data = torch.tensor(
             in_hid_weight, dtype=torch.float32)
        self.in_hid.bias.data   = torch.tensor(
                hid_bias,   dtype=torch.float32)
        self.hid_out.weight.data= torch.tensor(
             hid_out_weight,dtype=torch.float32)
        self.hid_out.bias.data  = torch.tensor(
                 out_bias,  dtype=torch.float32)

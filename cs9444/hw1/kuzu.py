"""
   kuzu.py
   COMP9444, CSE, UNSW
"""

from __future__ import print_function
import torch
import torch.nn as nn
import torch.nn.functional as F

#
# Linear function, log softmax
#
class NetLin(nn.Module):
    def __init__(self):
        super(NetLin, self).__init__()

        input_size = 28 * 28
        output_size = 10

        self.fc = nn.Linear(input_size, output_size)

    def forward(self, x):
        x = x.view(-1, 28 * 28)
        x = self.fc(x)

        return F.log_softmax(x, dim=1)

#
# Fully connected 2-layer network, tanh and log softmax
#
class NetFull(nn.Module):
    def __init__(self):
        super(NetFull, self).__init__()
        hidden_size = 200

        self.fc1 = nn.Linear(28 * 28, hidden_size)
        self.fc2 = nn.Linear(hidden_size, 10)

    def forward(self, x):
        x = x.view(-1, 28 * 28)
        x = torch.tanh(self.fc1(x))
        x = self.fc2(x)

        return F.log_softmax(x, dim=1)

#
# Convolutional network
#
class NetConv(nn.Module):
    def __init__(self):
        super(NetConv, self).__init__()

        self.conv1 = nn.Conv2d(1, 32, kernel_size=3, padding=1)
        self.conv2 = nn.Conv2d(32, 64, kernel_size=3, padding=1)

        self.fc1 = nn.Linear(64 * 7 * 7, 128)
        self.fc2 = nn.Linear(128, 10)

        self.pool = nn.MaxPool2d(2, 2)

    def forward(self, x):
        x = F.relu(self.conv1(x))
        x = self.pool(x)

        x = F.relu(self.conv2(x))
        x = self.pool(x)

        x = x.view(-1, 64 * 7 * 7)
        x = F.relu(self.fc1(x))

        x = self.fc2(x)
        return F.log_softmax(x, dim=1)

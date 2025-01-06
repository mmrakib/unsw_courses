import json
import gzip

from torch.utils.data import Dataset, DataLoader

class RawData(Dataset):
    def __init__(self, path):
        self.data = []
        with gzip.open(path, 'rt', encoding='utf-8') as file:
            for line in file:
                self.data.append(json.loads(line))

    def __len__(self):
        return len(self.data)

    def __getitem__(self, idx):
        return self.data[idx]

class TokenizedData(Dataset):
    def __init__(self):
        pass

    def __len__(self):
        pass

    def __getitem__(self, idx):
        pass

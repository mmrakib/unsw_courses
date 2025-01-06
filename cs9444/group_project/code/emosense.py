import argparse

import pandas as pd
from tqdm import tqdm

from datasets import load_dataset

from transformers import BertTokenizer, BertForSequenceClassification
from sklearn.metrics import classification_report
import evaluate

import torch
from torch.utils.data import DataLoader, Dataset
from torch.optim import AdamW
from torch.nn import functional as F

desc = 'A text-based NLP emotion classifier trained on Tweets.'

# Load the dataset
print("Loading dataset...")
dataset = load_dataset("emotion")

# Split dataset into training and testing sets
dataset = dataset["train"].train_test_split(test_size=0.2)
train_dataset = dataset["train"]
test_dataset = dataset["test"]

# Load the tokenizer and model
model_name = "bert-base-uncased"
print(f"Loading tokenizer and model: {model_name}")
tokenizer = BertTokenizer.from_pretrained(model_name)
model = BertForSequenceClassification.from_pretrained(model_name, num_labels=6)

# Custom Dataset class
class TextDataset(Dataset):
    def __init__(self, texts, labels, tokenizer, max_len):
        self.texts = texts
        self.labels = labels
        self.tokenizer = tokenizer
        self.max_len = max_len

    def __len__(self):
        return len(self.texts)

    def __getitem__(self, idx):
        text = self.texts[idx]
        label = self.labels[idx]
        encoding = self.tokenizer.encode_plus(
            text,
            max_length=self.max_len,
            padding='max_length',
            truncation=True,
            return_tensors='pt'
        )
        return {
            'input_ids': encoding['input_ids'].flatten(),
            'attention_mask': encoding['attention_mask'].flatten(),
            'label': torch.tensor(label, dtype=torch.long)
        }

# Create Dataset and DataLoader
max_len = 128
batch_size = 16

train_dataset = TextDataset(train_dataset['text'], train_dataset['label'], tokenizer, max_len)
test_dataset = TextDataset(test_dataset['text'], test_dataset['label'], tokenizer, max_len)

train_loader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True)
test_loader = DataLoader(test_dataset, batch_size=batch_size, shuffle=False)

# Set device
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
model.to(device)

# Training function
def train_epoch(model, data_loader, optimizer, device, scheduler):
    model = model.train()
    total_loss = 0

    for batch in tqdm(data_loader, desc="Training"):
        input_ids = batch['input_ids'].to(device)
        attention_mask = batch['attention_mask'].to(device)
        labels = batch['label'].to(device)

        optimizer.zero_grad()

        outputs = model(input_ids=input_ids, attention_mask=attention_mask, labels=labels)
        loss = outputs.loss
        loss.backward()
        optimizer.step()
        scheduler.step()

        total_loss += loss.item()

    return total_loss / len(data_loader)

# Evaluation function
def eval_model(model, data_loader, device):
    model = model.eval()
    preds = []
    labels = []

    with torch.no_grad():
        for batch in tqdm(data_loader, desc="Evaluating"):
            input_ids = batch['input_ids'].to(device)
            attention_mask = batch['attention_mask'].to(device)
            labels.extend(batch['label'].cpu().numpy())

            outputs = model(input_ids=input_ids, attention_mask=attention_mask)
            logits = outputs.logits
            preds.extend(torch.argmax(logits, dim=1).cpu().numpy())

    return preds, labels

# Optimizer and learning rate scheduler
optimizer = AdamW(model.parameters(), lr=2e-5)
total_steps = len(train_loader) * 3  # 3 epochs
scheduler = torch.optim.lr_scheduler.StepLR(optimizer, step_size=total_steps // 3, gamma=0.1)

# Training loop
epochs = 3

for epoch in range(epochs):
    print(f'Epoch {epoch + 1}/{epochs}')
    train_loss = train_epoch(model, train_loader, optimizer, device, scheduler)
    print(f'Training loss: {train_loss}')

# Evaluate the model
print("Evaluating the model...")
preds, labels = eval_model(model, test_loader, device)

# Output statistics
print("Evaluation results:")
accuracy_metric = evaluate.load("accuracy")
accuracy = accuracy_metric.compute(predictions=preds, references=labels)
print(f"Accuracy: {accuracy['accuracy']}")

# Detailed classification report
emotions = ['anger', 'fear', 'joy', 'love', 'sadness', 'surprise']
print(classification_report(labels, preds, target_names=emotions))

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=desc)

import json
import gzip
import pandas as pd
import os
from sklearn.model_selection import train_test_split
from transformers import BertTokenizer, BertForSequenceClassification, Trainer, TrainingArguments
from transformers import DataCollatorWithPadding
from datasets import Dataset, load_from_disk
import torch
import evaluate

# Check CUDA device initialization
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print(f"Using device: {device}")

# Create a test tensor to check device utilization
test_tensor = torch.tensor([1.0, 2.0, 3.0]).to(device)
print(f"Test tensor device: {test_tensor.device}")

# Load the tokenizer and model
model_name = "bert-base-uncased"
print(f"Loading tokenizer and model: {model_name}")
tokenizer = BertTokenizer.from_pretrained(model_name)
model = BertForSequenceClassification.from_pretrained(model_name, num_labels=6).to(device)

# Verify model is on the correct device
print(f"Model device: {next(model.parameters()).device}")

# Define file paths for tokenized data
train_data_path = './datasets/emotion/train_tokenized'
test_data_path = './datasets/emotion/test_tokenized'

# Function to tokenize the data
def tokenize_function(examples):
    return tokenizer(examples["text"], padding="max_length", truncation=True)

# Load and tokenize data
if os.path.exists(train_data_path) and os.path.exists(test_data_path):
    # Load tokenized data
    print("Loading tokenized data...")
    train_dataset = load_from_disk(train_data_path)
    test_dataset = load_from_disk(test_data_path)
else:
    # Load the data
    print("Loading data...")
    data = []
    with gzip.open('./datasets/emotion/data.jsonl.gz', 'rt', encoding='utf-8') as f:
        for line in f:
            data.append(json.loads(line))

    # Convert to DataFrame
    df = pd.DataFrame(data)

    # Assuming the DataFrame has 'text' and 'label' columns
    print("Splitting data into training and testing sets...")
    train_df, test_df = train_test_split(df, test_size=0.2, random_state=42)

    # Convert DataFrame to Dataset
    train_dataset = Dataset.from_pandas(train_df)
    test_dataset = Dataset.from_pandas(test_df)

    # Tokenize the data
    print("Tokenizing data...")
    train_dataset = train_dataset.map(tokenize_function, batched=True)
    test_dataset = test_dataset.map(tokenize_function, batched=True)

    # Save tokenized data
    print("Saving tokenized data...")
    train_dataset.save_to_disk(train_data_path)
    test_dataset.save_to_disk(test_data_path)

# Define the training arguments
training_args = TrainingArguments(
    output_dir="./results",
    eval_strategy="epoch",
    learning_rate=2e-5,
    per_device_train_batch_size=16,
    per_device_eval_batch_size=16,
    num_train_epochs=3,
    weight_decay=0.01,
    logging_dir='./logs',
    logging_steps=10,
)

# Define the data collator
data_collator = DataCollatorWithPadding(tokenizer=tokenizer)

# Define the metric
accuracy_metric = evaluate.load("accuracy")

def compute_metrics(eval_pred):
    logits, labels = eval_pred
    predictions = logits.argmax(axis=-1)
    return accuracy_metric.compute(predictions=predictions, references=labels)

# Initialize the Trainer
print("Initializing Trainer...")
trainer = Trainer(
    model=model,
    args=training_args,
    train_dataset=train_dataset,
    eval_dataset=test_dataset,
    tokenizer=tokenizer,
    data_collator=data_collator,
    compute_metrics=compute_metrics,
)

# Override the training step to ensure inputs are moved to the correct device
def training_step(self, model, inputs):
    model.train()
    inputs = self._prepare_inputs(inputs)
    inputs = {k: v.to(device) for k, v in inputs.items()}
    outputs = model(**inputs)
    loss = outputs.loss
    loss.backward()
    return loss

Trainer.training_step = training_step

# Train the model
print("Training the model...")
trainer.train()

# Evaluate the model
print("Evaluating the model...")
evaluation_results = trainer.evaluate()

# Output statistics
print("Evaluation results:")
for key, value in evaluation_results.items():
    print(f"{key}: {value}")

# To get more detailed evaluation metrics
from sklearn.metrics import classification_report

print("Generating detailed classification report...")
predictions = trainer.predict(test_dataset)
y_pred = predictions.predictions.argmax(axis=1)
y_true = predictions.label_ids

# Assuming 'emotion' is the label column and it contains labels in the same order as the `num_labels=6`
emotions = ['anger', 'fear', 'joy', 'love', 'sadness', 'surprise']

print(classification_report(y_true, y_pred, target_names=emotions))

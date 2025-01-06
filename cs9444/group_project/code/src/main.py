import argparse

desc = 'A text-based NLP emotion classifier trained on Tweets.'

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=desc)

    parser.add_argument('function', type=str, choices=['prepare', 'train', 'test', 'run'])

    args = parser.parse_args()

    if args.function == 'prepare':
        pass

    if args.function == 'train':
        pass

    if args.function == 'test':
        pass

    if args.function == 'run':
        pass

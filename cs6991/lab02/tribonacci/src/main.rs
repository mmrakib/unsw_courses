use std::env;
use std::num::ParseIntError;

struct TribonacciError(String);

fn main() {
    let args: Vec<String> = env::args().collect();
    let error_message = String::from("Please enter a valid size");

    let size = match args.get(1) {
        Some(s) => s.parse::<usize>().map_err(|_| TribonacciError(error_message.clone())),
        None => Ok(10), // Default size is 10 if no argument is given
    };

    if let Err(e) = compute_tribonacci(size, error_message) {
        println!("Error: {}", e.0)
    }
}

/// Computes the tribonacci sequence of a given size
/// Prints the sequence, and its sum
fn compute_tribonacci(
    size: Result<usize, TribonacciError>,
    error_msg: String,
) -> Result<(), TribonacciError> {
    let size = size?;
    
    // Check if size is within the valid range
    if size < 3 || size > 145 {
        return Err(TribonacciError(error_msg));
    }

    // Initialize the Tribonacci sequence
    let mut tribonacci = vec![1u128, 1, 1];  // Starting with [1, 1, 1]
    
    for i in 3..size {
        let next_value = tribonacci[i - 1] + tribonacci[i - 2] + tribonacci[i - 3];
        tribonacci.push(next_value);
    }

    // Print the sequence and the sum
    println!("Values: {:?}", &tribonacci[..size]);
    let sum: u128 = tribonacci.iter().take(size).sum();
    println!("\nSum: {}", sum);

    Ok(())
}


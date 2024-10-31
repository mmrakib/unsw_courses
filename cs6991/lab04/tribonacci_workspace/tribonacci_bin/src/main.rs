use tribonacci_lib::{compute_tribonacci, get_size, TribonacciError};
use std::env;

fn main() {
    let args: Vec<String> = env::args().collect();

    match get_size(&args) {
        Ok(size) => {
            match compute_tribonacci(size) {
                Ok((sequence, sum)) => {
                    println!("Values: {:?}", sequence);
                    println!("\nSum: {}", sum);
                }
                Err(e) => {
                    eprintln!("Error computing tribonacci: {}", e.0);
                }
            }
        }
        Err(e) => {
            eprintln!("Error: {}", e.0);
        }
    }
}


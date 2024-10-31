//! # Tribonacci Library
//!
//! Provides functionality to compute the Tribonacci sequence and related utilities.

pub mod constants;
pub mod utils;

use crate::constants::ERROR_MESSAGE;
use crate::utils::first_argument;

#[derive(Debug)]
pub struct TribonacciError(pub String);

/// Computes the Tribonacci sequence of a given size.
/// Returns a vector containing the sequence and the sum.
///
/// # Examples
///
/// ```
/// use tribonacci_lib::{compute_tribonacci, TribonacciError};
///
/// let result = compute_tribonacci(5);
/// assert!(result.is_ok());
/// let (sequence, sum) = result.unwrap();
/// assert_eq!(sequence, vec![1, 1, 1, 3, 5]);
/// assert_eq!(sum, 11);
/// ```
pub fn compute_tribonacci(size: usize) -> Result<(Vec<u128>, u128), TribonacciError> {
    if size == 0 {
        return Err(TribonacciError(ERROR_MESSAGE.to_string()));
    }

    let mut tribonacci = vec![1_u128; 3];
    if size < 3 {
        tribonacci.truncate(size);
    } else {
        for i in 3..size {
            let next = tribonacci[i - 1] + tribonacci[i - 2] + tribonacci[i - 3];
            tribonacci.push(next);
        }
    }

    let sum: u128 = tribonacci.iter().sum();
    Ok((tribonacci, sum))
}

/// Parses the first command-line argument as a Tribonacci sequence size.
///
/// # Examples
///
/// ```
/// use tribonacci_lib::get_size;
///
/// let args = vec!["program".to_string(), "5".to_string()];
/// let size = get_size(&args);
/// assert_eq!(size.unwrap(), 5);
/// ```
pub fn get_size(args: &[String]) -> Result<usize, TribonacciError> {
    first_argument(args).map_err(|_| TribonacciError(ERROR_MESSAGE.to_string()))
}


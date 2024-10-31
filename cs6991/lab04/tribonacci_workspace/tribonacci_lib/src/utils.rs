use crate::constants::DEFAULT_NUM;
use std::num::ParseIntError;

/// Retrieves the first command-line argument and parses it as a `usize`.
/// If no argument is provided, returns the default number.
///
/// # Examples
///
/// ```
/// use tribonacci_lib::utils::first_argument;
///
/// let args = vec!["program".to_string(), "5".to_string()];
/// let size = first_argument(&args).unwrap();
/// assert_eq!(size, 5);
///
/// let args = vec!["program".to_string()];
/// let size = first_argument(&args).unwrap();
/// assert_eq!(size, tribonacci_lib::constants::DEFAULT_NUM as usize);
/// ```
pub fn first_argument(args: &[String]) -> Result<usize, ParseIntError> {
    match args.get(1) {
        Some(s) => s.parse::<usize>(),
        None => Ok(DEFAULT_NUM.into()),
    }
}


//! # Doctor Who
//! 
//! Doctor Who is a Rust library that provides functionality to perform Caesar cipher shifts on input text.
//! It allows users to encrypt or decrypt messages by shifting the alphabetic characters by a specified number.

const DEFAULT_SHIFT: i32 = 5; // The default number of positions to shift if none is specified.
const UPPERCASE_A: i32 = 65; // ASCII value for uppercase 'A'.
const LOWERCASE_A: i32 = 97; // ASCII value for lowercase 'a'.
const ALPHABET_SIZE: i32 = 26; // Number of letters in the English alphabet.

/// Shifts the letters in each line of input text by the specified number of positions using the Caesar cipher.
/// 
/// # Examples
/// 
/// ```
/// use doctor_who::caesar_shift;
/// 
/// let input = vec!["Hello, World!".to_string()];
/// let shifted = caesar_shift(Some(3), input);
/// assert_eq!(shifted[0], "Khoor, Zruog!");
/// ```
/// 
/// If no shift is specified, the default shift of 5 is used.
///
/// # Parameters
/// * `shift_by` - The number of positions to shift each letter. If `None`, uses the default shift.
/// * `lines` - A vector of strings, each representing a line of text to be shifted.
///
/// # Returns
/// A vector of strings with each line's alphabetic characters shifted accordingly.
pub fn caesar_shift(shift_by: Option<i32>, lines: Vec<String>) -> Vec<String> {
    let shift_number = shift_by.unwrap_or(DEFAULT_SHIFT);
    
    lines
        .iter()
        .map(|line| shift(shift_number, line.to_string()))
        .collect()
}

/// Shifts the characters in a single line by the specified number using the Caesar cipher.
/// 
/// This function is private and not exposed to the library's users.
///
/// # Parameters
/// * `shift_by` - The number of positions to shift each letter.
/// * `line` - A string representing a line of text to be shifted.
///
/// # Returns
/// A new string with alphabetic characters shifted accordingly.
fn shift(shift_by: i32, line: String) -> String {
    let mut result: Vec<char> = Vec::new();

    // Normalize shift to a positive number within the alphabet size.
    let shift_by = shift_by % ALPHABET_SIZE + ALPHABET_SIZE;

    line.chars().for_each(|c| {
        let ascii = c as i32;

        if ('A'..='Z').contains(&c) {
            result.push(to_ascii(
                abs_modulo((ascii - UPPERCASE_A) + shift_by, ALPHABET_SIZE) + UPPERCASE_A,
            ));
        } else if ('a'..='z').contains(&c) {
            result.push(to_ascii(
                abs_modulo((ascii - LOWERCASE_A) + shift_by, ALPHABET_SIZE) + LOWERCASE_A,
            ));
        } else {
            result.push(c)
        }
    });

    result.iter().collect()
}

/// Computes the absolute modulo of two integers.
///
/// This function is private and not exposed to the library's users.
///
/// # Parameters
/// * `a` - The dividend.
/// * `b` - The divisor.
///
/// # Returns
/// The absolute value of `a` modulo `b`.
fn abs_modulo(a: i32, b: i32) -> i32 {
    (a % b).abs()
}

/// Converts an integer to its corresponding ASCII character.
///
/// This function is private and not exposed to the library's users.
///
/// # Parameters
/// * `i` - The integer to convert.
///
/// # Returns
/// The corresponding ASCII character.
fn to_ascii(i: i32) -> char {
    char::from_u32(i as u32).unwrap()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_caesar_shift_with_shift() {
        let input = vec!["Rustaceans!".to_string()];
        let shifted = caesar_shift(Some(2), input);
        assert_eq!(shifted[0], "Twuvcegcpu!");
    }

    #[test]
    fn test_caesar_shift_default_shift() {
        let input = vec!["Rust".to_string()];
        let shifted = caesar_shift(None, input);
        assert_eq!(shifted[0], "Wzxy");
    }

    #[test]
    fn test_caesar_shift_negative_shift() {
        let input = vec!["Hello".to_string()];
        let shifted = caesar_shift(Some(-3), input);
        assert_eq!(shifted[0], "Ebiil");
    }

    #[test]
    fn test_caesar_shift_non_alphabetic() {
        let input = vec!["123 ABC abc!".to_string()];
        let shifted = caesar_shift(Some(1), input);
        assert_eq!(shifted[0], "123 BCD bcd!");
    }
}


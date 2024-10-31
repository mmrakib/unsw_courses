fn main() {
    // Get the first argument from the command line
    let arg = std::env::args().nth(1).expect("Please provide a valid argument");

    // Convert the argument to uppercase
    let upp = uppercase(&arg);

    println!("arg = {}", arg);
    println!("upp = {}", upp);
}

// Function to convert a string to uppercase
fn uppercase(src: &str) -> String {
    let mut destination = String::new();

    for c in src.chars() {
        // to_uppercase returns an iterator, so we need to push each char
        destination.push_str(&c.to_uppercase().to_string());
    }

    destination
}


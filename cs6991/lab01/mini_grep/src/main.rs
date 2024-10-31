use std::io;

fn main() {
    let pattern_string = std::env::args()
        .nth(1)
        .expect("missing required command-line argument: <pattern>");

    let pattern = &pattern_string;

    loop {
        let mut input = String::new();

        let result = io::stdin().read_line(&mut input);

        match result {
            Ok(0) => break,
            Ok(_) => {
                if input.trim().contains(pattern) {
                    println!("{}", input.trim());
                }
            },
            Err(e) => {
                eprintln!("error: {}", e);
                break;
            }
        }
    }
}

use serde::Deserialize;
use std::collections::VecDeque;
use std::io;

#[derive(Debug, Deserialize)]
enum Instruction {
    Set(i32),
    Left,
    Right,
    Reset,
}

#[derive(Debug)]
struct Light {
    left: Option<Box<Light>>,
    right: Option<Box<Light>>,
    brightness: i32,
}

impl Light {
    fn new() -> Self {
        Light {
            left: None,
            right: None,
            brightness: 0,
        }
    }

    fn set_brightness(&mut self, value: i32) {
        self.brightness = value;
    }

    fn move_left(&mut self) -> &mut Light {
        if self.left.is_none() {
            self.left = Some(Box::new(Light::new()));
        }
        self.left.as_mut().unwrap()
    }

    fn move_right(&mut self) -> &mut Light {
        if self.right.is_none() {
            self.right = Some(Box::new(Light::new()));
        }
        self.right.as_mut().unwrap()
    }

    fn calculate_brightness(&self) -> (i32, i32) {
        let mut total_brightness = self.brightness;
        let mut total_lights = 1;

        if let Some(left) = &self.left {
            let (left_brightness, left_lights) = left.calculate_brightness();
            total_brightness += left_brightness;
            total_lights += left_lights;
        }

        if let Some(right) = &self.right {
            let (right_brightness, right_lights) = right.calculate_brightness();
            total_brightness += right_brightness;
            total_lights += right_lights;
        }

        (total_brightness, total_lights)
    }
}

fn get_instructions_from_stdin() -> VecDeque<Instruction> {
    let mut instructions = String::new();
    io::stdin().read_line(&mut instructions).unwrap();
    ron::from_str(&instructions).unwrap()
}

fn main() {
    let instructions = get_instructions_from_stdin();
    let mut root = Light::new();
    let mut current = &mut root;

    for instruction in instructions {
        match instruction {
            Instruction::Set(value) => {
                current.set_brightness(value);
            }
            Instruction::Left => {
                current = current.move_left();
            }
            Instruction::Right => {
                current = current.move_right();
            }
            Instruction::Reset => {
                current = &mut root;
            }
        }
    }

    let (total_brightness, total_lights) = root.calculate_brightness();

    let average_brightness = total_brightness as f64 / total_lights as f64;
    println!("{}", average_brightness.round() as i32);
}


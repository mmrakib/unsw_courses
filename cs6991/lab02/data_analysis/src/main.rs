use std::collections::{HashMap, HashSet};
use serde::Deserialize;
use std::error::Error;
use std::fs::File;

#[derive(Debug, Deserialize)]
struct Enrolment {
    course_code: String,
    student_number: String,
    name: String,
    program: String,
    plan: String,
    wam: Option<f64>,
    session: String,
    birthdate: String,
    sex: String,
}

fn main() -> Result<(), Box<dyn Error>> {
    // Open the .psv file
    let file = File::open("enrolments.psv")?;
    
    // Set up CSV reader without headers and with pipe delimiter
    let mut rdr = csv::ReaderBuilder::new()
        .has_headers(false)  // Disable headers
        .delimiter(b'|')     // Use pipe as delimiter
        .from_reader(file);
    
    let mut unique_students = HashSet::new();
    let mut course_counts = HashMap::new();
    let mut total_wam_scaled = 0;  // Total WAM scaled by 100 to avoid floating point precision issues
    let mut wam_count = 0;

    for result in rdr.deserialize() {
        let enrolment: Enrolment = result?;
        
        // Add student number to unique set
        unique_students.insert(enrolment.student_number.clone());

        // Count the occurrences of each course code
        *course_counts.entry(enrolment.course_code.clone()).or_insert(0) += 1;

        // If the WAM is present, valid (greater than 0.0), scale it by 100 and add to the total
        if let Some(wam) = enrolment.wam {
            if wam > 0.0 {
                total_wam_scaled += (wam * 100.0) as i64;  // Scale WAM to avoid floating point rounding issues
                wam_count += 1;
            }
        }
    }

    // Find the most and least common course
    let most_common_course = course_counts
        .iter()
        .max_by_key(|entry| entry.1)
        .map(|(course, count)| (course.clone(), *count))
        .unwrap();
    
    let least_common_course = course_counts
        .iter()
        .min_by_key(|entry| entry.1)
        .map(|(course, count)| (course.clone(), *count))
        .unwrap();

    // Calculate average WAM with integer math, then divide by 100 for final result
    let average_wam = if wam_count > 0 {
        (total_wam_scaled as f64 / wam_count as f64) / 100.0  // Re-scale the WAM after calculating the average
    } else {
        0.0
    };

    // Output the results with correct formatting
    println!("Number of students: {}", unique_students.len());
    println!("Most common course: {} with {} students", most_common_course.0, most_common_course.1);
    println!("Least common course: {} with {} students", least_common_course.0, least_common_course.1);
    println!("Average WAM: {:.2}", average_wam);

    Ok(())
}


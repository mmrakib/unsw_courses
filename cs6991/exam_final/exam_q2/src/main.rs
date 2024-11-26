use exam_q2_lib::{find_str, most_common};

fn main() {
    let result;
    {
        let list_of_strs = vec!["hello", "world", "correct", "correct"];
        {
            let search_string = String::from("correct"); 
            result = find_str(&list_of_strs, search_string.as_str());
        }
        println!("find_str: {:?}", result);
    }

    let result = {
        let list_of_strs = vec!["hello", "world", "correct", "correct"];
        most_common(&list_of_strs)
    };
    println!("most_common: {:?}", result);
}

use std::collections::{VecDeque, LinkedList, HashMap};

/*
1. Which collection type was the fastest for adding and removing elements?
Vec was the fastest for insertion, but VecDeque was by far the fastest for removal. 

2. Why do you think this was the case?
Vec benefits from contiguous memory allocation, making insertion relatively fast. However, removing in Vec is slow because it requires shifting all the remaining elements when moving from the front.

3. Is there any significant difference between Vec and VecDeque deletion?
Yes. The difference is significant, given the fact that it is the difference shifting all elements vs. not shifting any elements.

4. When would you consider using VecDeque over Vec?
VecDeque should be used when efficient insertions and removals at the ends are needed, but not necessarily in the middle.

5. When would you consider using LinkedList over Vec?
It would only be preferable in cases where we need extensive mid-list manipulation.

6. Did the results surprise you? Why or why not?
Yes, the results are somewhat surprising. I expected LinkedList to perform better, but it was significantly slower in both insertion and removal compared to VecDeque. Additionally, the large difference between Vec and VecDeque for removals confirmed how inefficient front removal in Vec can be. HashMap's insertion and removal times were within expectations due to the overhead associated with maintaining a hash table.
*/

const MAX_ITER: i32 = 300000;

fn main() {
    // Vectors
    vec_operations();

    // VecDeque
    vec_deque_operations();

    // LinkedList
    linked_list_operations();

    // HashMap
    hash_map_operations();
}

/// measure the insertion and removal
/// operations of a vector
fn vec_operations() {
    let mut vec = Vec::new();

    let time_start = std::time::Instant::now();
    for i in 0..MAX_ITER {
        vec.push(i);
    }
    let time_end = std::time::Instant::now();

    println!("==== Vector ====");
    println!("insert: {:?}", time_end - time_start);

    let time_start = std::time::Instant::now();
    for _ in 0..MAX_ITER {
        vec.remove(0);
    }
    let time_end = std::time::Instant::now();

    println!("remove: {:?}", time_end - time_start);
}

/// measure the insertion and removal
/// operations of a VecDeque
fn vec_deque_operations() {
    let mut vec_deque = VecDeque::new();

    let time_start = std::time::Instant::now();
    for i in 0..MAX_ITER {
        vec_deque.push_back(i);
    }
    let time_end = std::time::Instant::now();

    println!("==== VecDeque ====");
    println!("insert: {:?}", time_end - time_start);

    let time_start = std::time::Instant::now();
    for _ in 0..MAX_ITER {
        vec_deque.pop_front();
    }
    let time_end = std::time::Instant::now();

    println!("remove: {:?}", time_end - time_start);
}

/// measure the insertion and removal
/// operations of a LinkedList
fn linked_list_operations() {
    let mut linked_list = LinkedList::new();

    let time_start = std::time::Instant::now();
    for i in 0..MAX_ITER {
        linked_list.push_back(i);
    }
    let time_end = std::time::Instant::now();

    println!("==== LinkedList ====");
    println!("insert: {:?}", time_end - time_start);

    let time_start = std::time::Instant::now();
    for _ in 0..MAX_ITER {
        linked_list.pop_front();
    }
    let time_end = std::time::Instant::now();

    println!("remove: {:?}", time_end - time_start);
}

/// measure the insertion and removal
/// operations of a HashMap
fn hash_map_operations() {
    let mut hash_map = HashMap::new();

    let time_start = std::time::Instant::now();
    for i in 0..MAX_ITER {
        hash_map.insert(i, i);
    }
    let time_end = std::time::Instant::now();

    println!("==== HashMap ====");
    println!("insert: {:?}", time_end - time_start);

    let time_start = std::time::Instant::now();
    for i in 0..MAX_ITER {
        hash_map.remove(&i);
    }
    let time_end = std::time::Instant::now();

    println!("remove: {:?}", time_end - time_start);
}


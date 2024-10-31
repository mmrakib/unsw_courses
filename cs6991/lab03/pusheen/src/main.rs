/* 
 * In Rust, any given resource can be borrowed mutably *once*, and only once.
 * This program attempts to mutably reference 'vec' twice, which the compiler rejects.
 */

fn main() {
    let mut vec = vec![1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    
    /*
     * To fix the issue of multiple mutable references, we simply remove on them, and only access
     * the resource with the other one.
     */
    let a = &mut vec;
    a.push(11);
    a.push(12);
}

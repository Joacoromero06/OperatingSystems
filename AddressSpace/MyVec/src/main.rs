mod my_vec;
use my_vec::MyVec;
fn main() {
    println!("Hello, world!");
    let mut buf = MyVec::from_capacity(10);
}

use std::alloc::{alloc,dealloc, handle_alloc_error, Layout};

fn main(){

    let layout_oneU8 = Layout::from_size_align(1,1).unwrap();
    let layout_arrU8 = Layout::array::<u8>(30).unwrap();
    unsafe {
        let p: *mut u8 = alloc(layout_oneU8);
        if p.is_null() {handle_alloc_error(layout_oneU8);}
        *p = 10;
        println!("p = {:p} -> {}", p, *p);
        dealloc(p, layout_oneU8);

        let arr: *mut u8 = alloc(layout_arrU8);
        if arr.is_null() {handle_alloc_error(layout_arrU8);}
        println!("array = {:p} -> 1000u8", arr);
        for i in 0..29 {
            println!("{}", *arr.wrapping_add(i)); // arr[i]
        }
        dealloc(p, layout_arrU8);
    }
}

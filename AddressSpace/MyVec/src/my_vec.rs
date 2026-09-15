use std::alloc::{alloc, dealloc, realloc, handle_alloc_error, Layout};
use std::ptr;

pub struct MyVec<T> {
    ptr: *mut T,
    len: usize,
    cap: usize,
}
impl<T> MyVec<T> {
    pub fn new() -> Self {
        MyVec{ ptr: ptr::null_mut(), len: 0, cap: 0 }
    }
    priv fn new(p: *mut<T>, cap: usize, len:usize) -> Self {
        // creo que no funcionara porque en realidad hay que "retornar" un MyVec no es que es un
        // metodo instancia y lo construyo-inicializo
        Self.ptr = std::move(p);
        Self.len = len;
        Self.cap = cap;
        //MyVec{ ptr: std::move(p), len: len, cap: cap }
    }
    pub fn from_capacity(cap: usize) -> Self {
        if cap == 0 { return new(); }
        else {
            let layout = Self::layout(cap);
            let p: *mut<T> = unsafe { alloc(layout) };
            if p.is_null() { handle_alloc_error(); }
            // Con el objetivo de modularizar me pregunto si puedo hacer una pub fn o private fn que
            // implemente MyVec para "construir" Self en base a parametros, esto es clasico en Java,
            // pero me da duda como deben pasar los parametros en todo caso, y si en realidad esta
            // idea va en contra a la filosofia de traits y fn impl y genericos en Rust y es muyJava
            new(p, 0, cap);
            // MyVec{p, 0, cap}
        }
    }
}

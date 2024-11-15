use std::ops::Deref;

struct Inner<T> {
    refcount: usize,
    data: T,
}

pub struct MyRc<T> {
    inner: *mut Inner<T>,
}

impl<T> MyRc<T> {
    pub fn new(value: T) -> Self {
        // Initialize the reference count to 1 and create the inner value.
        let inner = Box::into_raw(Box::new(Inner {
            refcount: 1,
            data: value,
        }));
        MyRc { inner }
    }
}

impl<T> Clone for MyRc<T> {
    fn clone(&self) -> Self {
        // SAFETY: We are updating the refcount of a valid `Inner` instance.
        unsafe {
            (*self.inner).refcount += 1;
        }
        // Return a new `MyRc` pointing to the same `Inner` data.
        MyRc { inner: self.inner }
    }
}

impl<T> Drop for MyRc<T> {
    fn drop(&mut self) {
        // SAFETY: We are checking and updating the refcount of a valid `Inner`.
        unsafe {
            (*self.inner).refcount -= 1;
            if (*self.inner).refcount == 0 {
                // SAFETY: Since the refcount is 0, we can free the memory.
                Box::from_raw(self.inner);
            }
        }
    }
}

impl<T> Deref for MyRc<T> {
    type Target = T;

    fn deref(&self) -> &T {
        // SAFETY: `inner` is a valid pointer and we are returning a reference to its data.
        unsafe { &(*self.inner).data }
    }
}


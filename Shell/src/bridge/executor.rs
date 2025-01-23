pub mod sen {

    pub mod shell {

        use std::ffi::{c_int};

        use crate::bridge::data::sen::shell::{CStringList, CStringView};

        pub type Callback = unsafe extern "C" fn(source: *mut CStringList, destination: *mut CStringView) -> c_int;

        pub type Execute = unsafe extern "C" fn(arguments: *mut CStringList, callback: Callback) -> c_int;
    }

}
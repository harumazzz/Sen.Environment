pub mod sen {

    pub mod shell {
        #[repr(C)]
        pub struct CStringView {
            pub value: *mut u8,
            pub size: usize,
        }

        #[repr(C)]
        pub struct CStringList {
            pub value:*mut CStringView,
            pub size: usize,
        }

    }

}

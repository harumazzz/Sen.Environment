pub mod sen {

    pub mod shell {

        use std::io::Write;
        use windows_sys::Win32::Globalization::CP_UTF8;
        use windows_sys::Win32::System::Console::{SetConsoleCP, SetConsoleOutputCP};

        pub struct Host {

        }

        impl Host {

            pub fn new() -> Host {
                Host {}
            }

            pub fn collect_arguments(
                &self
            ) -> Vec<String>
            {
                std::env::args().collect()
            }

            pub fn ensure_synchronization(
                &self
            )
            {
                std::io::stdout().flush().unwrap();
            }

            pub fn enable_unicode(
                &self
            ) -> ()
            {
                #[cfg(windows)]
                unsafe {
                    SetConsoleCP(CP_UTF8);
                    SetConsoleOutputCP(CP_UTF8);
                }
            }

        }

    }

}
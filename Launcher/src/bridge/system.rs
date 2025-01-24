pub mod sen {

    pub mod launcher {

        pub fn kernel(
            home: &String,
        ) -> String
        {
            #[cfg(windows)]
            return format!("{}/{}", home, "Kernel.dll");
            #[cfg(target_os = "android")]
            return String::from("libKernel.so");
            #[cfg(target_os = "ios")]
            return String::from("libKernel.dylib");
            #[cfg(target_os = "linux")]
            return format!("{}/{}", home, "libKernel.so");
            #[cfg(target_os = "macos")]
            return format!("{}/{}", home, "libKernel.dylib");
        }

        pub fn shell(
            home: &String,
        ) -> String
        {
            #[cfg(windows)]
            return format!("{}/{}", home, "Shell.exe");
            #[cfg(not(windows))]
            return format!("{}/{}", home, "Shell");
        }

        pub fn script(
            home: &String,
        ) -> String
        {
            return format!("{}/Script/{}", home, "main.js");
        }

    }

}
pub mod sen {

    pub mod shell {
        use std::ffi::{c_void, CString, OsStr};
        #[cfg(windows)]
        use std::os::windows::ffi::OsStrExt;
        #[cfg(windows)]
        use windows_sys;
        #[cfg(windows)]
        use windows_sys::Win32::{
            Foundation::HMODULE,
            System::LibraryLoader::{GetProcAddress, LoadLibraryW},
            Foundation::FreeLibrary,
        };
        #[cfg(not(windows))]
        use libc::{dlopen, RTLD_LAZY, RTLD_LOCAL, dlclose};
        use libc::{c_int, free, malloc};
        #[cfg(not(windows))]
        use libc::dlsym;
        use crate::bridge::convert::sen::shell::{to_cstring, to_vector};
        use crate::bridge::data::sen::shell::{CStringList, CStringView};
        use crate::bridge::executor::sen::shell::Execute;
        use crate::service::client::sen::shell::Client;
        use crate::service::host::sen::shell::Host;
        use crate::service::interaction::sen::shell::Interaction;
        use crate::utility::exception::sen::shell::assert_if;


        #[warn(unsafe_op_in_unsafe_fn)]
        unsafe extern "C" fn callback (
            source: *mut CStringList,
            destination: *mut CStringView,
        ) -> c_int
        {
            unsafe {
                assert_if((*source).size >= 1, "Not enough arguments");
            }
            let mut data: Vec<String> = Vec::new();
            to_vector(source, &mut data);
            match data[0].as_str() {
                "display" => Interaction::display_argument(&data),
                "wait" => Interaction::wait(),
                "version" => Interaction::current_version(destination),
                "is_gui" => Interaction::is_gui(destination),
                "input" =>  Interaction::input_string(destination),
                "pick_file" => Interaction::pick_file(destination),
                "pick_directory" => Interaction::pick_directory(destination),
                _ => {},
            };
            return 0;
        }

        pub struct Loader {
            arguments: Vec<String>,
            #[cfg(windows)]
            handle: Option<HMODULE>,
            #[cfg(not(windows))]
            handle: Option<*mut c_void>,
            argument_list: Option<*mut CStringList>,
        }

        impl Drop for Loader {
            fn drop(
                &mut self
            ) -> ()
            {
                unsafe {
                    self.release_argument();
                    self.free_library();
                }
            }
        }

        impl Loader {
            pub fn new(
                host: &Host
            ) -> Loader {
                Loader {
                    arguments: host.collect_arguments(),
                    handle: None,
                    argument_list: None,
                }
            }



            fn lookup_symbol(
                &self,
                symbol: &str
            ) -> Result<Execute, String>{
                #[cfg(windows)]
                {
                    unsafe {
                        let symbol_function = CString::new(symbol).unwrap();
                        let proc_address = GetProcAddress(self.handle.unwrap(), symbol_function.as_ptr().cast());
                        assert_if(!proc_address.is_none(), "Function not found.");
                        Ok(std::mem::transmute(proc_address))
                    }
                }
                #[cfg(not(windows))]
                {
                    unsafe {
                        let symbol_function = CString::new(symbol).unwrap();
                        let symbol_ptr = dlsym(self.handle.unwrap(), symbol_function.as_ptr());
                        assert_if(!symbol_ptr.is_null(), "Function not found.");
                        Ok(std::mem::transmute(symbol_ptr))
                    }
                }
            }

            fn free_library(
                &self
            ) -> ()
            {
                unsafe {
                    if !self.handle.is_none() {
                        #[cfg(windows)]
                        FreeLibrary(self.handle.unwrap());
                        #[cfg(not(windows))]
                        dlclose(self.handle.unwrap());
                    }
                }
            }

            unsafe fn release_argument (
                &self
            ) -> ()
            {
                unsafe {
                    for index in 0..(*self.argument_list.unwrap()).size {
                        if !(*(*self.argument_list.unwrap()).value.offset(index as isize)).value.is_null() {
                            free((*(*self.argument_list.unwrap()).value.offset(index as isize)).value as *mut c_void);
                        }
                    }
                    free(self.argument_list.unwrap() as *mut c_void);
                }
            }

            unsafe fn construct_argument (
                &mut self
            ) -> ()
            {
                unsafe {
                    self.argument_list = Some(malloc(std::mem::size_of::<CStringList>()) as *mut CStringList);
                    (*self.argument_list.unwrap()).size = self.arguments.len() as usize;
                    (*self.argument_list.unwrap()).value = malloc(std::mem::size_of::<CStringView>() * self.arguments.len()) as *mut CStringView;
                    for (index, argument) in self.arguments.iter().enumerate() {
                        let value: *mut CStringView = (*self.argument_list.unwrap()).value.offset(index as isize);
                        to_cstring(argument, value);
                    }
                }
            }
        }

        impl Client for Loader {

            fn initialize(
                &mut self
            ) -> ()
            {
                assert_if(self.arguments.len() >= 3, "Please use launcher to launch Sen");
                #[cfg(windows)]
                {
                    let kernel: Vec<u16> = OsStr::new(self.arguments[1].as_str())
                        .encode_wide()
                        .chain(Some(0))
                        .collect();
                    unsafe {
                        let handle = LoadLibraryW(kernel.as_ptr());
                        assert_if(!handle.is_null(), "Failed to load library");
                        self.handle = Option::from(handle);
                    };
                }
                #[cfg(not(windows))]
                {
                    let kernel = CString::new(self.arguments[1].clone()).map_err(|_| "Unable to load Kernel".to_string()).unwrap();
                    unsafe {
                        let handle = dlopen(kernel.as_ptr(), RTLD_LAZY | RTLD_LOCAL);
                        assert_if(!handle.is_null(), "Failed to load library");
                        self.handle = Option::from(handle);
                    }
                }
            }

            fn execute(
                &mut self
            ) -> ()
            {
                let execute = self.lookup_symbol("execute");
                assert_if(execute.is_ok(), "Failed to load execute function");
                let execute = execute.unwrap();
                unsafe {
                    self.construct_argument();
                    execute(self.argument_list.unwrap(), callback);
                }
            }
        }

    }

}
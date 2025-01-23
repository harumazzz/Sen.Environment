pub mod sen {

    pub mod shell {
        use colored::Colorize;
        use crate ::external::color::sen::shell::Color;
        use crate::bridge::convert::sen::shell::{to_cstring};
        use crate::bridge::data::sen::shell::{CStringView};
        use crate::constants::version::sen::shell::{IS_GUI, VERSION};
        use crate::external::color::sen::shell::exchange_color;
        use crate::utility::dialog::sen::shell::{pick_directory, pick_file};
        use crate::utility::exception::sen::shell::assert_if;

        pub struct Interaction {

        }

        impl Interaction {

            fn print_color(
                message: &String,
                color: &Color,
            ) {
                match color {
                    Color::Cyan => println!("{:?}", message.as_str().cyan()),
                    Color::Red => println!("{:?}", message.as_str().red()),
                    Color::Green => println!("{:?}", message.as_str().green()),
                    Color::Yellow => println!("{:?}", message.as_str().yellow()),
                    Color::Default => println!("{:?}", message),
                }
            }

            fn print(
                title: &String,
                message: &String,
                color: &Color,
            ) {
                Self::print_color(title, color);
                if !message.is_empty() {
                    println!("{}", message);
                }
            }

            fn input(

            ) -> String
            {
                let mut value: String = String::new();
                std::io::stdin().read_line(&mut value).expect("Unable to get the input value");
                value
            }

            pub(crate) unsafe fn input_string(
                destination: *mut CStringView,
            ){
                let value = Self::input();
                to_cstring(&value, destination);
            }

            pub(crate) fn display_argument (
                data: &Vec<String>
            )
            {
                assert_if(data.len() < 2, "Not enough arguments");
                match data.len() {
                    2 => println!("{}", data[1]),
                    3 => Self::print(&data[0], &data[1], &Color::Default),
                    _ => Self::print(&data[0], &data[1], &exchange_color(data[2].as_str())),
                }
            }

            pub(crate) fn wait (

            )
            {
                Self::print_color(&String::from("● "), &Color::Cyan);
            }

            pub(crate) unsafe fn current_version (
                destination: *mut CStringView,
            )
            {
                let version = VERSION.to_string();
                to_cstring(&version, destination);
            }

            pub(crate) unsafe fn is_gui (
                destination: *mut CStringView,
            )
            {
                let is_gui = IS_GUI.to_string();
                to_cstring(&is_gui, destination);
            }

            pub(crate) unsafe fn pick_file (
                destination: *mut CStringView,
            )
            {
                let file = pick_file();
                if !file.is_none() {
                    to_cstring(&file.unwrap(), destination);
                }
            }

            pub(crate) unsafe fn pick_directory (
                destination: *mut CStringView,
            )
            {
                let file = pick_directory();
                if !file.is_none() {
                    to_cstring(&file.unwrap(), destination);
                }
            }

        }

    }

}
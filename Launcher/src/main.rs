mod utility;
mod constant;
mod bridge;

use crate::bridge::invoke::sen::launcher::construct_arguments;
use crate::bridge::system::sen::launcher::{kernel, script, shell};
use crate::utility::system::sen::launcher::{current_directory, execute_command, pause};

fn run() -> Result<(), std::io::Error> {
    let home = current_directory().expect("Failed to determine home directory");
    let args = construct_arguments();
    let mut argument = Vec::new();
    argument.reserve(2 + args.len());
    argument.push(kernel(&home));
    argument.push(script(&home));
    argument.extend(args);
    if let Err(e) = execute_command(&shell(&home), &argument) {
        eprintln!("Failed to execute command: {}", e);
    }
    Ok(())
}

fn main() {
    match run() {
        Ok(_) => (),
        Err(e) => println!("Exception found:: {}", e),
    }
    pause();
}

pub mod service;
pub mod external;
pub mod constants;
pub mod utility;
pub mod bridge;

use crate ::service::loader::sen::shell::Loader;
use crate::service::client::sen::shell::Client;
use crate::service::host::sen::shell::Host;

fn run() -> Result<(), Box<dyn std::error::Error>> {
	let host: Host = Host::new();
	host.ensure_synchronization();
	#[cfg(windows)]
	host.enable_unicode();
	let mut loader = Loader::new(&host);
	loader.initialize();
	loader.execute();
	Ok(())
}

fn main() -> () {
	match run() {
		Ok(_) => {},
		Err(e) => eprintln!("Application encountered an error: {}", e),
	}
}

pub mod sen {

    pub mod launcher {
        use std::io::Write;
        use std::process::Command;

        pub fn current_directory(

        ) -> std::io::Result<String> {
            let exec_path = std::env::current_exe()?;
            let home_path = exec_path.parent()
                .map(|p| p.to_string_lossy().to_string())
                .ok_or_else(|| std::io::Error::new(std::io::ErrorKind::Other, "Failed to determine home path"))?;
            Ok(home_path)
        }

        pub fn execute_command(
            command: &String,
            arguments: &Vec<String>,
        ) -> std::io::Result<()> {
            let status = Command::new(command)
                .args(arguments)
                .status()?;
            if !status.success() {
                return Err(std::io::Error::new(
                    std::io::ErrorKind::Other,
                    format!("Command execution failed with status: {:?}", status.code()),
                ));
            }
            Ok(())
        }

        pub fn pause(

        ) {
            print!("Press Enter to continue...");
            std::io::stdout().flush().unwrap();
            let _ = std::io::stdin().read_line(&mut String::new());
        }

    }

}
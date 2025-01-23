pub mod sen {

    pub mod shell {

        use rfd::FileDialog;

        pub fn pick_file (

        ) -> Option<String>
        {
            let files = FileDialog::new()
                .add_filter("All files", &["*"])
                .set_directory("/")
                .pick_file();
            if let Some(file) = files {
                Option::from(file.as_path().to_str().unwrap().to_string())
            } else {
                None
            }
        }

        pub fn pick_directory (

        ) -> Option<String>
        {
            let folders = FileDialog::new()
                .set_directory("/")
                .pick_folder();
            if let Some(folder) = folders {
                Option::from(folder.as_path().to_str().unwrap().to_string())
            } else {
                None
            }
        }
    }
}
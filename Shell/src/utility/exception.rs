pub mod sen {

    pub mod shell {

        pub fn assert_if (condition: bool, message: &str) {
            if !condition {
                panic!("{}", message);
            }
        }

    }

}